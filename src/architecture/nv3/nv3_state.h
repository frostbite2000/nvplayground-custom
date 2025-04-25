#pragma once

#include <stdint.h>

// Mode table entry structure
typedef struct nv3_mode_entry_s {
    uint16_t width;           // Horizontal resolution
    uint16_t height;          // Vertical resolution
    uint8_t bpp;              // Bits per pixel (8, 16, 32)
    uint8_t refresh_rate;     // Refresh rate in Hz
    
    // Timing information
    uint32_t horizontal_total;        // Total horizontal pixels
    uint32_t horizontal_display_end;  // Horizontal display end
    uint32_t horizontal_blank_start;  // Horizontal blank start
    uint32_t horizontal_blank_end;    // Horizontal blank end
    uint32_t horizontal_retrace_start; // Horizontal retrace start
    uint32_t horizontal_retrace_end;  // Horizontal retrace end
    
    uint32_t vertical_total;         // Total vertical lines
    uint32_t vertical_display_end;   // Vertical display end
    uint32_t vertical_blank_start;   // Vertical blank start
    uint32_t vertical_blank_end;     // Vertical blank end
    uint32_t vertical_retrace_start; // Vertical retrace start
    uint32_t vertical_retrace_end;   // Vertical retrace end
    
    // Clock information
    uint32_t pixel_clock;      // Pixel clock in kHz
    uint32_t memory_clock;     // Memory clock setting
} nv3_mode_entry_t;

// NV3 GPU state structure
typedef struct nv3_state_s {
    // GPU configuration
    uint32_t revision;          // NV3 chip revision (A00, B00, C00)
    uint32_t vram_size;         // VRAM size in bytes
    uint32_t vram_width;        // VRAM bus width (64 or 128 bit)
    uint32_t crystal_freq;      // Crystal frequency (13.5 MHz or 14.31818 MHz)
    
    // Current display mode
    nv3_mode_entry_t current_mode;
    
    // Timing registers
    uint32_t vpll;              // Vertical PLL setting
    uint32_t mpll;              // Memory PLL setting
    
    // Subsystem status
    uint32_t enabled_subsystems; // Subsystems that are enabled
} nv3_state_t;

// External declarations for mode table
extern nv3_mode_entry_t mode_table[];
extern const int mode_table_size;

// Mode indices for common resolutions
#define NV3_MODE_640x480x8    0
#define NV3_MODE_640x480x16   1
#define NV3_MODE_640x480x32   2
#define NV3_MODE_800x600x8    3
#define NV3_MODE_800x600x16   4
#define NV3_MODE_800x600x32   5
#define NV3_MODE_1024x768x8   6
#define NV3_MODE_1024x768x16  7
#define NV3_MODE_1024x768x32  8
#define NV3_MODE_1280x1024x8  9
#define NV3_MODE_1280x1024x16 10
#define NV3_MODE_1280x1024x32 11