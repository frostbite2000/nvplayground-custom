#pragma once

#include <stdbool.h>
#include <stdint.h>
#include "nvplayground.h"

// Function prototypes for device initialization
typedef bool (*init_function_t)(void);
typedef bool (*shutdown_function_t)(void);
typedef bool (*tick_function_t)(void);
typedef bool (*render_function_t)(void);

// Device info structure
typedef struct nv_device_info_s {
    uint32_t device_id;
    uint32_t vendor_id;
    char name[64];
    init_function_t init_function;
    shutdown_function_t shutdown_function;
    tick_function_t tick_function;
    render_function_t render_function;
} nv_device_info_t;

// Current device state
typedef struct nv_device_s {
    uint32_t bus_number;
    uint32_t function_number;
    uint32_t nv_pmc_boot_0;
    uint32_t nv_pfb_boot_0;
    uint32_t vram_amount;
    uint32_t straps;
    void *mmio_mapping;
    void *vram_mapping;
    void *ramin_mapping;
    nv_device_info_t device_info;
} nv_device_t;

// External globals
extern nv_device_info_t supported_devices[];
extern nv_device_t current_device;

// Function prototypes
bool nv_detect(void);
uint32_t nv_mmio_read32(uint32_t addr);
void nv_mmio_write32(uint32_t addr, uint32_t value);
bool init_mmio_mappings(uint32_t bar0_base, uint32_t bar1_base);
void cleanup_mmio_mappings(void);