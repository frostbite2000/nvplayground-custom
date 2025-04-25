cmake_minimum_required(VERSION 3.7)
project(nvplay)

set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)
add_compile_options(-Wall -std=gnu99)

# Allow selecting between real and virtual PCI mode
option(USE_VIRTUAL_PCI "Use virtual PCI device instead of real hardware" ON)

# Define whether to use virtual PCI
if(USE_VIRTUAL_PCI)
    add_definitions(-DUSE_VIRTUAL_PCI)
    message(STATUS "Building with virtual PCI device support")
else()
    message(STATUS "Building with real hardware PCI support")
endif()

# Linux-specific
if(NOT USE_VIRTUAL_PCI)
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(LIBPCI REQUIRED libpci)
    include_directories(${LIBPCI_INCLUDE_DIRS})
endif()

# Include directories
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# Source files
set(SOURCES
    src/main.c
    src/core/nvcore_detect.c
    src/core/nvcore_io.c
    src/core/pci/linux_pci.c
    src/core/pci/virtual_pci.c
    src/architecture/nv3/nv3_core.c
    src/architecture/nv3/nv3_mode_table.c
)

add_executable(nvplay ${SOURCES})

# Link with libpci only when not using virtual PCI
if(NOT USE_VIRTUAL_PCI)
    target_link_libraries(nvplay ${LIBPCI_LIBRARIES})
endif()

# Copy assets if they exist
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/assets")
    add_custom_target(assets
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/assets
        ${CMAKE_BINARY_DIR}/assets
    )
    add_dependencies(nvplay assets)
endif()