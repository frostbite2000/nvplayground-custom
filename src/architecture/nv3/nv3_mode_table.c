/*
 * Filename: nv3_mode_table.c
 * Purpose: Mode table for the NV3/NV3T GPU
 * Date: 2025-04-25
 */

#include "architecture/nv3/nv3_state.h"
#include "architecture/nv3/nv3_ref.h"

// Mode table for common resolutions supported by the NV3/NV3T
nv3_mode_entry_t mode_table[] =
{
    // 640x480x8 @ 60Hz
    {
        .width = 640,
        .height = 480,
        .bpp = 8,
        .refresh_rate = 60,
        
        .horizontal_total = 800,
        .horizontal_display_end = 640,
        .horizontal_blank_start = 640,
        .horizontal_blank_end = 800,
        .horizontal_retrace_start = 656,
        .horizontal_retrace_end = 752,
        
        .vertical_total = 525,
        .vertical_display_end = 480,
        .vertical_blank_start = 480,
        .vertical_blank_end = 525,
        .vertical_retrace_start = 490,
        .vertical_retrace_end = 492,
        
        .pixel_clock = 25175,
        .memory_clock = 0x0EC40E,  // 100MHz default
    },
    
    // 640x480x16 @ 60Hz
    {
        .width = 640,
        .height = 480,
        .bpp = 16,
        .refresh_rate = 60,
        
        .horizontal_total = 800,
        .horizontal_display_end = 640,
        .horizontal_blank_start = 640,
        .horizontal_blank_end = 800,
        .horizontal_retrace_start = 656,
        .horizontal_retrace_end = 752,
        
        .vertical_total = 525,
        .vertical_display_end = 480,
        .vertical_blank_start = 480,
        .vertical_blank_end = 525,
        .vertical_retrace_start = 490,
        .vertical_retrace_end = 492,
        
        .pixel_clock = 25175,
        .memory_clock = 0x0EC40E,  // 100MHz default
    },
    
    // 640x480x32 @ 60Hz
    {
        .width = 640,
        .height = 480,
        .bpp = 32,
        .refresh_rate = 60,
        
        .horizontal_total = 800,
        .horizontal_display_end = 640,
        .horizontal_blank_start = 640,
        .horizontal_blank_end = 800,
        .horizontal_retrace_start = 656,
        .horizontal_retrace_end = 752,
        
        .vertical_total = 525,
        .vertical_display_end = 480,
        .vertical_blank_start = 480,
        .vertical_blank_end = 525,
        .vertical_retrace_start = 490,
        .vertical_retrace_end = 492,
        
        .pixel_clock = 25175,
        .memory_clock = 0x0EC40E,  // 100MHz default
    },
    
    // 800x600x8 @ 60Hz
    {
        .width = 800,
        .height = 600,
        .bpp = 8,
        .refresh_rate = 60,
        
        .horizontal_total = 1056,
        .horizontal_display_end = 800,
        .horizontal_blank_start = 800,
        .horizontal_blank_end = 1056,
        .horizontal_retrace_start = 840,
        .horizontal_retrace_end = 968,
        
        .vertical_total = 628,
        .vertical_display_end = 600,
        .vertical_blank_start = 600,
        .vertical_blank_end = 628,
        .vertical_retrace_start = 601,
        .vertical_retrace_end = 605,
        
        .pixel_clock = 40000,
        .memory_clock = 0x0EC40E,  // 100MHz default
    },
    
    // 800x600x16 @ 60Hz
    {
        .width = 800,
        .height = 600,
        .bpp = 16,
        .refresh_rate = 60,
        
        .horizontal_total = 1056,
        .horizontal_display_end = 800,
        .horizontal_blank_start = 800,
        .horizontal_blank_end = 1056,
        .horizontal_retrace_start = 840,
        .horizontal_retrace_end = 968,
        
        .vertical_total = 628,
        .vertical_display_end = 600,
        .vertical_blank_start = 600,
        .vertical_blank_end = 628,
        .vertical_retrace_start = 601,
        .vertical_retrace_end = 605,
        
        .pixel_clock = 40000,
        .memory_clock = 0x0EC40E,  // 100MHz default
    },
    
    // 800x600x32 @ 60Hz
    {
        .width = 800,
        .height = 600,
        .bpp = 32,
        .refresh_rate = 60,
        
        .horizontal_total = 1056,
        .horizontal_display_end = 800,
        .horizontal_blank_start = 800,
        .horizontal_blank_end = 1056,
        .horizontal_retrace_start = 840,
        .horizontal_retrace_end = 968,
        
        .vertical_total = 628,
        .vertical_display_end = 600,
        .vertical_blank_start = 600,
        .vertical_blank_end = 628,
        .vertical_retrace_start = 601,
        .vertical_retrace_end = 605,
        
        .pixel_clock = 40000,
        .memory_clock = 0x0EC40E,  // 100MHz default
    },
    
    // 1024x768x8 @ 60Hz
    {
        .width = 1024,
        .height = 768,
        .bpp = 8,
        .refresh_rate = 60,
        
        .horizontal_total = 1344,
        .horizontal_display_end = 1024,
        .horizontal_blank_start = 1024,
        .horizontal_blank_end = 1344,
        .horizontal_retrace_start = 1048,
        .horizontal_retrace_end = 1184,
        
        .vertical_total = 806,
        .vertical_display_end = 768,
        .vertical_blank_start = 768,
        .vertical_blank_end = 806,
        .vertical_retrace_start = 771,
        .vertical_retrace_end = 777,
        
        .pixel_clock = 65000,
        .memory_clock = 0x0EC40E,  // 100MHz default
    },
    
    // 1024x768x16 @ 60Hz
    {
        .width = 1024,
        .height = 768,
        .bpp = 16,
        .refresh_rate = 60,
        
        .horizontal_total = 1344,
        .horizontal_display_end = 1024,
        .horizontal_blank_start = 1024,
        .horizontal_blank_end = 1344,
        .horizontal_retrace_start = 1048,
        .horizontal_retrace_end = 1184,
        
        .vertical_total = 806,
        .vertical_display_end = 768,
        .vertical_blank_start = 768,
        .vertical_blank_end = 806,
        .vertical_retrace_start = 771,
        .vertical_retrace_end = 777,
        
        .pixel_clock = 65000,
        .memory_clock = 0x0EC40E,  // 100MHz default
    },
    
    // 1024x768x32 @ 60Hz
    {
        .width = 1024,
        .height = 768,
        .bpp = 32,
        .refresh_rate = 60,
        
        .horizontal_total = 1344,
        .horizontal_display_end = 1024,
        .horizontal_blank_start = 1024,
        .horizontal_blank_end = 1344,
        .horizontal_retrace_start = 1048,
        .horizontal_retrace_end = 1184,
        
        .vertical_total = 806,
        .vertical_display_end = 768,
        .vertical_blank_start = 768,
        .vertical_blank_end = 806,
        .vertical_retrace_start = 771,
        .vertical_retrace_end = 777,
        
        .pixel_clock = 65000,
        .memory_clock = 0x0EC40E,  // 100MHz default
    },
    
    // 1280x1024x8 @ 60Hz
    {
        .width = 1280,
        .height = 1024,
        .bpp = 8,
        .refresh_rate = 60,
        
        .horizontal_total = 1688,
        .horizontal_display_end = 1280,
        .horizontal_blank_start = 1280,
        .horizontal_blank_end = 1688,
        .horizontal_retrace_start = 1328,
        .horizontal_retrace_end = 1440,
        
        .vertical_total = 1066,
        .vertical_display_end = 1024,
        .vertical_blank_start = 1024,
        .vertical_blank_end = 1066,
        .vertical_retrace_start = 1025,
        .vertical_retrace_end = 1028,
        
        .pixel_clock = 108000,
        .memory_clock = 0x0EC40E,  // 100MHz default
    },
    
    // 1280x1024x16 @ 60Hz
    {
        .width = 1280,
        .height = 1024,
        .bpp = 16,
        .refresh_rate = 60,
        
        .horizontal_total = 1688,
        .horizontal_display_end = 1280,
        .horizontal_blank_start = 1280,
        .horizontal_blank_end = 1688,
        .horizontal_retrace_start = 1328,
        .horizontal_retrace_end = 1440,
        
        .vertical_total = 1066,
        .vertical_display_end = 1024,
        .vertical_blank_start = 1024,
        .vertical_blank_end = 1066,
        .vertical_retrace_start = 1025,
        .vertical_retrace_end = 1028,
        
        .pixel_clock = 108000,
        .memory_clock = 0x0EC40E,  // 100MHz default
    },
    
    // 1280x1024x32 @ 60Hz
    {
        .width = 1280,
        .height = 1024,
        .bpp = 32,
        .refresh_rate = 60,
        
        .horizontal_total = 1688,
        .horizontal_display_end = 1280,
        .horizontal_blank_start = 1280,
        .horizontal_blank_end = 1688,
        .horizontal_retrace_start = 1328,
        .horizontal_retrace_end = 1440,
        
        .vertical_total = 1066,
        .vertical_display_end = 1024,
        .vertical_blank_start = 1024,
        .vertical_blank_end = 1066,
        .vertical_retrace_start = 1025,
        .vertical_retrace_end = 1028,
        
        .pixel_clock = 108000,
        .memory_clock = 0x0EC40E,  // 100MHz default
    }
};

// Size of the mode table (number of entries)
const int mode_table_size = sizeof(mode_table) / sizeof(nv3_mode_entry_t);