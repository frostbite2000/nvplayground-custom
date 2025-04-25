#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "nvplayground.h"
#include "core/nvcore.h"
#include "core/pci/pci.h"

#ifndef USE_VIRTUAL_PCI
#include <pci/pci.h>

static struct pci_access *pacc = NULL;

bool pci_init(void)
{
    pacc = pci_alloc();
    if (!pacc) {
        printf("Failed to initialize libpci\n");
        return false;
    }
    
    pci_init(pacc);
    pci_scan_bus(pacc);
    
    printf("Linux PCI system initialized\n");
    return true;
}

bool pci_does_device_exist(uint32_t device_id, uint32_t vendor_id)
{
    struct pci_dev *dev;
    
    for (dev = pacc->devices; dev; dev = dev->next) {
        if (dev->vendor_id == vendor_id && dev->device_id == device_id) {
            current_device.bus_number = dev->bus;
            current_device.function_number = dev->dev | (dev->func << 3);
            printf("Found PCI device %04x:%04x at bus %02x device %02x function %d\n",
                   vendor_id, device_id, dev->bus, dev->dev, dev->func);
            return true;
        }
    }
    
    return false;
}

uint32_t pci_read_config_8(uint32_t bus_number, uint32_t function_number, uint32_t offset)
{
    struct pci_dev *dev;
    uint8_t value = 0;
    
    for (dev = pacc->devices; dev; dev = dev->next) {
        if (dev->bus == bus_number && 
            (dev->dev | (dev->func << 3)) == function_number) {
            value = pci_read_byte(dev, offset);
            return value;
        }
    }
    
    printf("Failed to read 8-bit PCI config at bus %u function %u offset 0x%X\n", 
           bus_number, function_number, offset);
    return 0;
}

uint32_t pci_read_config_16(uint32_t bus_number, uint32_t function_number, uint32_t offset)
{
    if (offset % 0x02) {
        printf("Error: pci_read_config_16 called with unaligned address 0x%X\n", offset);
        return 0x00;
    }
    
    struct pci_dev *dev;
    uint16_t value = 0;
    
    for (dev = pacc->devices; dev; dev = dev->next) {
        if (dev->bus == bus_number && 
            (dev->dev | (dev->func << 3)) == function_number) {
            value = pci_read_word(dev, offset);
            return value;
        }
    }
    
    printf("Failed to read 16-bit PCI config at bus %u function %u offset 0x%X\n", 
           bus_number, function_number, offset);
    return 0;
}

uint32_t pci_read_config_32(uint32_t bus_number, uint32_t function_number, uint32_t offset)
{
    if (offset % 0x04) {
        printf("Error: pci_read_config_32 called with unaligned address 0x%X\n", offset);
        return 0x00;
    }
    
    struct pci_dev *dev;
    uint32_t value = 0;
    
    for (dev = pacc->devices; dev; dev = dev->next) {
        if (dev->bus == bus_number && 
            (dev->dev | (dev->func << 3)) == function_number) {
            value = pci_read_long(dev, offset);
            return value;
        }
    }
    
    printf("Failed to read 32-bit PCI config at bus %u function %u offset 0x%X\n", 
           bus_number, function_number, offset);
    return 0;
}

void pci_cleanup(void)
{
    if (pacc) {
        pci_cleanup(pacc);
        pacc = NULL;
    }
}

#else

// These are just stubs when using virtual PCI
bool pci_init(void) 
{
    return virtual_pci_init();
}

bool pci_does_device_exist(uint32_t device_id, uint32_t vendor_id)
{
    // Register the device with the virtual PCI system
    virtual_pci_register_device(device_id, vendor_id);
    return true;
}

uint32_t pci_read_config_8(uint32_t bus_number, uint32_t function_number, uint32_t offset)
{
    return virtual_pci_read_config_8(bus_number, function_number, offset);
}

uint32_t pci_read_config_16(uint32_t bus_number, uint32_t function_number, uint32_t offset)
{
    return virtual_pci_read_config_16(bus_number, function_number, offset);
}

uint32_t pci_read_config_32(uint32_t bus_number, uint32_t function_number, uint32_t offset)
{
    return virtual_pci_read_config_32(bus_number, function_number, offset);
}

void pci_cleanup(void)
{
    virtual_pci_cleanup();
}

#endif // !USE_VIRTUAL_PCI