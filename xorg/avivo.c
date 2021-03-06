/*
 * Copyright © 2007 Daniel Stone
 * Copyright © 2007 Matthew Garrett
 * Copyright © 2007 Jerome Glisse
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 2.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * A copy of the General Public License is included with the source
 * distribution of this driver, as COPYING.
 *
 * Authors: Daniel Stone <daniel@fooishbar.org>
 *          Matthew Garrett <mjg59@srcf.ucam.org>
 *          Jerome Glisse <glisse@freedesktop.org>
 *
 * Portions based on the Radeon and VESA drivers.
 */

/*
 * This is quite a primitive driver.  It's like radeontool, but in driver
 * form.  It doesn't support offscreen allocation.  Largely because it
 * _doesn't have an allocator_.  Not much point since there's no
 * acceleration yet, anyway.
 */
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <string.h>

#include "micmap.h"
#include "cursorstr.h"
#include "xf86Cursor.h"
#include "xf86str.h"
#include "xf86RandR12.h"
#include "xf86fbman.h"
#include "shadow.h"

#ifdef WITH_VGAHW
#include "vgaHW.h"
#endif

#include "avivo.h"
#include "radeon_reg.h"

/* DPMS */
#define DPMS_SERVER
#include <X11/extensions/dpms.h>

/* Mandatory functions */
static const OptionInfoRec *avivo_available_options(int chipid, int busid);
static void avivo_identify(int flags);
static Bool avivo_old_probe(DriverPtr driver, int flags);
#ifdef PCIACCESS
static Bool avivo_pci_probe(DriverPtr driver, int entity_num,
                            struct pci_device *dev, intptr_t match_data);
#endif
static Bool avivo_preinit(ScrnInfoPtr screen_info, int flags);
static Bool avivo_screen_init(int index, ScreenPtr screen, int argc,
                              char **argv);
static Bool avivo_enter_vt(int index, int flags);
static void avivo_leave_vt(int index, int flags);
static Bool avivo_close_screen(int index, ScreenPtr screen);
static Bool avivo_save_screen(ScreenPtr screen, int mode);

static Bool avivo_switch_mode(int index, DisplayModePtr mode, int flags);
static void avivo_adjust_frame(int index, int x, int y, int flags);
static void avivo_free_screen(int index, int flags);
static void avivo_free_info(ScrnInfoPtr screen_info);

/* 
 * This contains the functions needed by the server after loading the
 * driver module.  It must be supplied, and gets added the driver list by
 * the Module Setup funtion in the dynamic case.  In the static case a
 * reference to this is compiled in, and this requires that the name of
 * this DriverRec be an upper-case version of the driver name.
 */
_X_EXPORT DriverRec avivo_driver = {
    AVIVO_DRIVER_VERSION,
    "avivo",
    avivo_identify,
    avivo_old_probe,
    avivo_available_options,
    NULL,
    0,
    NULL,

#ifdef PCIACCESS
    avivo_device_match,
    avivo_pci_probe,
#endif
};


enum avivo_option_type {
    OPTION_LAYOUT,
    OPTION_SHADOW_FB,
};

static const OptionInfoRec avivo_options[] = {
    { OPTION_LAYOUT,       "MonitorLayout",     OPTV_STRING,    { 0 },  FALSE },
    { OPTION_SHADOW_FB,    "ShadowFB",         OPTV_BOOLEAN,    { 0 },  FALSE },
    { -1,                  NULL,                OPTV_NONE,      { 0 },  FALSE }
};

static const char *shadow_symboles[] = {
    "shadowAdd",
    "shadowInit",
    "shadowSetup",
    "shadowUpdatePacked",
    "shadowUpdatePackedWeak",
    NULL
};

/* Module loader interface */
static MODULESETUPPROTO(avivo_setup);

static XF86ModuleVersionInfo avivo_version = {
    "avivo",
    MODULEVENDORSTRING,
    MODINFOSTRING1,
    MODINFOSTRING2,
    XORG_VERSION_CURRENT,
    PACKAGE_VERSION_MAJOR, PACKAGE_VERSION_MINOR, PACKAGE_VERSION_PATCHLEVEL,
    ABI_CLASS_VIDEODRV,
    ABI_VIDEODRV_VERSION,
    MOD_CLASS_VIDEODRV,
    { 0, 0, 0, 0, },
};

