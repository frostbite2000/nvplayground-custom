#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "nvplayground.h"
#include "core/nvcore.h"
#include "core/pci/pci.h"

// Memory for emulating virtual hardware
static uint32_t *virtual_mmio = NULL;
static uint32_t *virtual_vram = NULL;
static uint32_t *virtual_ramin = NULL;

// Virtual PCI configuration space
static uint8_t virtual_pci_config[256] = {0};

// Virtual device information
static struct {
    uint32_t device_id;
    uint32_t vendor_id;
    uint32_t bar0_addr;
    uint32_t bar1_addr;
    bool initialized;
} virtual_device = {0};

// Virtual MMIO access functions
uint32_t virtual_mmio_read32(uint32_t addr)
{
    if (!virtual_mmio || addr >= 0x1000000) {
        printf("Virtual MMIO: Invalid read from address 0x%08X\n", addr);
        return 0xFFFFFFFF;
    }
    
    uint32_t value = virtual_mmio[addr/4];
    
    // Log reads from important registers
    switch (addr) {
        case 0x000000: // PMC_BOOT
            printf("Virtual MMIO: Read PMC_BOOT = 0x%08X\n", value);
            break;
        case 0x100000: // PFB_BOOT
            printf("Virtual MMIO: Read PFB_BOOT = 0x%08X\n", value);
            break;
        case 0x101000: // PSTRAPS
            printf("Virtual MMIO: Read PSTRAPS = 0x%08X\n", value);
            break;
    }
    
    return value;
}

void virtual_mmio_write32(uint32_t addr, uint32_t value)
{
    if (!virtual_mmio || addr >= 0x1000000) {
        printf("Virtual MMIO: Invalid write to address 0x%08X (value 0x%08X)\n", addr, value);
        return;
    }
    
    // Log writes to important registers
    switch (addr) {
        case 0x000200: // PMC_ENABLE
            printf("Virtual MMIO: Write PMC_ENABLE = 0x%08X (", value);
            if (value == 0x11111111) 
                printf("all subsystems enabled)\n");
            else
                printf("partial subsystem enable)\n");
            break;
            
        case 0x000140: // PMC_INTR_EN
            printf("Virtual MMIO: Write PMC_INTR_EN = 0x%08X (interrupts %s)\n", 
                  value, (value & 0x3) ? "enabled" : "disabled");
            break;
            
        case 0x680504: // PRAMDAC_CLOCK_MEMORY
            printf("Virtual MMIO: Write PRAMDAC_CLOCK_MEMORY = 0x%08X\n", value);
            
            // Extract clock parameters
            uint32_t vdiv = value & 0xFF;
            uint32_t ndiv = (value >> 8) & 0xFF;
            uint32_t pdiv = (value >> 16) & 0x7;
            
            // Calculate approximate clock frequency in MHz
            float base_freq = 14.31818f; // 14.31818 MHz crystal
            float mclk = (base_freq * ndiv) / (vdiv * (1 << pdiv));
            
            printf("Virtual MMIO: MCLK set to approximately %.2f MHz\n", mclk);
            break;
    }
    
    virtual_mmio[addr/4] = value;
}

bool virtual_pci_init(void)
{
    printf("Initializing virtual PCI device...\n");
    
    // Allocate memory for virtual hardware components
    virtual_mmio = (uint32_t *)calloc(0x1000000 / 4, sizeof(uint32_t));
    virtual_vram = (uint32_t *)calloc(0x800000 / 4, sizeof(uint32_t));  // 8MB VRAM
    virtual_ramin = (uint32_t *)calloc(0x400000 / 4, sizeof(uint32_t)); // 4MB RAMIN
    
    if (!virtual_mmio || !virtual_vram || !virtual_ramin) {
        printf("Failed to allocate memory for virtual hardware\n");
        virtual_pci_cleanup();
        return false;
    }
    
    // Set default BAR addresses
    virtual_device.bar0_addr = 0xF0000000;  // Virtual BAR0 address
    virtual_device.bar1_addr = 0xF8000000;  // Virtual BAR1 address
    virtual_device.initialized = true;
    
    // Initialize core MMIO registers for NV3
    
    // PMC_BOOT (NV3 Rev B0)
    virtual_mmio[0x000000/4] = 0x00030110;  // NV3_BOOT_REG_REV_B00
    
    // PFB_BOOT (4MB VRAM, 64-bit wide, 2 banks)
    virtual_mmio[0x100000/4] = (0x2 << 0) | (0x0 << 2) | (0x0 << 3);
    
    // PSTRAPS register
    virtual_mmio[0x101000/4] = (0x1 << 6) | (0x1 << 1) | (0x1 << 0); // 14.31818 MHz, 66MHz, BIOS present
    
    // PRAMDAC_CLOCK_MEMORY (100 MHz default)
    virtual_mmio[0x680504/4] = 0x0EC40E;
    
    printf("Virtual NV3 device initialized successfully\n");
    return true;
}

