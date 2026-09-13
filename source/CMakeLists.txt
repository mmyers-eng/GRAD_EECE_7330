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
# This will be important as models and TF features can
# bloat up the image size.
add_custom_command(TARGET qemu_app POST_BUILD
    COMMAND ${CMAKE_SIZE} $<TARGET_FILE:qemu_app>
)

#Tensorflow lite micro
#TFLM must be pre built using the build_lib.sh script
set(PREBUILT_TFLM_PATH "${CMAKE_SOURCE_DIR}/lib/microlite/libtensorflow-microlite.a")
set(TFLM_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/tflite-micro")

message(STATUS "Configuring prebuilt Microlite: ${PREBUILT_TFLM_PATH}")
add_library(microlite_prebuilt STATIC IMPORTED GLOBAL)
set_target_properties(microlite_prebuilt PROPERTIES
  IMPORTED_LOCATION ${PREBUILT_TFLM_PATH}
  INTERFACE_INCLUDE_DIRECTORIES ${TFLM_INCLUDE_DIR}
)

set(TFLM_DIR "${CMAKE_CURRENT_SOURCE_DIR}/source/tflite-micro")
set(TFLM_DOWNLOADS "${TFLM_INCLUDE_DIR}/tensorflow/lite/micro/tools/make/downloads/")

target_include_directories(qemu_app PRIVATE
  ${TFLM_INCLUDE_DIR}
  ${TFLM_DOWNLOADS}/flatbuffers/include
  ${TFLM_DOWNLOADS}/gemmlowp
  ${TFLM_DOWNLOADS}/kissfft
)
target_link_libraries(qemu_app PRIVATE microlite_prebuilt)
