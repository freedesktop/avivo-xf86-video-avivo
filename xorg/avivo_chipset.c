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
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include "avivo.h"

#ifdef PCIACCESS
const struct pci_id_match avivo_device_match[] = {
    {
        PCI_VENDOR_ATI, 0x7248, PCI_MATCH_ANY, PCI_MATCH_ANY,
        0x00030000, 0x00ffffff, 0
    },
    {
        PCI_VENDOR_ATI, 0x71D2, PCI_MATCH_ANY, PCI_MATCH_ANY,
        0x00030000, 0x00ffffff, 0
    },
    {
        PCI_VENDOR_ATI, 0x71DA, PCI_MATCH_ANY, PCI_MATCH_ANY,
        0x00030000, 0x00ffffff, 0
    },
    {
        PCI_VENDOR_ATI, 0x7105, PCI_MATCH_ANY, PCI_MATCH_ANY,
        0x00030000, 0x00ffffff, 0
    },
    {
        PCI_VENDOR_ATI, 0x7104, PCI_MATCH_ANY, PCI_MATCH_ANY,
        0x00030000, 0x00ffffff, 0
    },
    {
        PCI_VENDOR_ATI, 0x710E, PCI_MATCH_ANY, PCI_MATCH_ANY,
        0x00030000, 0x00ffffff, 0
    },
    {
        PCI_VENDOR_ATI, 0x710F, PCI_MATCH_ANY, PCI_MATCH_ANY,
        0x00030000, 0x00ffffff, 0
    },
    {
        PCI_VENDOR_ATI, 0x719B, PCI_MATCH_ANY, PCI_MATCH_ANY,
        0x00030000, 0x00ffffff, 0
    },
    {
        PCI_VENDOR_ATI, 0x724E, PCI_MATCH_ANY, PCI_MATCH_ANY,
        0x00030000, 0x00ffffff, 0
    },
    {
        PCI_VENDOR_ATI, 0x71C4, PCI_MATCH_ANY, PCI_MATCH_ANY,
        0x00030000, 0x00ffffff, 0
    },
    {
        PCI_VENDOR_ATI, 0x71D4, PCI_MATCH_ANY, PCI_MATCH_ANY,
        0x00030000, 0x00ffffff, 0
    },
    {
        PCI_VENDOR_ATI, 0x7106, PCI_MATCH_ANY, PCI_MATCH_ANY,
        0x00030000, 0x00ffffff, 0
    },
    {
        PCI_VENDOR_ATI, 0x7103, PCI_MATCH_ANY, PCI_MATCH_ANY,
        0x00030000, 0x00ffffff, 0
    },
    {
        PCI_VENDOR_ATI, 0x714A, PCI_MATCH_ANY, PCI_MATCH_ANY,
        0x00030000, 0x00ffffff, 0
    },
    {
        PCI_VENDOR_ATI, 0x7149, PCI_MATCH_ANY, PCI_MATCH_ANY,
        0x00030000, 0x00ffffff, 0
    },
    {
        PCI_VENDOR_ATI, 0x714B, PCI_MATCH_ANY, PCI_MATCH_ANY,
        0x00030000, 0x00ffffff, 0
    },
    {
        PCI_VENDOR_ATI, 0x714C, PCI_MATCH_ANY, PCI_MATCH_ANY,
        0x00030000, 0x00ffffff, 0
    },
    {
        PCI_VENDOR_ATI, 0x718B, PCI_MATCH_ANY, PCI_MATCH_ANY,
        0x00030000, 0x00ffffff, 0
    },
    {
        PCI_VENDOR_ATI, 0x718C, PCI_MATCH_ANY, PCI_MATCH_ANY,
        0x00030000, 0x00ffffff, 0
    },
    {
        PCI_VENDOR_ATI, 0x7196, PCI_MATCH_ANY, PCI_MATCH_ANY,
        0x00030000, 0x00ffffff, 0
    },
    {
        PCI_VENDOR_ATI, 0x7145, PCI_MATCH_ANY, PCI_MATCH_ANY,
        0x00030000, 0x00ffffff, 0
    },
    {
        PCI_VENDOR_ATI, 0x7186, PCI_MATCH_ANY, PCI_MATCH_ANY,
        0x00030000, 0x00ffffff, 0
    },
    {
        PCI_VENDOR_ATI, 0x718D, PCI_MATCH_ANY, PCI_MATCH_ANY,
        0x00030000, 0x00ffffff, 0
    },
    {
        PCI_VENDOR_ATI, 0x71C5, PCI_MATCH_ANY, PCI_MATCH_ANY,
        0x00030000, 0x00ffffff, 0
    },
    {
        PCI_VENDOR_ATI, 0x71D5, PCI_MATCH_ANY, PCI_MATCH_ANY,
        0x00030000, 0x00ffffff, 0
    },
    {
        PCI_VENDOR_ATI, 0x71DE, PCI_MATCH_ANY, PCI_MATCH_ANY,
        0x00030000, 0x00ffffff, 0
    },
    {
        PCI_VENDOR_ATI, 0x71D6, PCI_MATCH_ANY, PCI_MATCH_ANY,
        0x00030000, 0x00ffffff, 0
    },
    {
        PCI_VENDOR_ATI, 0x7102, PCI_MATCH_ANY, PCI_MATCH_ANY,
        0x00030000, 0x00ffffff, 0
    },
    {
        PCI_VENDOR_ATI, 0x7101, PCI_MATCH_ANY, PCI_MATCH_ANY,
        0x00030000, 0x00ffffff, 0
    },
    {
        PCI_VENDOR_ATI, 0x7284, PCI_MATCH_ANY, PCI_MATCH_ANY,
        0x00030000, 0x00ffffff, 0
    },
    {
        PCI_VENDOR_ATI, 0x791E, PCI_MATCH_ANY, PCI_MATCH_ANY,
        0x00030000, 0x00ffffff, 0
    },
    {
        PCI_VENDOR_ATI, 0x791F, PCI_MATCH_ANY, PCI_MATCH_ANY,
        0x00030000, 0x00ffffff, 0
    },
    {
        PCI_VENDOR_ATI, 0x7288, PCI_MATCH_ANY, PCI_MATCH_ANY,
        0x00030000, 0x00ffffff, 0
    },
    {
        PCI_VENDOR_ATI, 0x793F, PCI_MATCH_ANY, PCI_MATCH_ANY,
        0x00030000, 0x00ffffff, 0
    },
    {
        PCI_VENDOR_ATI, 0x7941, PCI_MATCH_ANY, PCI_MATCH_ANY,
        0x00030000, 0x00ffffff, 0
    },
    {
        PCI_VENDOR_ATI, 0x7942, PCI_MATCH_ANY, PCI_MATCH_ANY,
        0x00030000, 0x00ffffff, 0
    },
    {
        PCI_VENDOR_ATI, 0x7146, PCI_MATCH_ANY, PCI_MATCH_ANY,
        0x00030000, 0x00ffffff, 0
    },
    {
        PCI_VENDOR_ATI, 0x714E, PCI_MATCH_ANY, PCI_MATCH_ANY,
        0x00030000, 0x00ffffff, 0
    },
    {
        PCI_VENDOR_ATI, 0x715E, PCI_MATCH_ANY, PCI_MATCH_ANY,
        0x00030000, 0x00ffffff, 0
    },
    {
        PCI_VENDOR_ATI, 0x714D, PCI_MATCH_ANY, PCI_MATCH_ANY,
        0x00030000, 0x00ffffff, 0
    },
    {
        PCI_VENDOR_ATI, 0x71C3, PCI_MATCH_ANY, PCI_MATCH_ANY,
        0x00030000, 0x00ffffff, 0
    },
    {
        PCI_VENDOR_ATI, 0x718F, PCI_MATCH_ANY, PCI_MATCH_ANY,
        0x00030000, 0x00ffffff, 0
    },
    {
        PCI_VENDOR_ATI, 0x7142, PCI_MATCH_ANY, PCI_MATCH_ANY,
        0x00030000, 0x00ffffff, 0
    },
    {
        PCI_VENDOR_ATI, 0x7180, PCI_MATCH_ANY, PCI_MATCH_ANY,
        0x00030000, 0x00ffffff, 0
    },
    {
        PCI_VENDOR_ATI, 0x7183, PCI_MATCH_ANY, PCI_MATCH_ANY,
        0x00030000, 0x00ffffff, 0
    },
    {
        PCI_VENDOR_ATI, 0x7187, PCI_MATCH_ANY, PCI_MATCH_ANY,
        0x00030000, 0x00ffffff, 0
    },
    {
        PCI_VENDOR_ATI, 0x7147, PCI_MATCH_ANY, PCI_MATCH_ANY,
        0x00030000, 0x00ffffff, 0
    },
    {
        PCI_VENDOR_ATI, 0x715F, PCI_MATCH_ANY, PCI_MATCH_ANY,
        0x00030000, 0x00ffffff, 0
    },
    {
        PCI_VENDOR_ATI, 0x719F, PCI_MATCH_ANY, PCI_MATCH_ANY,
        0x00030000, 0x00ffffff, 0
    },
    {
        PCI_VENDOR_ATI, 0x7143, PCI_MATCH_ANY, PCI_MATCH_ANY,
        0x00030000, 0x00ffffff, 0
    },
    {
        PCI_VENDOR_ATI, 0x7193, PCI_MATCH_ANY, PCI_MATCH_ANY,
        0x00030000, 0x00ffffff, 0
    },
    {
        PCI_VENDOR_ATI, 0x71CE, PCI_MATCH_ANY, PCI_MATCH_ANY,
        0x00030000, 0x00ffffff, 0
    },
    {
        PCI_VENDOR_ATI, 0x7140, PCI_MATCH_ANY, PCI_MATCH_ANY,
        0x00030000, 0x00ffffff, 0
    },
    {
        PCI_VENDOR_ATI, 0x71C0, PCI_MATCH_ANY, PCI_MATCH_ANY,
        0x00030000, 0x00ffffff, 0
    },
    {
        PCI_VENDOR_ATI, 0x71C2, PCI_MATCH_ANY, PCI_MATCH_ANY,
        0x00030000, 0x00ffffff, 0
    },
    {
        PCI_VENDOR_ATI, 0x71C6, PCI_MATCH_ANY, PCI_MATCH_ANY,
        0x00030000, 0x00ffffff, 0
    },
    {
        PCI_VENDOR_ATI, 0x7181, PCI_MATCH_ANY, PCI_MATCH_ANY,
        0x00030000, 0x00ffffff, 0
    },
    {
        PCI_VENDOR_ATI, 0x71CD, PCI_MATCH_ANY, PCI_MATCH_ANY,
        0x00030000, 0x00ffffff, 0
    },
    {
        PCI_VENDOR_ATI, 0x71C1, PCI_MATCH_ANY, PCI_MATCH_ANY,
        0x00030000, 0x00ffffff, 0
    },
    {
        PCI_VENDOR_ATI, 0x7293, PCI_MATCH_ANY, PCI_MATCH_ANY,
        0x00030000, 0x00ffffff, 0
    },
    {
        PCI_VENDOR_ATI, 0x7291, PCI_MATCH_ANY, PCI_MATCH_ANY,
        0x00030000, 0x00ffffff, 0
    },
    {
        PCI_VENDOR_ATI, 0x71C7, PCI_MATCH_ANY, PCI_MATCH_ANY,
        0x00030000, 0x00ffffff, 0
    },
    {
        PCI_VENDOR_ATI, 0x7100, PCI_MATCH_ANY, PCI_MATCH_ANY,
        0x00030000, 0x00ffffff, 0
    },
    {
        PCI_VENDOR_ATI, 0x7108, PCI_MATCH_ANY, PCI_MATCH_ANY,
        0x00030000, 0x00ffffff, 0
    },
    {
        PCI_VENDOR_ATI, 0x7109, PCI_MATCH_ANY, PCI_MATCH_ANY,
        0x00030000, 0x00ffffff, 0
    },
    {
        PCI_VENDOR_ATI, 0x710A, PCI_MATCH_ANY, PCI_MATCH_ANY,
        0x00030000, 0x00ffffff, 0
    },
    {
        PCI_VENDOR_ATI, 0x710B, PCI_MATCH_ANY, PCI_MATCH_ANY,
        0x00030000, 0x00ffffff, 0
    },
    {
        PCI_VENDOR_ATI, 0x710C, PCI_MATCH_ANY, PCI_MATCH_ANY,
        0x00030000, 0x00ffffff, 0
    },
    {
        PCI_VENDOR_ATI, 0x7243, PCI_MATCH_ANY, PCI_MATCH_ANY,
        0x00030000, 0x00ffffff, 0
    },
    {
        PCI_VENDOR_ATI, 0x7245, PCI_MATCH_ANY, PCI_MATCH_ANY,
        0x00030000, 0x00ffffff, 0
    },
    {
        PCI_VENDOR_ATI, 0x7246, PCI_MATCH_ANY, PCI_MATCH_ANY,
        0x00030000, 0x00ffffff, 0
    },
    {
        PCI_VENDOR_ATI, 0x7247, PCI_MATCH_ANY, PCI_MATCH_ANY,
        0x00030000, 0x00ffffff, 0
    },
    {
        PCI_VENDOR_ATI, 0x7248, PCI_MATCH_ANY, PCI_MATCH_ANY,
        0x00030000, 0x00ffffff, 0
    },
    {
        PCI_VENDOR_ATI, 0x7249, PCI_MATCH_ANY, PCI_MATCH_ANY,
        0x00030000, 0x00ffffff, 0
    },
    {
        PCI_VENDOR_ATI, 0x724A, PCI_MATCH_ANY, PCI_MATCH_ANY,
        0x00030000, 0x00ffffff, 0
    },
    {
        PCI_VENDOR_ATI, 0x724B, PCI_MATCH_ANY, PCI_MATCH_ANY,
        0x00030000, 0x00ffffff, 0
    },
    {
        PCI_VENDOR_ATI, 0x724C, PCI_MATCH_ANY, PCI_MATCH_ANY,
        0x00030000, 0x00ffffff, 0
    },
    {
        PCI_VENDOR_ATI, 0x724D, PCI_MATCH_ANY, PCI_MATCH_ANY,
        0x00030000, 0x00ffffff, 0
    },
    {
        PCI_VENDOR_ATI, 0x724F, PCI_MATCH_ANY, PCI_MATCH_ANY,
        0x00030000, 0x00ffffff, 0
    },
    {
        PCI_VENDOR_ATI, 0x7280, PCI_MATCH_ANY, PCI_MATCH_ANY,
        0x00030000, 0x00ffffff, 0
    },
    {
        PCI_VENDOR_ATI, 0x7240, PCI_MATCH_ANY, PCI_MATCH_ANY,
        0x00030000, 0x00ffffff, 0
    },
    {
        PCI_VENDOR_ATI, 0x7244, PCI_MATCH_ANY, PCI_MATCH_ANY,
        0x00030000, 0x00ffffff, 0
    },

    { 0, 0, 0 },
};
#endif