/*
 * This data is accessed by the loader.  The name must be the module name
 * followed by "ModuleData".
 */
_X_EXPORT XF86ModuleData avivoModuleData = { &avivo_version, avivo_setup, NULL };

#ifndef PCIACCESS
static int
avivo_map_ctrl_mem(ScrnInfoPtr screen_info)
{
    struct avivo_info *avivo = avivo_get_info(screen_info);

    if (avivo->ctrl_base)
        return 1;

    avivo->ctrl_base = xf86MapPciMem(screen_info->scrnIndex,
                                     VIDMEM_MMIO | VIDMEM_READSIDEEFFECT,
                                     avivo->pci_tag, avivo->ctrl_addr,
                                     avivo->ctrl_size);
    if (!avivo->ctrl_base) {
        xf86DrvMsg(screen_info->scrnIndex, X_ERROR,
                   "Couldn't map control memory at %p", (void *)avivo->ctrl_addr);
        return 0;
    }
    return 1;
}

static int
avivo_map_fb_mem(ScrnInfoPtr screen_info)
{
    struct avivo_info *avivo = avivo_get_info(screen_info);

    if (avivo->fb_base)
        return 0;

    avivo->fb_base = xf86MapPciMem(screen_info->scrnIndex, VIDMEM_FRAMEBUFFER,
                                   avivo->pci_tag, avivo->fb_addr,
                                   avivo->fb_size);
    if (!avivo->fb_base) {
        xf86DrvMsg(screen_info->scrnIndex, X_ERROR,
                   "Couldn't map fb memory at %p", (void *)avivo->fb_addr);
        return 0;
    }
    screen_info->memPhysBase = 0;
    screen_info->fbOffset = 0;

    return 1;
}
#endif /*PCIACCESS*/

static void
avivo_unmap_ctrl_mem(ScrnInfoPtr screen_info)
{
    struct avivo_info *avivo = avivo_get_info(screen_info);

#ifdef PCIACCESS
    ;
#else
    xf86UnMapVidMem(screen_info->scrnIndex, avivo->ctrl_base, avivo->ctrl_size);
#endif
    avivo->ctrl_base = NULL;
}

static void
avivo_unmap_fb_mem(ScrnInfoPtr screen_info)
{
    struct avivo_info *avivo = avivo_get_info(screen_info);

#ifdef PCIACCESS
    ;
#else
    xf86UnMapVidMem(screen_info->scrnIndex, avivo->fb_base, avivo->fb_size);
#endif

    avivo->fb_base = NULL;
}

static pointer
avivo_setup(pointer module, pointer options, int *err_major, int *err_minor)
{
    static Bool inited = FALSE;

    if (!inited) {
        inited = TRUE;
        xf86AddDriver(&avivo_driver, module, 1);
        LoaderRefSymLists(shadow_symboles, NULL);
        return (pointer) TRUE;
    }

    if (err_major)
        *err_major = LDR_ONCEONLY;

    return NULL;
}

static const OptionInfoRec *
avivo_available_options(int chipid, int busid)
{
    return avivo_options;
}

static void
avivo_identify(int flags)
{
    xf86PrintChipsets("Avivo", "driver for Radeon r5xx chipsets",
                      avivo_chips);
}

void fill_in_screen(ScrnInfoPtr screen_info)
{
    screen_info->driverVersion = AVIVO_DRIVER_VERSION;
    screen_info->driverName = AVIVO_DRIVER_NAME;
    screen_info->name = AVIVO_NAME;
    screen_info->Probe = avivo_old_probe;
    screen_info->PreInit = avivo_preinit;
    screen_info->ScreenInit = avivo_screen_init;
    screen_info->SwitchMode = avivo_switch_mode;
    screen_info->AdjustFrame = avivo_adjust_frame;
    screen_info->EnterVT = avivo_enter_vt;
    screen_info->LeaveVT = avivo_leave_vt;
    screen_info->FreeScreen = avivo_free_screen;
}

