This project explores embedded machine learning. Presently the project builds an example
and launches it in qemu, printing a message once the example is booted.





Setup Instructions

Docker installation:
Install Docker:
https://docs.docker.com/engine/install/ubuntu/

Building the docker container
cd ./docker
docker build -t stm32-env .

Launching the docker container once built
./launch_docker.sh

Run the docker container mounting this repo as "workspace"
cd ..

Build Instructions
cd source
./build_emu

Debug instructions
Launch one container then:
cd source
./debug_emu

Launch a second container
cd source
gdb-multiarch ./build/qemu_app
target remote :1234

Tips:
- Kill a frozen Qemu session using CTRL-A X
- kill any process sitting on a socket fuser -k 1234/tcp 2>/dev/null

Debugging
gdb-multiarch ./build/qemu_app
target remote :1234
info registers
info functions
