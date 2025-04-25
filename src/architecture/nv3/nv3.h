#pragma once

#include <stdbool.h>
#include <stdint.h>
#include "architecture/nv3/nv3_state.h"

// NV3 initialization function
bool nv3_init(void);
bool nv3_init_test_overclock(void);

// Test overclock time in seconds
#define NV3_TEST_OVERCLOCK_TIME_BETWEEN_RECLOCKS 5

// Default clock settings
#define NV3_TEST_OVERCLOCK_BASE_13500 0x01A30B
#define NV3_TEST_OVERCLOCK_BASE_14318 0x01C40E

// Memory sizes for NV3
#define NV3_VRAM_SIZE_1MB              0x100000 // 1MB (commented out in drivers)
#define NV3_VRAM_SIZE_2MB              0x200000 // 2MB
#define NV3_VRAM_SIZE_4MB              0x400000 // 4MB
#define NV3_VRAM_SIZE_8MB              0x800000 // 8MB (NV3T only)