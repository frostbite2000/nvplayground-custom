#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "nvplayground.h"
#include "core/nvcore.h"

#ifndef USE_VIRTUAL_PCI

static int mem_fd = -1;
static void *mmio_mapped_base = NULL;
static void *vram_mapped_base = NULL;
static void *ramin_mapped_base = NULL;

bool init_mmio_mappings(uint32_t bar0_base, uint32_t bar1_base)
{
    printf("Initializing memory mappings with real hardware access\n");
    
    // Open /dev/mem for physical memory mapping
    mem_fd = open("/dev/mem", O_RDWR | O_SYNC);
    if (mem_fd == -1) {
        perror("Failed to open /dev/mem");
        return false;
    }
    
    // Map BAR0 (MMIO registers)
    mmio_mapped_base = mmap(0, 0x1000000, PROT_READ | PROT_WRITE, MAP_SHARED,
                          mem_fd, bar0_base);
    if (mmio_mapped_base == MAP_FAILED) {
        perror("Failed to map BAR0 MMIO registers");
        close(mem_fd);
        return false;
    }
    
    // Map BAR1 VRAM area
    vram_mapped_base = mmap(0, 0x800000, PROT_READ | PROT_WRITE, MAP_SHARED,
                          mem_fd, bar1_base);
    if (vram_mapped_base == MAP_FAILED) {
        perror("Failed to map BAR1 VRAM");
        munmap(mmio_mapped_base, 0x1000000);
        close(mem_fd);
        return false;
    }
    
    // Map BAR1 RAMIN area (starts at 0xC00000 offset from BAR1 base)
    ramin_mapped_base = mmap(0, 0x400000, PROT_READ | PROT_WRITE, MAP_SHARED,
                           mem_fd, bar1_base + 0xC00000);
    if (ramin_mapped_base == MAP_FAILED) {
        perror("Failed to map BAR1 RAMIN");
        munmap(vram_mapped_base, 0x800000);
        munmap(mmio_mapped_base, 0x1000000);
        close(mem_fd);
        return false;
    }
    
    // Store mappings in the current_device structure
    current_device.mmio_mapping = mmio_mapped_base;
    current_device.vram_mapping = vram_mapped_base;
    current_device.ramin_mapping = ramin_mapped_base;
    
    printf("Memory mappings initialized successfully\n");
    return true;
}

void cleanup_mmio_mappings(void)
{
    if (ramin_mapped_base) {
        munmap(ramin_mapped_base, 0x400000);
        ramin_mapped_base = NULL;
        current_device.ramin_mapping = NULL;
    }
    
    if (vram_mapped_base) {
        munmap(vram_mapped_base, 0x800000);
        vram_mapped_base = NULL;
        current_device.vram_mapping = NULL;
    }
    
    if (mmio_mapped_base) {
        munmap(mmio_mapped_base, 0x1000000);
        mmio_mapped_base = NULL;
        current_device.mmio_mapping = NULL;
    }
    
    if (mem_fd != -1) {
        close(mem_fd);
        mem_fd = -1;
    }
    
    printf("Memory mappings cleaned up\n");
}

uint32_t nv_mmio_read32(uint32_t addr)
{
    volatile uint32_t *ptr;
    
    if (addr < 0x1000000) {
        // MMIO access
        if (!mmio_mapped_base) {
            printf("Error: Attempted MMIO read before initialization\n");
            return 0xFFFFFFFF;
        }
        ptr = (volatile uint32_t *)((uint8_t *)mmio_mapped_base + addr);
    } 
    else if (addr >= 0x1000000 && addr < 0x1800000) {
        // VRAM access
        if (!vram_mapped_base) {
            printf("Error: Attempted VRAM read before initialization\n");
            return 0xFFFFFFFF;
        }
        ptr = (volatile uint32_t *)((uint8_t *)vram_mapped_base + (addr - 0x1000000));
    }
    else if (addr >= 0x1C00000 && addr < 0x2000000) {
        // RAMIN access
        if (!ramin_mapped_base) {
            printf("Error: Attempted RAMIN read before initialization\n");
            return 0xFFFFFFFF;
        }
        ptr = (volatile uint32_t *)((uint8_t *)ramin_mapped_base + (addr - 0x1C00000));
    }
    else {
        printf("Error: Invalid MMIO read address: 0x%08X\n", addr);
        return 0xFFFFFFFF;
    }
    
    return *ptr;
}

void nv_mmio_write32(uint32_t addr, uint32_t value)
{
    volatile uint32_t *ptr;
    
    if (addr < 0x1000000) {
        // MMIO access
        if (!mmio_mapped_base) {
            printf("Error: Attempted MMIO write before initialization\n");
            return;
        }
        ptr = (volatile uint32_t *)((uint8_t *)mmio_mapped_base + addr);
    } 
    else if (addr >= 0x1000000 && addr < 0x1800000) {
        // VRAM access
        if (!vram_mapped_base) {
            printf("Error: Attempted VRAM write before initialization\n");
            return;
        }
        ptr = (volatile uint32_t *)((uint8_t *)vram_mapped_base + (addr - 0x1000000));
    }
    else if (addr >= 0x1C00000 && addr < 0x2000000) {
        // RAMIN access
        if (!ramin_mapped_base) {
            printf("Error: Attempted RAMIN write before initialization\n");
            return;
        }
        ptr = (volatile uint32_t *)((uint8_t *)ramin_mapped_base + (addr - 0x1C00000));
    }
    else {
        printf("Error: Invalid MMIO write address: 0x%08X\n", addr);
        return;
    }
    
    *ptr = value;
}

#else

// In virtual mode, we use the functions from virtual_pci.c
extern uint32_t virtual_mmio_read32(uint32_t addr);
extern void virtual_mmio_write32(uint32_t addr, uint32_t value);

bool init_mmio_mappings(uint32_t bar0_base, uint32_t bar1_base)
{
    printf("Using virtual MMIO mappings (no real hardware access)\n");
    return true;  // Nothing to do, virtual mappings already set up
}

void cleanup_mmio_mappings(void)
{
    // Nothing to do, virtual mappings cleaned up in virtual_pci_cleanup
}

uint32_t nv_mmio_read32(uint32_t addr)
{
    return virtual_mmio_read32(addr);
}

void nv_mmio_write32(uint32_t addr, uint32_t value)
{
    virtual_mmio_write32(addr, value);
}

#endif // USE_VIRTUAL_PCI