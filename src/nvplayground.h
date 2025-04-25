#pragma once

/* 
    NVPlayground
    Copyright © 2025 starfrost
    Linux port by frostbite2000 (2025-04-25 06:10:05)

    Raw GPU programming for early Nvidia GPUs
*/

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#define APP_NAME "NVPlayground"
#define APP_DESCRIPTION "Raw GPU programming for early Nvidia GPUs\n"
#define APP_VERSION "0.1-linux"
#define APP_COMPILE_TIME __DATE__ " " __TIME__
#define APP_PORT_AUTHOR "frostbite2000"
#define APP_PORT_DATE "2025-04-25"

#define APP_SIGNON_STRING APP_NAME " Version " APP_VERSION " (" APP_COMPILE_TIME ")\n" \
                          APP_DESCRIPTION \
                          "Linux port by " APP_PORT_AUTHOR " (" APP_PORT_DATE ")\n"

// PCI configuration offset definitions
#define PCI_CFG_OFFSET_BAR0 0x10
#define PCI_CFG_OFFSET_BAR1 0x14

// PCI vendor IDs
#define PCI_VENDOR_NV      0x10DE  // NVIDIA
#define PCI_VENDOR_SGS     0x104A  // SGS Thomson (for some early NV1)
#define PCI_VENDOR_SGS_NV  0x12D2  // SGS Thomson / NVIDIA Combined

// PCI device IDs
#define PCI_DEVICE_NV1_NV      0x0008  // NV1 (VRAM version)
#define PCI_DEVICE_NV1_STG     0x0009  // NV1 (SGS STG-2000 DRAM version)
#define PCI_DEVICE_NV2         0x0010  // NV2 (never released)
#define PCI_DEVICE_NV3         0x0018  // NV3 (RIVA 128)
#define PCI_DEVICE_NV3T_ACPI   0x0019  // NV3T (RIVA 128ZX) with ACPI
#define PCI_DEVICE_NV4         0x0020  // NV4 (RIVA TNT)
#define PCI_DEVICE_NV5         0x0028  // NV5 (RIVA TNT2/TNT2 Pro)
#define PCI_DEVICE_NV5_ULTRA   0x0029  // NV5 (RIVA TNT2 Ultra)
#define PCI_DEVICE_NV5_CRAP    0x002C  // NV5 (RIVA TNT2 Vanta)
#define PCI_DEVICE_NV6         0x002D  // NV6 (RIVA TNT2 M64)
#define PCI_DEVICE_NV10        0x0100  // NV10 (GeForce 256 SDR)
#define PCI_DEVICE_NV10_DDR    0x0101  // NV10 (GeForce 256 DDR)
#define PCI_DEVICE_NV10_QUADRO 0x0103  // NV10GL (Quadro)