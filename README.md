# Distributed Embedded Local Machine Learning
This project explores embedded machine learning run on an embedded target. Presently the project builds an example
and launches it in qemu, printing a message once the example is booted.



## Setup Instructions
### Docker installation:
Install Docker:
https://docs.docker.com/engine/install/ubuntu/

### Building the docker container
cd ./docker
docker build -t stm32-env .

#### Launching the docker container once built
./launch_docker.sh

## Build Instructions
cd source
./build_emu

## Debug instructions

Launch one container then:
cd source
./debug_emu

Launch a second container
cd source
gdb-multiarch ./build/qemu_app
target remote :1234

## Tips:
- Kill a frozen Qemu session using CTRL-A X
- kill any process sitting on a socket fuser -k 1234/tcp 2>/dev/null

## Useful Debugging Commands
gdb-multiarch ./build/qemu_app
target remote :1234
info registers
info functions


# Building tinyML
1. cd tinyml/benchmark/reference_submissions/image_classification
2. ./setup_example.sh NOTE: This is getting stuck on https://www.cs.toronto.edu/~kriz/cifar-10-binary.tar.gz which is a huge image library

3. mbed compile -m NUCLEO_L4R5ZI -t GCC_ARM

Note: Freezing on tensorflow/lite/micro/tools/make/download_and_extract.sh "https://www.cs.toronto.edu/~kriz/cifar-10-binary.tar.gz"

This is part of the tensor flow lite install