/*
 * This function is called once, at the start of the first server generation to
 * do a minimal probe for supported hardware.
 */

#ifdef PCIACCESS
static Bool
avivo_pci_probe(DriverPtr drv, int entity_num, struct pci_device *dev,
                intptr_t match_data)
{
    ScrnInfoPtr screen_info;
    struct avivo_info *avivo;
    
    screen_info = xf86ConfigPciEntity(NULL, 0, entity_num, NULL, 
                                      NULL, NULL, NULL, NULL, NULL);
    if (screen_info) {
        avivo = avivo_get_info(screen_info);
        fill_in_screen(screen_info);
        avivo->pci_info = dev;
    }
    
    return !!screen_info;
}
#endif

/* God, this is a crap interface.  No wonder we don't use it any more. */
static Bool
avivo_old_probe(DriverPtr drv, int flags)
{
    ScrnInfoPtr screen_info = NULL;
    GDevPtr *sections;
    Bool found_screen = FALSE;
    int num_sections, used_sections;
    int *used_chips;
    int i;

    /*
     * Find the config file Device sections that match this
     * driver, and return if there are none.
     */
    num_sections = xf86MatchDevice("avivo", &sections);
    if (num_sections <= 0)
        return FALSE;

/*#ifndef PCIACCESS*/
    used_sections = xf86MatchPciInstances(AVIVO_NAME, PCI_VENDOR_ATI,
                                          avivo_chips, avivo_pci_chips, 
                                          sections, num_sections, drv,
                                          &used_chips);

    if (used_sections > 0) {
        if (flags & PROBE_DETECT) {
            found_screen = TRUE;
        }
        else {
            for (i = 0; i < used_sections; i++) {
                screen_info = xf86ConfigPciEntity(screen_info, 0, used_chips[i],
                                                  avivo_pci_chips, NULL,
                                                  NULL, NULL, NULL, NULL);
                if (screen_info) {
                    fill_in_screen(screen_info);
                    found_screen = TRUE;
                }
            }
        }

        xfree(used_chips);
    }
/*#endif*/

    xfree(sections);

    return found_screen;
}

static void
avivo_free_info(ScrnInfoPtr screen_info)
{
}

/*
 * This function is called once for each screen at the start of the first
 * server generation to initialise the screen for all server generations.
 */
