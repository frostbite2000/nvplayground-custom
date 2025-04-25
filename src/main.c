#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include "nvplayground.h"
#include "core/nvcore.h"
#include "core/pci/pci.h"

// Signal handling
static void cleanup(void);
static void signal_handler(int sig)
{
    printf("\nCaught signal %d, cleaning up and exiting...\n", sig);
    cleanup();
    exit(1);
}

// Main application cleanup
static void cleanup(void)
{
    // Clean up hardware resources
    cleanup_mmio_mappings();
    pci_cleanup();
}

int main(int argc, char *argv[])
{
    // Register signal handlers
    signal(SIGINT, signal_handler);
    signal(SIGTERM, signal_handler);
    
    // Print welcome message
    printf("\n\n");
    printf("=================================================================\n");
    printf(APP_SIGNON_STRING);
    printf("=================================================================\n\n");

#ifdef USE_VIRTUAL_PCI    
    printf("Running with virtual PCI device (no hardware access)\n\n");
#else
    printf("Running with real hardware PCI access\n\n");
#endif

    // Initialize PCI subsystem
    if (!pci_init()) {
        fprintf(stderr, "Failed to initialize PCI subsystem\n");
        return 1;
    }
    
    // Detect supported NVIDIA GPU
    if (!nv_detect()) {
        fprintf(stderr, "No supported NVIDIA GPU found\n");
        pci_cleanup();
        return 2;
    }
    
    // Make sure the GPU is supported
    if (!current_device.device_info.init_function) {
        fprintf(stderr, "This GPU is not yet supported :(\n");
        cleanup();
        return 3;
    }
    
    // Initialize the GPU
    printf("Initializing GPU: %s\n", current_device.device_info.name);
    if (!current_device.device_info.init_function()) {
        fprintf(stderr, "Failed to initialize GPU\n");
        cleanup();
        return 4;
    }
    
    printf("\nGPU initialized successfully!\n");
    printf("Press Ctrl+C to exit...\n");
    
    // Main loop - in a real application, you would handle events here
    while (1) {
        sleep(1);
    }
    
    // Cleanup and exit
    cleanup();
    return 0;
}