/* Supported chipsets.  I'm really, really glad that these are
 * separate, and the nomenclature is beyond reproach. */
SymTabRec avivo_chips[] = {
    { PCI_CHIP_R580_7248, "R580 (Radeon X1950 XTX Uber - Limited Edition)" },
    { PCI_CHIP_RV530GL_71D2, "RV530GL (ATI FireGL V3400)" },
    { PCI_CHIP_RV530GL_71DA, "RV530GL (ATI FireGL V5200)" },
    { PCI_CHIP_R520GL_7105, "R520GL (ATI FireGL V5300)" },
    { PCI_CHIP_R520GL_7104, "R520GL (ATI FireGL V7200)" },
    { PCI_CHIP_R520GL_710E, "R520GL (ATI FireGL V7300)" },
    { PCI_CHIP_R520GL_710F, "R520GL (ATI FireGL V7350)" },
    { PCI_CHIP_RV515_719B, "RV515 (ATI FireMV 2250)" },
    { PCI_CHIP_R580_724E, "R580 (ATI FireStream 2U)" },
    { PCI_CHIP_M56GL_71C4, "M56GL (ATI MOBILITY FireGL V5200)" },
    { PCI_CHIP_M56GL_71D4, "M56GL (ATI MOBILITY FireGL V5250)" },
    { PCI_CHIP_M58GL_7106, "M58GL (ATI MOBILITY FireGL V7100)" },
    { PCI_CHIP_M58GL_7103, "M58GL (ATI MOBILITY FireGL V7200)" },
    { PCI_CHIP_M52_714A, "M52 (ATI Mobility Radeon X1300)" },
    { PCI_CHIP_M52_7149, "M52 (ATI Mobility Radeon X1300)" },
    { PCI_CHIP_M52_714B, "M52 (ATI Mobility Radeon X1300)" },
    { PCI_CHIP_M52_714C, "M52 (ATI Mobility Radeon X1300)" },
    { PCI_CHIP_M52_718B, "M52 (ATI Mobility Radeon X1350)" },
    { PCI_CHIP_M52_718C, "M52 (ATI Mobility Radeon X1350)" },
    { PCI_CHIP_M52_7196, "M52 (ATI Mobility Radeon X1350)" },
    { PCI_CHIP_M54_7145, "M54 (ATI Mobility Radeon X1400)" },
    { PCI_CHIP_M54_7186, "M54 (ATI Mobility Radeon X1450)" },
    { PCI_CHIP_M54_718D, "M54 (ATI Mobility Radeon X1450)" },
    { PCI_CHIP_M56_71C5, "M56 (ATI Mobility Radeon X1600)" },
    { PCI_CHIP_M56_71D5, "M56 (ATI Mobility Radeon X1700)" },
    { PCI_CHIP_M56_71DE, "M56 (ATI Mobility Radeon X1700)" },
    { PCI_CHIP_M56_71D6, "M56 (ATI Mobility Radeon X1700 XT)" },
    { PCI_CHIP_M58_7102, "M58 (ATI Mobility Radeon X1800)" },
    { PCI_CHIP_M58_7101, "M58 (ATI Mobility Radeon X1800 XT)" },
    { PCI_CHIP_M58_7284, "M58 (ATI Mobility Radeon X1900)" },
    { PCI_CHIP_RS690_791E, "RS690 (ATI Radeon X1200 Series)" },
    { PCI_CHIP_RS690M_791F, "RS690M (ATI Radeon X1200 Series)" },
    { PCI_CHIP_R580_7288, "R580 (ATI Radeon X1950 GT)" },
    { PCI_CHIP_RS600_793F, "RS600 (ATI Radeon Xpress 1200 Series)" },
    { PCI_CHIP_RS600_7941, "RS600 (ATI Radeon Xpress 1200 Series)" },
    { PCI_CHIP_RS600M_7942, "RS600M (ATI Radeon Xpress 1200 Series)" },
    { PCI_CHIP_RV515_7146, "RV515 (Radeon X1300 / X1550 Series)" },
    { PCI_CHIP_RV515PCI_714E, "RV515PCI (Radeon X1300 Series)" },
    { PCI_CHIP_RV515_715E, "RV515 (Radeon X1300 Series)" },
    { PCI_CHIP_RV515_714D, "RV515 (Radeon X1300 Series)" },
    { PCI_CHIP_RV535_71C3, "RV535 (Radeon X1300 Series)" },
    { PCI_CHIP_RV515PCI_718F, "RV515PCI (Radeon X1300 Series)" },
    { PCI_CHIP_RV515_7142, "RV515 (Radeon X1300/X1550 Series)" },
    { PCI_CHIP_RV515_7180, "RV515 (Radeon X1300/X1550 Series)" },
    { PCI_CHIP_RV515_7183, "RV515 (Radeon X1300/X1550 Series)" },
    { PCI_CHIP_RV515_7187, "RV515 (Radeon X1300/X1550 Series)" },
    { PCI_CHIP_RV515_7147, "RV515 (Radeon X1550 64-bit)" },
    { PCI_CHIP_RV515_715F, "RV515 (Radeon X1550 64-bit)" },
    { PCI_CHIP_RV515_719F, "RV515 (Radeon X1550 64-bit)" },
    { PCI_CHIP_RV515_7143, "RV515 (Radeon X1550 Series)" },
    { PCI_CHIP_RV515_7193, "RV515 (Radeon X1550 Series)" },
    { PCI_CHIP_RV530_71CE, "RV530 (Radeon X1600 Pro / Radeon X1300 XT)" },
    { PCI_CHIP_RV515_7140, "RV515 (Radeon X1600 Series)" },
    { PCI_CHIP_RV530_71C0, "RV530 (Radeon X1600 Series)" },
    { PCI_CHIP_RV530_71C2, "RV530 (Radeon X1600 Series)" },
    { PCI_CHIP_RV530_71C6, "RV530 (Radeon X1600 Series)" },
    { PCI_CHIP_RV515_7181, "RV515 (Radeon X1600 Series)" },
    { PCI_CHIP_RV530_71CD, "RV530 (Radeon X1600 Series)" },
    { PCI_CHIP_RV535_71C1, "RV535 (Radeon X1650 Series)" },
    { PCI_CHIP_RV535_7293, "RV535 (Radeon X1650 Series)" },
    { PCI_CHIP_RV535_7291, "RV535 (Radeon X1650 Series)" },
    { PCI_CHIP_RV535_71C7, "RV535 (Radeon X1650 Series)" },
    { PCI_CHIP_R520_7100, "R520 (Radeon X1800 Series)" },
    { PCI_CHIP_R520_7108, "R520 (Radeon X1800 Series)" },
    { PCI_CHIP_R520_7109, "R520 (Radeon X1800 Series)" },
    { PCI_CHIP_R520_710A, "R520 (Radeon X1800 Series)" },
    { PCI_CHIP_R520_710B, "R520 (Radeon X1800 Series)" },
    { PCI_CHIP_R520_710C, "R520 (Radeon X1800 Series)" },
    { PCI_CHIP_R580_7243, "R580 (Radeon X1900 Series)" },
    { PCI_CHIP_R580_7245, "R580 (Radeon X1900 Series)" },
    { PCI_CHIP_R580_7246, "R580 (Radeon X1900 Series)" },
    { PCI_CHIP_R580_7247, "R580 (Radeon X1900 Series)" },
    { PCI_CHIP_R580_7248, "R580 (Radeon X1900 Series)" },
    { PCI_CHIP_R580_7249, "R580 (Radeon X1900 Series)" },
    { PCI_CHIP_R580_724A, "R580 (Radeon X1900 Series)" },
    { PCI_CHIP_R580_724B, "R580 (Radeon X1900 Series)" },
    { PCI_CHIP_R580_724C, "R580 (Radeon X1900 Series)" },
    { PCI_CHIP_R580_724D, "R580 (Radeon X1900 Series)" },
    { PCI_CHIP_R580_724F, "R580 (Radeon X1900 Series)" },
    { PCI_CHIP_R580_7280, "R580 (Radeon X1950 Series)" },
    { PCI_CHIP_R580_7240, "R580 (Radeon X1950 Series)" },
    { PCI_CHIP_R580_7244, "R580 (Radeon X1950 Series)" },
    { -1,                  NULL }
};

