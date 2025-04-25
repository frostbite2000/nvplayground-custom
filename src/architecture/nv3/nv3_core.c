//
// Filename: nv3_core.c
// Purpose: NV3/NV3T (Riva 128/128ZX) core functions (bringup, shutdown, mainloop)
//
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include "architecture/nv3/nv3_ref.h"
#include "core/nvcore.h"
#include "architecture/nv3/nv3.h"

// Test overclock time in seconds
#define NV3_TEST_OVERCLOCK_TIME_BETWEEN_RECLOCKS 5

// Default clock settings
#define NV3_TEST_OVERCLOCK_BASE_13500 0x01A30B
#define NV3_TEST_OVERCLOCK_BASE_14318 0x01C40E

/* TEMPORARY test function to test certain hardcoded overclocks */
bool nv3_init_test_overclock(void)
{
    /* print out some helpful messages */
    printf("Basic clockspeed test (text mode: best case scenario)\n");
    printf("The GPU will try to run for %d seconds at each clock setting, gradually going from an underclock to an overclock.\n", 
           NV3_TEST_OVERCLOCK_TIME_BETWEEN_RECLOCKS);
    printf("Note: Some NVIDIA RIVA 128 ZX cards manufactured by TSMC run at 90Mhz and will have less overclocking potential!\n");

    /* read the straps to find our base clock value */
    uint32_t straps = nv_mmio_read32(NV3_PSTRAPS);

    /* 
        there are two possible clock bases here: 13.5 and 14.318 Mhz 
        we need two different values for our start menu
    */
    float clock_base = 13500000.0f;
    bool is_14318mhz_clock = false;     

    if ((straps >> NV3_PSTRAPS_CRYSTAL) & 0x01) {
        clock_base = 14318180.0f;
        is_14318mhz_clock = true; 
    }
        
    /* We vary the n-parameter of the MCLK to fine-tune the GPU clock speed */
    uint32_t clock_n_start = 0xA3; 
    uint32_t clock_m = 0x0B, clock_p = 0x01;

    if (is_14318mhz_clock) {
        clock_m = 0x0E;
        // base clock_n is 0xC4, so the gpu will be biased more towards underclocking
        clock_n_start = 0xC4;
    }

    for (int32_t clock_n = clock_n_start; clock_n <= 0xFF; clock_n++) {
        time_t start_time = time(NULL);

        uint32_t final_clock = (clock_p << 16) 
            | (clock_n << 8)
            | clock_m;

        // Calculate approximate MHz
        double megahertz = (clock_base * clock_n) / (clock_m * (1 << clock_p)) / 1000000.0f;

        printf("Trying MCLK = %.2f Mhz (NV_PRAMDAC_MPLL_COEFF = 0x%08X)...\n", megahertz, final_clock);

        nv_mmio_write32(NV3_PRAMDAC_CLOCK_MEMORY, final_clock);

        // Sleep for the specified interval
        sleep(NV3_TEST_OVERCLOCK_TIME_BETWEEN_RECLOCKS);
    }

    printf("We survived. Returning to 100Mhz...\n");
    
    /* restore original clock */
    if (is_14318mhz_clock)
        nv_mmio_write32(NV3_PRAMDAC_CLOCK_MEMORY, NV3_TEST_OVERCLOCK_BASE_14318);
    else
        nv_mmio_write32(NV3_PRAMDAC_CLOCK_MEMORY, NV3_TEST_OVERCLOCK_BASE_13500);

    return true; 
}

bool nv3_init(void)
{
    // Get BAR addresses from PCI config
    uint32_t bar0_base = pci_read_config_32(current_device.bus_number, current_device.function_number, PCI_CFG_OFFSET_BAR0);
    uint32_t bar1_base = pci_read_config_32(current_device.bus_number, current_device.function_number, PCI_CFG_OFFSET_BAR1);

    /* According to the datasheet only the top 8 bits matter */
    bar0_base &= 0xFF000000;
    bar1_base &= 0xFF000000;

    printf("NV3 - PCI BAR0 0x%08X\n", bar0_base);
    printf("NV3 - PCI BAR1 0x%08X\n", bar1_base);
    
    // Initialize memory mappings
    if (!init_mmio_mappings(bar0_base, bar1_base)) {
        printf("Failed to initialize MMIO mappings\n");
        return false;
    }

    current_device.nv_pmc_boot_0 = nv_mmio_read32(NV3_PMC_BOOT);
    current_device.nv_pfb_boot_0 = nv_mmio_read32(NV3_PFB_BOOT);

    printf("I'm a Riva 128! Information: \n");
    printf("NV_PMC_BOOT_0           = 0x%08X\n", current_device.nv_pmc_boot_0);
    printf("NV_PFB_BOOT_0           = 0x%08X\n", current_device.nv_pfb_boot_0);

    /* Determine the amount of Video RAM */
    uint32_t ram_amount_value = (current_device.nv_pfb_boot_0 >> NV3_PFB_BOOT_RAM_AMOUNT) & 0x03;
    bool ram_extension_8mb = (current_device.nv_pfb_boot_0 >> NV3_PFB_BOOT_RAM_EXTENSION) & 0x01;
    
    /* Read in the amount of video memory from the NV_PFB_BOOT_0 register */
    if (!ram_amount_value && ram_extension_8mb == NV3_PFB_BOOT_RAM_EXTENSION_8MB)       // 8MB (Riva 128 ZX)
        current_device.vram_amount = NV3_VRAM_SIZE_8MB;
    else if (ram_amount_value == NV3_PFB_BOOT_RAM_AMOUNT_4MB)                           // 4MB (Most Riva 128s)
        current_device.vram_amount = NV3_VRAM_SIZE_4MB;
    else if (ram_amount_value == NV3_PFB_BOOT_RAM_AMOUNT_2MB)                           // 2MB (Single NEC card)
        current_device.vram_amount = NV3_VRAM_SIZE_2MB;
    else if (!ram_amount_value && ram_extension_8mb == NV3_PFB_BOOT_RAM_EXTENSION_NONE) // 1MB (never existed)
        current_device.vram_amount = NV3_VRAM_SIZE_1MB;

    printf("Video RAM Size          = %u MB\n", (unsigned int)(current_device.vram_amount / 1048576));

    /* Read in the straps */
    current_device.straps = nv_mmio_read32(NV3_PSTRAPS);
    printf("Straps                  = 0x%08X\n", current_device.straps);

    uint32_t vpll = nv_mmio_read32(NV3_PRAMDAC_CLOCK_PIXEL);
    uint32_t mpll = nv_mmio_read32(NV3_PRAMDAC_CLOCK_MEMORY);
    
    printf("Pixel Clock Coefficient = 0x%08X\n", vpll);
    printf("Memory Clock Coefficient= 0x%08X\n", mpll);

    /* Power up all GPU subsystems */
    printf("Enabling all GPU subsystems (0x11111111 -> NV3_PMC_ENABLE)...");
    nv_mmio_write32(NV3_PMC_ENABLE, 0x11111111);
    printf("Done!\n");

    /* Enable interrupts */
    printf("Enabling interrupts...");
    nv_mmio_write32(NV3_PMC_INTERRUPT_ENABLE, (NV3_PMC_INTERRUPT_ENABLE_HARDWARE | NV3_PMC_INTERRUPT_ENABLE_SOFTWARE));
    printf("Done!\n");
 
    if (nv3_init_test_overclock())
        printf("Passed clock stability test\n");

    return true; 
}