static Bool
avivo_preinit(ScrnInfoPtr screen_info, int flags)
{
    struct avivo_info *avivo;
    int i;
    Gamma gzeros = { 0.0, 0.0, 0.0 };
    rgb rzeros = { 0, 0, 0 };

    /* for time being no force option so we refuse to initialize on
     * unprobed hw
     */
    if (flags & PROBE_DETECT)
        return FALSE;

    /* load sub module needed by avivo */
    if (!xf86LoadSubModule(screen_info, "fb"))
        FatalError("Couldn't load fb\n");
    if (!xf86LoadSubModule(screen_info, "ramdac"))
        FatalError("Couldn't load ramdac\n");

#ifdef WITH_VGAHW
    /* vga hw support enabled, use vga to restore state on VT switch */
    xf86LoadSubModule(screen_info, "vgahw");
    vgaHWGetHWRec (screen_info);
    vgaHWGetIOBase(VGAHWPTR(screen_info));
#endif

    /* initialize private structure */
    avivo = avivo_get_info(screen_info);
    avivo->entity = xf86GetEntityInfo(screen_info->entityList[0]);
    avivo->device = xf86GetDevFromEntity(screen_info->entityList[0],
                                         screen_info->entityInstanceList[0]);

#ifndef PCIACCESS
    /* get PCI informations */
    avivo->pci_info = xf86GetPciInfoForEntity(avivo->entity->index);
    avivo->pci_tag = pciTag(avivo->pci_info->bus, avivo->pci_info->device,
                            avivo->pci_info->func);

    /* Map MMIO space first, then the framebuffer. */
    for (i = 0; i < 6; i++) {
        if (avivo->pci_info->size[i] == 15 || avivo->pci_info->size[i] == 16) {
            avivo->ctrl_addr = avivo->pci_info->memBase[i] & 0xffffff00;
            avivo->ctrl_size = (1 << avivo->pci_info->size[i]);
            avivo_map_ctrl_mem(screen_info);
        }
    }
    for (i = 0; i < 6; i++) {
        if (avivo->pci_info->size[i] >= 26) {
            avivo->fb_addr = avivo->pci_info->memBase[i] & 0xfe000000;
            avivo->fb_size = INREG(RADEON_CONFIG_MEMSIZE);
            /* FIXME: we can't map more than 256Mo better solution would be
             * to get aperture size */
            if (avivo->fb_size > 0x10000000)
                avivo->fb_size = 0x10000000;
            screen_info->videoRam = avivo->fb_size / 1024;
            avivo_map_fb_mem(screen_info);
        }
    }
#else /*PCIACCESS*/
    /* get PCI informations */
    avivo->pci_info = xf86GetPciInfoForEntity(avivo->entity->index);

    if (pci_device_probe (avivo->pci_info)) {
        FatalError("failed to probe PCI device\n");
    }

    /*
     * Map MMIO space first, then the framebuffer.
     */

    /*so first the MMIO space ...*/
    for (i = 0; i < 6; i++) {
        /*
         * the mmio space is the one pointed to by the BAR
         * that has 15 or 16 bits length (usually 64K size, 16 bits).
         */
        if (avivo->pci_info->regions[i].size == 0x10000/*sizeof(16bits)*/
            ||avivo->pci_info->regions[i].size == 0x8000/*sizeof(15bits)*/) {
            if (!avivo->pci_info->regions[i].memory) {
                if (pci_device_map_region(avivo->pci_info,
                                          i,
                                          1/*write_enable*/)) {

                    FatalError("Could not map pci region %d,"
                               " device %#x, vendor %#x\n",
                               i,
                               avivo->pci_info->device_id,
                               avivo->pci_info->vendor_id);
                }
            }
            avivo->ctrl_addr = avivo->pci_info->regions[i].base_addr ;
            avivo->ctrl_size = avivo->pci_info->regions[i].size ;
            avivo->ctrl_base = avivo->pci_info->regions[i].memory;
        }
    }
    /*...now, map the framebuffer memory*/
    for (i = 0; i < 6; i++) {
        /*
         * the framebuffer buffer is the one pointed to by
         * the BAR having a size >= 26 bits
         */
        if (avivo->pci_info->regions[i].size >= 0x4000000/*size(26bits)*/) {
            if (!avivo->pci_info->regions[i].memory) {
                if (pci_device_map_region(avivo->pci_info,
                                          i,
                                          1/*write_enable*/)) {

                    FatalError("Could not map pci region %d,"
                               " device %#x, vendor %#x\n",
                               i,
                               avivo->pci_info->device_id,
                               avivo->pci_info->vendor_id);
                }
            }
            avivo->fb_addr = avivo->pci_info->regions[i].base_addr;
            avivo->fb_base = avivo->pci_info->regions[i].memory ;
            avivo->fb_size = INREG(RADEON_CONFIG_MEMSIZE);
            /*
             * fb memory size should be the minum of bar size
             * and the result of INREG(RADEON_CONFIG_MEMSIZE);
             * Normally both should be the same, but who knows
             * what some card manufacturers do sometimes ...
             */
            if (avivo->pci_info->regions[i].size < avivo->fb_size)
                avivo->fb_size = avivo->pci_info->regions[i].size ;

            /* FIXME: we can't map more than 256Mo better solution would be
             * to get aperture size */
            if (avivo->fb_size > 0x10000000)
                avivo->fb_size = 0x10000000;
            screen_info->videoRam = avivo->fb_size / 1024;
        }
    }

#endif
    xf86DrvMsg(screen_info->scrnIndex, X_INFO,
               "Control memory at %p[size = %d, 0x%08X]\n",
               (void *)avivo->ctrl_addr, avivo->ctrl_size, avivo->ctrl_size);
    xf86DrvMsg(screen_info->scrnIndex, X_INFO,
               "Frame buffer memory at %p[size = %d, 0x%08X]\n",
               (void *)avivo->fb_addr, avivo->fb_size, avivo->fb_size);

    /* now we got PCI informations we can check which chipset family we
     * have to deal with
     */
    avivo_get_chipset(avivo);
    screen_info->chipset = "avivo";
    screen_info->monitor = screen_info->confScreen->monitor;

    /* set screen color depth */
    if (!xf86SetDepthBpp(screen_info, 0, 0, 0, Support32bppFb))
        return FALSE;
    xf86PrintDepthBpp(screen_info);
    switch (screen_info->depth) {
    case 16:
        avivo->bpp = 2;
        break;
    case 24:
    case 32:
        avivo->bpp = 4;
        break;
    default:
        FatalError("Unsupported screen depth: %d\n", xf86GetDepth());
    }

    /* options */
    xf86CollectOptions(screen_info, NULL);
    avivo->options = xalloc(sizeof(avivo_options));
    if (avivo->options == NULL)
        return FALSE;
    memcpy(avivo->options, avivo_options, sizeof(avivo_options));
    xf86ProcessOptions(screen_info->scrnIndex, screen_info->options,
                       avivo->options);
    /* use shadow framebuffer by default */
    avivo->fb_use_shadow = xf86ReturnOptValBool(avivo->options,
                                                OPTION_SHADOW_FB, TRUE);

    /* create crtrc & output */
    if (!avivo_crtc_create(screen_info))
        return FALSE;
    avivo_output_setup(screen_info);
    if (!xf86InitialConfiguration(screen_info, FALSE)) {
        xf86DrvMsg(screen_info->scrnIndex, X_ERROR, "No valid modes.\n");
        return FALSE;
    }
    /* check if there modes available */
    if (!xf86RandR12PreInit(screen_info)) {
        xf86DrvMsg(screen_info->scrnIndex, X_ERROR,
                   "RandR initialization failure\n");
        return FALSE;
    }	
    if (screen_info->modes == NULL) {
        xf86DrvMsg(screen_info->scrnIndex, X_ERROR, "No modes available\n");
        return FALSE;
    }
    screen_info->currentMode = screen_info->modes;

    /* color weight */
    if (!xf86SetWeight(screen_info, rzeros, rzeros))
        return FALSE;
    /* visual init */
    if (!xf86SetDefaultVisual(screen_info, -1))
        return FALSE;
    /* TODO: gamma correction */
    xf86SetGamma(screen_info, gzeros);
    /* Set display resolution */
    xf86SetDpi(screen_info, 100, 100);

    /* load shadow if needed */
    if (avivo->fb_use_shadow) {
        xf86DrvMsg(screen_info->scrnIndex, X_INFO,
                   "using shadow framebuffer\n");
        if (!xf86LoadSubModule(screen_info, "shadow"))
            return FALSE;
        xf86LoaderReqSymLists(shadow_symboles, NULL);
    }

    xf86DrvMsg(screen_info->scrnIndex, X_INFO,
               "pre-initialization successfull\n");
    return TRUE;
}