PciChipsets avivo_pci_chips[] = {
    { PCI_CHIP_R580_7248, PCI_CHIP_R580_7248, RES_SHARED_VGA },
    { PCI_CHIP_RV530GL_71D2, PCI_CHIP_RV530GL_71D2, RES_SHARED_VGA },
    { PCI_CHIP_RV530GL_71DA, PCI_CHIP_RV530GL_71DA, RES_SHARED_VGA },
    { PCI_CHIP_R520GL_7105, PCI_CHIP_R520GL_7105, RES_SHARED_VGA },
    { PCI_CHIP_R520GL_7104, PCI_CHIP_R520GL_7104, RES_SHARED_VGA },
    { PCI_CHIP_R520GL_710E, PCI_CHIP_R520GL_710E, RES_SHARED_VGA },
    { PCI_CHIP_R520GL_710F, PCI_CHIP_R520GL_710F, RES_SHARED_VGA },
    { PCI_CHIP_RV515_719B, PCI_CHIP_RV515_719B, RES_SHARED_VGA },
    { PCI_CHIP_R580_724E, PCI_CHIP_R580_724E, RES_SHARED_VGA },
    { PCI_CHIP_M56GL_71C4, PCI_CHIP_M56GL_71C4, RES_SHARED_VGA },
    { PCI_CHIP_M56GL_71D4, PCI_CHIP_M56GL_71D4, RES_SHARED_VGA },
    { PCI_CHIP_M58GL_7106, PCI_CHIP_M58GL_7106, RES_SHARED_VGA },
    { PCI_CHIP_M58GL_7103, PCI_CHIP_M58GL_7103, RES_SHARED_VGA },
    { PCI_CHIP_M52_714A, PCI_CHIP_M52_714A, RES_SHARED_VGA },
    { PCI_CHIP_M52_7149, PCI_CHIP_M52_7149, RES_SHARED_VGA },
    { PCI_CHIP_M52_714B, PCI_CHIP_M52_714B, RES_SHARED_VGA },
    { PCI_CHIP_M52_714C, PCI_CHIP_M52_714C, RES_SHARED_VGA },
    { PCI_CHIP_M52_718B, PCI_CHIP_M52_718B, RES_SHARED_VGA },
    { PCI_CHIP_M52_718C, PCI_CHIP_M52_718C, RES_SHARED_VGA },
    { PCI_CHIP_M52_7196, PCI_CHIP_M52_7196, RES_SHARED_VGA },
    { PCI_CHIP_M54_7145, PCI_CHIP_M54_7145, RES_SHARED_VGA },
    { PCI_CHIP_M54_7186, PCI_CHIP_M54_7186, RES_SHARED_VGA },
    { PCI_CHIP_M54_718D, PCI_CHIP_M54_718D, RES_SHARED_VGA },
    { PCI_CHIP_M56_71C5, PCI_CHIP_M56_71C5, RES_SHARED_VGA },
    { PCI_CHIP_M56_71D5, PCI_CHIP_M56_71D5, RES_SHARED_VGA },
    { PCI_CHIP_M56_71DE, PCI_CHIP_M56_71DE, RES_SHARED_VGA },
    { PCI_CHIP_M56_71D6, PCI_CHIP_M56_71D6, RES_SHARED_VGA },
    { PCI_CHIP_M58_7102, PCI_CHIP_M58_7102, RES_SHARED_VGA },
    { PCI_CHIP_M58_7101, PCI_CHIP_M58_7101, RES_SHARED_VGA },
    { PCI_CHIP_M58_7284, PCI_CHIP_M58_7284, RES_SHARED_VGA },
    { PCI_CHIP_RS690_791E, PCI_CHIP_RS690_791E, RES_SHARED_VGA },
    { PCI_CHIP_RS690M_791F, PCI_CHIP_RS690M_791F, RES_SHARED_VGA },
    { PCI_CHIP_R580_7288, PCI_CHIP_R580_7288, RES_SHARED_VGA },
    { PCI_CHIP_RS600_793F, PCI_CHIP_RS600_793F, RES_SHARED_VGA },
    { PCI_CHIP_RS600_7941, PCI_CHIP_RS600_7941, RES_SHARED_VGA },
    { PCI_CHIP_RS600M_7942, PCI_CHIP_RS600M_7942, RES_SHARED_VGA },
    { PCI_CHIP_RV515_7146, PCI_CHIP_RV515_7146, RES_SHARED_VGA },
    { PCI_CHIP_RV515PCI_714E, PCI_CHIP_RV515PCI_714E, RES_SHARED_VGA },
    { PCI_CHIP_RV515_715E, PCI_CHIP_RV515_715E, RES_SHARED_VGA },
    { PCI_CHIP_RV515_714D, PCI_CHIP_RV515_714D, RES_SHARED_VGA },
    { PCI_CHIP_RV535_71C3, PCI_CHIP_RV535_71C3, RES_SHARED_VGA },
    { PCI_CHIP_RV515PCI_718F, PCI_CHIP_RV515PCI_718F, RES_SHARED_VGA },
    { PCI_CHIP_RV515_7142, PCI_CHIP_RV515_7142, RES_SHARED_VGA },
    { PCI_CHIP_RV515_7180, PCI_CHIP_RV515_7180, RES_SHARED_VGA },
    { PCI_CHIP_RV515_7183, PCI_CHIP_RV515_7183, RES_SHARED_VGA },
    { PCI_CHIP_RV515_7187, PCI_CHIP_RV515_7187, RES_SHARED_VGA },
    { PCI_CHIP_RV515_7147, PCI_CHIP_RV515_7147, RES_SHARED_VGA },
    { PCI_CHIP_RV515_715F, PCI_CHIP_RV515_715F, RES_SHARED_VGA },
    { PCI_CHIP_RV515_719F, PCI_CHIP_RV515_719F, RES_SHARED_VGA },
    { PCI_CHIP_RV515_7143, PCI_CHIP_RV515_7143, RES_SHARED_VGA },
    { PCI_CHIP_RV515_7193, PCI_CHIP_RV515_7193, RES_SHARED_VGA },
    { PCI_CHIP_RV530_71CE, PCI_CHIP_RV530_71CE, RES_SHARED_VGA },
    { PCI_CHIP_RV515_7140, PCI_CHIP_RV515_7140, RES_SHARED_VGA },
    { PCI_CHIP_RV530_71C0, PCI_CHIP_RV530_71C0, RES_SHARED_VGA },
    { PCI_CHIP_RV530_71C2, PCI_CHIP_RV530_71C2, RES_SHARED_VGA },
    { PCI_CHIP_RV530_71C6, PCI_CHIP_RV530_71C6, RES_SHARED_VGA },
    { PCI_CHIP_RV515_7181, PCI_CHIP_RV515_7181, RES_SHARED_VGA },
    { PCI_CHIP_RV530_71CD, PCI_CHIP_RV530_71CD, RES_SHARED_VGA },
    { PCI_CHIP_RV535_71C1, PCI_CHIP_RV535_71C1, RES_SHARED_VGA },
    { PCI_CHIP_RV535_7293, PCI_CHIP_RV535_7293, RES_SHARED_VGA },
    { PCI_CHIP_RV535_7291, PCI_CHIP_RV535_7291, RES_SHARED_VGA },
    { PCI_CHIP_RV535_71C7, PCI_CHIP_RV535_71C7, RES_SHARED_VGA },
    { PCI_CHIP_R520_7100, PCI_CHIP_R520_7100, RES_SHARED_VGA },
    { PCI_CHIP_R520_7108, PCI_CHIP_R520_7108, RES_SHARED_VGA },
    { PCI_CHIP_R520_7109, PCI_CHIP_R520_7109, RES_SHARED_VGA },
    { PCI_CHIP_R520_710A, PCI_CHIP_R520_710A, RES_SHARED_VGA },
    { PCI_CHIP_R520_710B, PCI_CHIP_R520_710B, RES_SHARED_VGA },
    { PCI_CHIP_R520_710C, PCI_CHIP_R520_710C, RES_SHARED_VGA },
    { PCI_CHIP_R580_7243, PCI_CHIP_R580_7243, RES_SHARED_VGA },
    { PCI_CHIP_R580_7245, PCI_CHIP_R580_7245, RES_SHARED_VGA },
    { PCI_CHIP_R580_7246, PCI_CHIP_R580_7246, RES_SHARED_VGA },
    { PCI_CHIP_R580_7247, PCI_CHIP_R580_7247, RES_SHARED_VGA },
    { PCI_CHIP_R580_7248, PCI_CHIP_R580_7248, RES_SHARED_VGA },
    { PCI_CHIP_R580_7249, PCI_CHIP_R580_7249, RES_SHARED_VGA },
    { PCI_CHIP_R580_724A, PCI_CHIP_R580_724A, RES_SHARED_VGA },
    { PCI_CHIP_R580_724B, PCI_CHIP_R580_724B, RES_SHARED_VGA },
    { PCI_CHIP_R580_724C, PCI_CHIP_R580_724C, RES_SHARED_VGA },
    { PCI_CHIP_R580_724D, PCI_CHIP_R580_724D, RES_SHARED_VGA },
    { PCI_CHIP_R580_724F, PCI_CHIP_R580_724F, RES_SHARED_VGA },
    { PCI_CHIP_R580_7280, PCI_CHIP_R580_7280, RES_SHARED_VGA },
    { PCI_CHIP_R580_7240, PCI_CHIP_R580_7240, RES_SHARED_VGA },
    { PCI_CHIP_R580_7244, PCI_CHIP_R580_7244, RES_SHARED_VGA },
    { -1,                  -1,                  RES_UNDEFINED }
};

