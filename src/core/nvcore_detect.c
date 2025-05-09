#include "core/nvcore.h"
#include "architecture/nv3/nv3.h"

//
// Globals
//
nv_device_info_t supported_devices[] = 
{
    { PCI_DEVICE_NV1_NV, PCI_VENDOR_SGS, "NV1 (STG-2000 DRAM version)", NULL, NULL, NULL, NULL, },
    { PCI_DEVICE_NV1_NV, PCI_VENDOR_NV, "NV1 (NV1 VRAM version)", NULL, NULL, NULL, NULL, },
    { PCI_DEVICE_NV2, PCI_VENDOR_NV, "NV2 (Mutara V08) (You don't have this)", NULL, NULL, NULL, NULL, },
    { PCI_DEVICE_NV3, PCI_VENDOR_SGS_NV, "Riva 128 (NV3), or Riva 128 ZX without ACPI support (NV3T)", nv3_init, NULL, NULL, NULL, }, 
    { PCI_DEVICE_NV3T_ACPI, PCI_VENDOR_SGS_NV, "Riva 128 ZX with ACPI support (NV3T)", nv3_init, NULL, NULL, NULL, },
    { PCI_DEVICE_NV4, PCI_VENDOR_NV, "Riva TNT (NV4)", NULL, NULL, NULL, NULL, },
    { PCI_DEVICE_NV5, PCI_VENDOR_NV, "Riva TNT2 / TNT2 Pro (NV5)", NULL, NULL, NULL, NULL, },
    { PCI_DEVICE_NV5_ULTRA, PCI_VENDOR_NV, "Riva TNT2 Ultra (NV5_ULTRA)", NULL, NULL, NULL, NULL, },
    { PCI_DEVICE_NV5_CRAP, PCI_VENDOR_NV, "Vanta (Riva TNT2 derivative) (NV5_VANTA)", NULL, NULL, NULL, NULL, },
    { PCI_DEVICE_NV6, PCI_VENDOR_NV, "Riva TNT2 M64 (NV6)", NULL, NULL, NULL, NULL, },
    { PCI_DEVICE_NV10, PCI_VENDOR_NV, "GeForce 256 with SDRAM (NV10)", NULL, NULL, NULL, NULL, },
    { PCI_DEVICE_NV10_DDR, PCI_VENDOR_NV, "GeForce 256 with DDR (NV10)", NULL, NULL, NULL, NULL, },
    { PCI_DEVICE_NV10_QUADRO, PCI_VENDOR_NV, "Quadro (NV10GL)", NULL, NULL, NULL, NULL, },
    { 0, 0, "", NULL, NULL, NULL, NULL, }, // sentinel
};

// The selected device after detection is done. 
nv_device_t current_device = {0}; 

bool nv_detect()
{
    nv_device_info_t current_device_info = supported_devices[0]; 
    int32_t i = 0; 

    // iterate through all the known device info sets
    while (current_device_info.vendor_id != 0x00)
    {
        current_device_info = supported_devices[i];
        
        printf("Trying to find GPU: %s\n", current_device_info.name);

        if (pci_does_device_exist(current_device_info.device_id, current_device_info.vendor_id))
        {
            printf("Detected GPU: %s\n", current_device_info.name);

            // set up current info
            current_device.device_info = current_device_info;
            return true; 
        }

        i++;
    }

    printf("No supported Nvidia GPU found\n");
    return false; 
}