static Bool
avivo_save_screen(ScreenPtr screen, int mode)
{
    Bool on = xf86IsUnblank(mode);

    if (on)
        SetTimeSinceLastInputEvent();

    return TRUE;
}

static void *
avivo_window_linear(ScreenPtr screen, CARD32 row, CARD32 offset, int mode,
                    CARD32 *size, void *closure)
{
    ScrnInfoPtr screen_info = xf86Screens[screen->myNum];
    struct avivo_info *avivo = avivo_get_info(screen_info);
    int stride;
#if 0
    if (!screen_info->vtSema)
        return NULL;
#endif
    stride = (screen_info->displayWidth * screen_info->bitsPerPixel) / 8;
    *size = stride;

    return ((CARD8 *)avivo->fb_base + screen_info->fbOffset +
            row * stride + offset);
}

static Bool
avivo_create_screen_resources(ScreenPtr screen)
{
    PixmapPtr pixmap;
    ScrnInfoPtr screen_info = xf86Screens[screen->myNum];
    struct avivo_info *avivo = avivo_get_info(screen_info);
    Bool ret;

    screen->CreateScreenResources = avivo->create_screen_resources;
    ret = screen->CreateScreenResources(screen);
    screen->CreateScreenResources = avivo_create_screen_resources;
    if (!ret)
        return FALSE;

    pixmap = screen->GetScreenPixmap(screen);

    if (!shadowAdd(screen, pixmap, shadowUpdatePackedWeak(),
                   avivo_window_linear, 0, NULL))
        return FALSE;

    return TRUE;
}