struct avivo_chipset_family {
    unsigned int pci_id;
    enum avivo_chip_type family;
};

static struct avivo_chipset_family chipset_family[] = {
    { PCI_CHIP_R580_7248, CHIP_FAMILY_R580 },
    { PCI_CHIP_RV530GL_71D2, CHIP_FAMILY_RV530GL },
    { PCI_CHIP_RV530GL_71DA, CHIP_FAMILY_RV530GL },
    { PCI_CHIP_R520GL_7105, CHIP_FAMILY_R520GL },
    { PCI_CHIP_R520GL_7104, CHIP_FAMILY_R520GL },
    { PCI_CHIP_R520GL_710E, CHIP_FAMILY_R520GL },
    { PCI_CHIP_R520GL_710F, CHIP_FAMILY_R520GL },
    { PCI_CHIP_RV515_719B, CHIP_FAMILY_RV515 },
    { PCI_CHIP_R580_724E, CHIP_FAMILY_R580 },
    { PCI_CHIP_M56GL_71C4, CHIP_FAMILY_M56GL },
    { PCI_CHIP_M56GL_71D4, CHIP_FAMILY_M56GL },
    { PCI_CHIP_M58GL_7106, CHIP_FAMILY_M58GL },
    { PCI_CHIP_M58GL_7103, CHIP_FAMILY_M58GL },
    { PCI_CHIP_M52_714A, CHIP_FAMILY_M52 },
    { PCI_CHIP_M52_7149, CHIP_FAMILY_M52 },
    { PCI_CHIP_M52_714B, CHIP_FAMILY_M52 },
    { PCI_CHIP_M52_714C, CHIP_FAMILY_M52 },
    { PCI_CHIP_M52_718B, CHIP_FAMILY_M52 },
    { PCI_CHIP_M52_718C, CHIP_FAMILY_M52 },
    { PCI_CHIP_M52_7196, CHIP_FAMILY_M52 },
    { PCI_CHIP_M54_7145, CHIP_FAMILY_M54 },
    { PCI_CHIP_M54_7186, CHIP_FAMILY_M54 },
    { PCI_CHIP_M54_718D, CHIP_FAMILY_M54 },
    { PCI_CHIP_M56_71C5, CHIP_FAMILY_M56 },
    { PCI_CHIP_M56_71D5, CHIP_FAMILY_M56 },
    { PCI_CHIP_M56_71DE, CHIP_FAMILY_M56 },
    { PCI_CHIP_M56_71D6, CHIP_FAMILY_M56 },
    { PCI_CHIP_M58_7102, CHIP_FAMILY_M58 },
    { PCI_CHIP_M58_7101, CHIP_FAMILY_M58 },
    { PCI_CHIP_M58_7284, CHIP_FAMILY_M58 },
    { PCI_CHIP_RS690_791E, CHIP_FAMILY_RS690 },
    { PCI_CHIP_RS690M_791F, CHIP_FAMILY_RS690M },
    { PCI_CHIP_R580_7288, CHIP_FAMILY_R580 },
    { PCI_CHIP_RS600_793F, CHIP_FAMILY_RS600 },
    { PCI_CHIP_RS600_7941, CHIP_FAMILY_RS600 },
    { PCI_CHIP_RS600M_7942, CHIP_FAMILY_RS600M },
    { PCI_CHIP_RV515_7146, CHIP_FAMILY_RV515 },
    { PCI_CHIP_RV515PCI_714E, CHIP_FAMILY_RV515PCI },
    { PCI_CHIP_RV515_715E, CHIP_FAMILY_RV515 },
    { PCI_CHIP_RV515_714D, CHIP_FAMILY_RV515 },
    { PCI_CHIP_RV535_71C3, CHIP_FAMILY_RV535 },
    { PCI_CHIP_RV515PCI_718F, CHIP_FAMILY_RV515PCI },
    { PCI_CHIP_RV515_7142, CHIP_FAMILY_RV515 },
    { PCI_CHIP_RV515_7180, CHIP_FAMILY_RV515 },
    { PCI_CHIP_RV515_7183, CHIP_FAMILY_RV515 },
    { PCI_CHIP_RV515_7187, CHIP_FAMILY_RV515 },
    { PCI_CHIP_RV515_7147, CHIP_FAMILY_RV515 },
    { PCI_CHIP_RV515_715F, CHIP_FAMILY_RV515 },
    { PCI_CHIP_RV515_719F, CHIP_FAMILY_RV515 },
    { PCI_CHIP_RV515_7143, CHIP_FAMILY_RV515 },
    { PCI_CHIP_RV515_7193, CHIP_FAMILY_RV515 },
    { PCI_CHIP_RV530_71CE, CHIP_FAMILY_RV530 },
    { PCI_CHIP_RV515_7140, CHIP_FAMILY_RV515 },
    { PCI_CHIP_RV530_71C0, CHIP_FAMILY_RV530 },
    { PCI_CHIP_RV530_71C2, CHIP_FAMILY_RV530 },
    { PCI_CHIP_RV530_71C6, CHIP_FAMILY_RV530 },
    { PCI_CHIP_RV515_7181, CHIP_FAMILY_RV515 },
    { PCI_CHIP_RV530_71CD, CHIP_FAMILY_RV530 },
    { PCI_CHIP_RV535_71C1, CHIP_FAMILY_RV535 },
    { PCI_CHIP_RV535_7293, CHIP_FAMILY_RV535 },
    { PCI_CHIP_RV535_7291, CHIP_FAMILY_RV535 },
    { PCI_CHIP_RV535_71C7, CHIP_FAMILY_RV535 },
    { PCI_CHIP_R520_7100, CHIP_FAMILY_R520 },
    { PCI_CHIP_R520_7108, CHIP_FAMILY_R520 },
    { PCI_CHIP_R520_7109, CHIP_FAMILY_R520 },
    { PCI_CHIP_R520_710A, CHIP_FAMILY_R520 },
    { PCI_CHIP_R520_710B, CHIP_FAMILY_R520 },
    { PCI_CHIP_R520_710C, CHIP_FAMILY_R520 },
    { PCI_CHIP_R580_7243, CHIP_FAMILY_R580 },
    { PCI_CHIP_R580_7245, CHIP_FAMILY_R580 },
    { PCI_CHIP_R580_7246, CHIP_FAMILY_R580 },
    { PCI_CHIP_R580_7247, CHIP_FAMILY_R580 },
    { PCI_CHIP_R580_7248, CHIP_FAMILY_R580 },
    { PCI_CHIP_R580_7249, CHIP_FAMILY_R580 },
    { PCI_CHIP_R580_724A, CHIP_FAMILY_R580 },
    { PCI_CHIP_R580_724B, CHIP_FAMILY_R580 },
    { PCI_CHIP_R580_724C, CHIP_FAMILY_R580 },
    { PCI_CHIP_R580_724D, CHIP_FAMILY_R580 },
    { PCI_CHIP_R580_724F, CHIP_FAMILY_R580 },
    { PCI_CHIP_R580_7280, CHIP_FAMILY_R580 },
    { PCI_CHIP_R580_7240, CHIP_FAMILY_R580 },
    { PCI_CHIP_R580_7244, CHIP_FAMILY_R580 },
};

void
avivo_get_chipset(struct avivo_info *avivo)
{
    int i;

    for (i = 0; i < sizeof(chipset_family) / sizeof(chipset_family[0]); i++) {
#ifdef PCIACCESS
        if (chipset_family[i].pci_id == avivo->pci_info->device_id) {
#else
        if (chipset_family[i].pci_id == avivo->pci_info->chipType) {
#endif /*PCIACCESS*/
            avivo->chipset = chipset_family[i].family;
            return;
        }
    }
#ifdef PCIACCESS
    FatalError("Unknown chipset for %x!\n", avivo->pci_info->device_id);
#else
    FatalError("Unknown chipset for %x!\n", avivo->pci_info->device);
#endif /*PCIACCESS*/
}
