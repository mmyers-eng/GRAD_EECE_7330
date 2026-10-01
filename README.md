# Distributed Embedded Local Machine Learning
This project explores embedded machine learning run on an embedded target. Presently the project builds an example and launches it in qemu, printing a message once the example is booted.


## Setup Instructions
1. Install Docker:
https://docs.docker.com/engine/install/ubuntu/

2. Initialize and update all submodules
- `git submodule init`
- `git submodule update`

3. Build the Docker Container
- `cd ./docker`
- `./build_docker.sh`

## Application Build Instructions
Launch the docker container
- `./launch_docker.sh`

Build the TFML Library
NOTE: This command can take up to 30 minutes on slower machines.
- `cd source`
- `./build_lib.sh`

Build the application
- `./build_emu.sh`

Launch and run the application in qemu
- `./launch_emu.sh`

When the test is finished in QEMU quit the test
CTRL-A X

## Debug instructions

Launch one container then:
- `cd source`
- `./debug_emu.sh`

Launch a second container
- `cd source`
- `gdb-multiarch ./build/qemu_app.elf`
- `target remote :1234`
- `b main`

## Tips:
- Kill a frozen Qemu session using CTRL-A X
- kill any process sitting on a socket fuser -k 1234/tcp 2>/dev/null