void virtual_pci_register_device(uint32_t device_id, uint32_t vendor_id)
{
    if (!virtual_device.initialized) {
        printf("Virtual PCI: Cannot register device, not initialized\n");
        return;
    }
    
    virtual_device.device_id = device_id;
    virtual_device.vendor_id = vendor_id;
    
    // Initialize PCI config space for the registered device
    
    // Device ID and Vendor ID
    virtual_pci_config[0] = vendor_id & 0xFF;
    virtual_pci_config[1] = (vendor_id >> 8) & 0xFF;
    virtual_pci_config[2] = device_id & 0xFF;
    virtual_pci_config[3] = (device_id >> 8) & 0xFF;
    
    // Command register: I/O and Memory space enabled
    virtual_pci_config[4] = 0x03;
    virtual_pci_config[5] = 0x00;
    
    // Status register: 66MHz capable, fast back-to-back capable
    virtual_pci_config[6] = 0x00;
    virtual_pci_config[7] = 0x20;
    
    // Revision ID: NV3 Rev B (0x10)
    virtual_pci_config[8] = 0x10;
    
    // Class code: Display controller (0x030000)
    virtual_pci_config[9] = 0x00;   // Programming interface
    virtual_pci_config[10] = 0x00;  // Subclass
    virtual_pci_config[11] = 0x03;  // Base class
    
    // BAR0: Memory mapped, non-prefetchable, 32-bit
    virtual_pci_config[0x10] = (virtual_device.bar0_addr & 0xFF);
    virtual_pci_config[0x11] = (virtual_device.bar0_addr >> 8) & 0xFF;
    virtual_pci_config[0x12] = (virtual_device.bar0_addr >> 16) & 0xFF;
    virtual_pci_config[0x13] = (virtual_device.bar0_addr >> 24) & 0xFF;
    
    // BAR1: Memory mapped, prefetchable, 32-bit
    virtual_pci_config[0x14] = (virtual_device.bar1_addr & 0xFF);
    virtual_pci_config[0x15] = (virtual_device.bar1_addr >> 8) & 0xFF;
    virtual_pci_config[0x16] = (virtual_device.bar1_addr >> 16) & 0xFF;
    virtual_pci_config[0x17] = (virtual_device.bar1_addr >> 24) & 0xFF;
    
    printf("Virtual PCI: Registered device %04X:%04X\n", device_id, vendor_id);
    
    // Set up device bus and function information for the framework
    current_device.bus_number = 0;
    current_device.function_number = 0;
}

void virtual_pci_cleanup(void)
{
    if (virtual_mmio) {
        free(virtual_mmio);
        virtual_mmio = NULL;
    }
    
    if (virtual_vram) {
        free(virtual_vram);
        virtual_vram = NULL;
    }
    
    if (virtual_ramin) {
        free(virtual_ramin);
        virtual_ramin = NULL;
    }
    
    virtual_device.initialized = false;
    printf("Virtual PCI device cleaned up\n");
}

uint32_t virtual_pci_read_config_8(uint32_t bus_number, uint32_t function_number, uint32_t offset)
{
    if (offset >= sizeof(virtual_pci_config)) {
        printf("Virtual PCI: Invalid 8-bit config read at offset 0x%X\n", offset);
        return 0xFF;
    }
    
    uint8_t value = virtual_pci_config[offset];
    printf("Virtual PCI: Read config byte at 0x%X = 0x%02X\n", offset, value);
    return value;
}

uint32_t virtual_pci_read_config_16(uint32_t bus_number, uint32_t function_number, uint32_t offset)
{
    if (offset % 2 != 0 || offset >= sizeof(virtual_pci_config) - 1) {
        printf("Virtual PCI: Invalid 16-bit config read at offset 0x%X\n", offset);
        return 0xFFFF;
    }
    
    uint16_t value = virtual_pci_config[offset] | (virtual_pci_config[offset+1] << 8);
    printf("Virtual PCI: Read config word at 0x%X = 0x%04X\n", offset, value);
    return value;
}

uint32_t virtual_pci_read_config_32(uint32_t bus_number, uint32_t function_number, uint32_t offset)
{
    if (offset % 4 != 0 || offset >= sizeof(virtual_pci_config) - 3) {
        printf("Virtual PCI: Invalid 32-bit config read at offset 0x%X\n", offset);
        return 0xFFFFFFFF;
    }
    
    uint32_t value = virtual_pci_config[offset] | 
                   (virtual_pci_config[offset+1] << 8) |
                   (virtual_pci_config[offset+2] << 16) |
                   (virtual_pci_config[offset+3] << 24);
    
    printf("Virtual PCI: Read config dword at 0x%X = 0x%08X\n", offset, value);
    
    // Special handling for BAR registers
    if (offset == PCI_CFG_OFFSET_BAR0) {
        printf("Virtual PCI: Read BAR0 (0x%08X)\n", value);
    } else if (offset == PCI_CFG_OFFSET_BAR1) {
        printf("Virtual PCI: Read BAR1 (0x%08X)\n", value);
    }
    
    return value;
}