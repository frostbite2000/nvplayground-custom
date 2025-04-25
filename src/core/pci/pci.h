#pragma once

#include <stdbool.h>
#include <stdint.h>

// PCI interface functions
bool pci_init(void);
bool pci_does_device_exist(uint32_t device_id, uint32_t vendor_id);
uint32_t pci_read_config_8(uint32_t bus_number, uint32_t function_number, uint32_t offset);
uint32_t pci_read_config_16(uint32_t bus_number, uint32_t function_number, uint32_t offset);
uint32_t pci_read_config_32(uint32_t bus_number, uint32_t function_number, uint32_t offset);
void pci_cleanup(void);

// Virtual PCI functions
bool virtual_pci_init(void);
void virtual_pci_cleanup(void);
uint32_t virtual_pci_read_config_8(uint32_t bus_number, uint32_t function_number, uint32_t offset);
uint32_t virtual_pci_read_config_16(uint32_t bus_number, uint32_t function_number, uint32_t offset);
uint32_t virtual_pci_read_config_32(uint32_t bus_number, uint32_t function_number, uint32_t offset);
void virtual_pci_register_device(uint32_t device_id, uint32_t vendor_id);