static Bool
avivo_shadow_init(ScreenPtr screen)
{
    ScrnInfoPtr screen_info = xf86Screens[screen->myNum];
    struct avivo_info *avivo = avivo_get_info(screen_info);
     
    if (!shadowSetup(screen)) {
        return FALSE;
    }
     
    avivo->create_screen_resources = screen->CreateScreenResources;
    screen->CreateScreenResources = avivo_create_screen_resources;
     
    return TRUE;
}

static Bool
avivo_screen_init(int index, ScreenPtr screen, int argc, char **argv)
{
    ScrnInfoPtr screen_info = xf86Screens[index];
    struct avivo_info *avivo = avivo_get_info(screen_info);
    xf86CrtcConfigPtr xf86_config = XF86_CRTC_CONFIG_PTR(screen_info);
    VisualPtr visual;
    void *fbstart;
    int i;


#ifndef PCIACCESS
    /* Map MMIO space first, then the framebuffer. */
    for (i = 0; i < 6; i++) {
        if (avivo->pci_info->size[i] == 15 || avivo->pci_info->size[i] == 16) {
            avivo->ctrl_addr = avivo->pci_info->memBase[i] & 0xffffff00;
            avivo->ctrl_size = (1 << avivo->pci_info->size[i]);
            avivo_map_ctrl_mem(screen_info);
        }
    }
    for (i = 0; i < 6; i++) {
        if (avivo->pci_info->size[i] >= 26) {
            avivo->fb_addr = avivo->pci_info->memBase[i] & 0xfe000000;
            avivo->fb_size = INREG(RADEON_CONFIG_MEMSIZE);
            screen_info->videoRam = avivo->fb_size / 1024;
            avivo_map_fb_mem(screen_info);
        }
    }
#endif
    avivo_save_state(screen_info);
    avivo_setup_gpu_memory_map(screen_info);
    /* display width is the higher resolution from width & height */
    if (screen_info->virtualX > screen_info->displayWidth)
        screen_info->displayWidth = screen_info->virtualX;
    /* display width * bpp need to be a multiple of 256 */
    screen_info->displayWidth = (screen_info->displayWidth + 255) & (~0xFF);
    xf86DrvMsg(screen_info->scrnIndex, X_INFO,
               "padded display width %d\n", screen_info->displayWidth);
    /* mi layer */
    miClearVisualTypes();
    if (!xf86SetDefaultVisual(screen_info, -1)) {
        xf86DrvMsg(screen_info->scrnIndex, X_ERROR,
                   "Couldn't set default visual\n");
        return FALSE;
    }
    if (!miSetVisualTypes(screen_info->depth, TrueColorMask,
                          screen_info->rgbBits, TrueColor)) {
        xf86DrvMsg(screen_info->scrnIndex, X_ERROR,
                   "Couldn't set visual types\n");
        return FALSE;
    }
    if (!miSetPixmapDepths()) {
        xf86DrvMsg(screen_info->scrnIndex, X_ERROR,
                   "Couldn't set pixmap depth\n");
        return FALSE;
    }
    if (avivo->fb_use_shadow) {
        avivo->fb_shadow = xcalloc(1,
                                   screen_info->virtualX *
                                   screen_info->virtualY *
                                   screen_info->bitsPerPixel / 8);
        if (avivo->fb_shadow == NULL) {
            xf86DrvMsg(screen_info->scrnIndex, X_ERROR,
                       "Failed to allocate shadow framebuffer\n");
            return FALSE;
        }
        fbstart = avivo->fb_shadow;
    } else {
        fbstart = avivo->fb_base + screen_info->fbOffset;
    }
    ErrorF("VirtualX,Y %d, %d\n",
           screen_info->virtualX, screen_info->virtualY);
    if (!fbScreenInit(screen, fbstart,
                      screen_info->virtualX, screen_info->virtualY,
                      screen_info->xDpi, screen_info->yDpi,
                      screen_info->displayWidth, screen_info->bitsPerPixel)) {
        xf86DrvMsg(screen_info->scrnIndex, X_ERROR,
                   "Couldn't init fb\n");
        return FALSE;
    }
    /* Fixup RGB ordering */
    visual = screen->visuals + screen->numVisuals;
    while (--visual >= screen->visuals) {
        if ((visual->class | DynamicClass) == DirectColor) {
            visual->offsetRed = screen_info->offset.red;
            visual->offsetGreen = screen_info->offset.green;
            visual->offsetBlue = screen_info->offset.blue;
            visual->redMask  = screen_info->mask.red;
            visual->greenMask = screen_info->mask.green;
            visual->blueMask = screen_info->mask.blue;
        }
    }
    /* must be after RGB ordering fixed */
    fbPictureInit(screen, 0, 0);
    xf86SetBlackWhitePixels(screen);

    if (avivo->fb_use_shadow && !avivo_shadow_init(screen)) {
        xf86DrvMsg(screen_info->scrnIndex, X_ERROR,
                   "shadow framebuffer initialization failed\n");
        return FALSE;
    }

    for (i = 0; i < xf86_config->num_crtc; i++) {
        xf86CrtcPtr crtc = xf86_config->crtc[i];
        /* Mark that we'll need to re-set the mode for sure */
        memset(&crtc->mode, 0, sizeof(crtc->mode));
        if (!crtc->desiredMode.CrtcHDisplay) {
            memcpy(&crtc->desiredMode, screen_info->currentMode,
                   sizeof(crtc->desiredMode));
            crtc->desiredRotation = RR_Rotate_0;
            crtc->desiredX = 0;
            crtc->desiredY = 0;
        }

        if (!xf86CrtcSetMode (crtc, &crtc->desiredMode, crtc->desiredRotation,
                              crtc->desiredX, crtc->desiredY)) {
            xf86DrvMsg(screen_info->scrnIndex, X_ERROR,
                       "Couldn't set crtc mode\n");
            return FALSE;
        }
    }
    /* set the viewport */
    avivo_adjust_frame(index, screen_info->frameX0, screen_info->frameY0, 0);

    xf86DPMSInit(screen, xf86DPMSSet, 0);

    miDCInitialize(screen, xf86GetPointerScreenFuncs());

    if (!miCreateDefColormap(screen)) {
        xf86DrvMsg(screen_info->scrnIndex, X_ERROR,
                   "Couldn't create colormap\n");
        return FALSE;
    }

    /* Report any unused options (only for the first generation) */
    if (serverGeneration == 1)
        xf86ShowUnusedOptions(screen_info->scrnIndex, screen_info->options);

    screen->SaveScreen = avivo_save_screen;
    avivo->close_screen = screen->CloseScreen;
    screen->CloseScreen = avivo_close_screen;

    if (!xf86CrtcScreenInit(screen)) {
        xf86DrvMsg(screen_info->scrnIndex, X_ERROR,
                   "Couldn't initialize crtc\n");
        return FALSE;
    }

    xf86DrvMsg(screen_info->scrnIndex, X_INFO, "initialization successfull\n");
    return TRUE;
}

