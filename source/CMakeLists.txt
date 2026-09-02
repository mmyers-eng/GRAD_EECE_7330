cmake_minimum_required(VERSION 3.20)

project(qemu_baremetal_cpp C CXX ASM)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(qemu_app
    startup.s
    main.cpp
)

# Linker flags for bare-metal output (nosys specs for stubbed system calls)
#target_link_options(qemu_app PRIVATE
#"-specs=nosys.specs"
#    "-Wl,--gc-sections"
#    "-Wl,-Ttext=0x08000000" # Flash memory start address for STM32
#)

# Use linker file
target_link_options(qemu_app PRIVATE
  -T${CMAKE_CURRENT_SOURCE_DIR}/linker.ld
  -nostartfiles
)

# Output size after build
add_custom_command(TARGET qemu_app POST_BUILD
    COMMAND ${CMAKE_SIZE} $<TARGET_FILE:qemu_app>
)