static Bool
avivo_enter_vt(int index, int flags)
{
    ScrnInfoPtr screen_info = xf86Screens[index];
    vgaHWPtr vga_hw = VGAHWPTR(screen_info);

#ifdef WITH_VGAHW
    vgaHWUnlock(vga_hw);
    vgaHWSave(screen_info, &vga_hw->SavedReg, VGA_SR_MODE | VGA_SR_FONTS);
    vgaHWLock(vga_hw);
#endif
    avivo_save_state(screen_info);
    avivo_setup_gpu_memory_map(screen_info);

    screen_info->vtSema = TRUE;
    if (!xf86SetDesiredModes(screen_info))
        return FALSE;
    avivo_adjust_frame(index, screen_info->frameX0, screen_info->frameY0, 0);

    return TRUE;
}

static void
avivo_leave_vt(int index, int flags)
{
    ScrnInfoPtr screen_info = xf86Screens[index];
    vgaHWPtr vga_hw = VGAHWPTR(screen_info);

    avivo_restore_state(screen_info);
#ifdef WITH_VGAHW
    vgaHWUnlock(vga_hw);
    vgaHWRestore(screen_info, &vga_hw->SavedReg, VGA_SR_MODE | VGA_SR_FONTS);
    vgaHWLock(vga_hw);
#endif
}

static Bool
avivo_switch_mode(int index, DisplayModePtr mode, int flags)
{
    ScrnInfoPtr screen_info = xf86Screens[index];

    xf86DrvMsg(screen_info->scrnIndex, X_INFO,
               "set mode: hdisp %d, htotal %d, hss %d, hse %d, hsk %d\n",
               mode->HDisplay, mode->HTotal, mode->HSyncStart, mode->HSyncEnd,
               mode->HSkew);
    xf86DrvMsg(screen_info->scrnIndex, X_INFO,
               "      vdisp %d, vtotal %d, vss %d, vse %d, vsc %d\n",
               mode->VDisplay, mode->VTotal, mode->VSyncStart, mode->VSyncEnd, 
               mode->VScan);
 
    return xf86SetSingleMode (screen_info, mode, RR_Rotate_0);
}

static void
avivo_adjust_frame(int index, int x, int y, int flags)
{
    ScrnInfoPtr screen_info = xf86Screens[index];
    struct avivo_info *avivo = avivo_get_info(xf86Screens[index]);
    xf86CrtcConfigPtr config = XF86_CRTC_CONFIG_PTR(screen_info);
    xf86OutputPtr output = config->output[config->compat_output];
    xf86CrtcPtr crtc = output->crtc;
    struct avivo_crtc_private *avivo_crtc = crtc->driver_private;

    xf86DrvMsg(screen_info->scrnIndex, X_INFO,
               "adjust frame: %d %d %d %d\n", index, x, y, flags);
    if (crtc && crtc->enabled) {
        x = x & ~3;
        OUTREG(AVIVO_CRTC1_OFFSET_START + avivo_crtc->crtc_offset,
               (x << 16) | y);
        OUTREG(AVIVO_CRTC1_OFFSET_END + avivo_crtc->crtc_offset,
            ((crtc->mode.HDisplay + x - 128) << 16) |
            (crtc->mode.HDisplay + y -128));
        crtc->x = output->initial_x + x;
        crtc->y = output->initial_y + y;
    }
}

static void
avivo_free_screen(int index, int flags)
{
    ScrnInfoPtr screen_info = xf86Screens[index];

    xf86DrvMsg(screen_info->scrnIndex, X_INFO, "free screen\n");
#ifdef WITH_VGAHW
    vgaHWFreeHWRec(screen_info);
#endif
    avivo_free_info(xf86Screens[index]);
}

static Bool
avivo_close_screen(int index, ScreenPtr screen)
{
    ScrnInfoPtr screen_info = xf86Screens[index];
    struct avivo_info *avivo = avivo_get_info(screen_info);

    xf86DrvMsg(screen_info->scrnIndex, X_INFO, "close screen\n");
    if (screen_info->vtSema == TRUE) {
        avivo_leave_vt(index, 0);
    }
    avivo_unmap_ctrl_mem(screen_info);
    avivo_unmap_fb_mem(screen_info);
#ifdef WITH_VGAHW
    vgaHWUnmapMem(screen_info);
#endif
    screen_info->vtSema = FALSE;

    if (avivo->fb_shadow) {
        xfree(avivo->fb_shadow);
        avivo->fb_shadow = NULL;
    }

    screen->CloseScreen = avivo->close_screen;
    return screen->CloseScreen(index, screen);
}
