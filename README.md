 # Distributed Embedded Local Machine Learning
This project explores embedded machine learning run on an emulated embedded target. 

# Core Principles

1. Reduction in dependencies:

## Setup Instructions
1. Install Docker:
https://docs.docker.com/engine/install/ubuntu/

2. Call the setup script.
``` bash
./setup.sh
```

3. Build the Docker Container
``` bash
cd ./docker
./build_docker.sh
 ```

## Application Build Instructions
Launch the docker container
``` bash
./launch_docker.sh
```

Build the TFML Library
NOTE: This command can take up to 30 minutes on slower machines.
``` bash
cd source
./build_lib.sh
```

Build the application
``` bash
./build_emu.sh
```

Launch and run the application in qemu
``` bash
./launch_emu.sh
```

When the test is finished in QEMU quit the test
CTRL-A X

## Test Instructions: Image Classification

1. Launch the docker container using 
``` bash
./launch_docker.sh
```
2. Build the app image using 
``` bash
./build_emu.sh
```
3. Launch the emu app using 
``` bash
./launch_emu_sock.sh
```
4. In a new terminal download https://cave.cs.toronto.edu/kriz/cifar-10-python.tar.gz
   to the /scripts directory.
5. Decompress the cifar-10-python.tar.gz in the scripts directory.
6. In the scripts directory run (you will only need to do this step once)
``` bash
python3 -m venv .venv
source .venv/bin/activate
pip install requirements.txt
```
7. In the scripts directory run:
```bash
python3 cifar_packet_test.py
```

## Debug instructions

Launch one container then:

``` bash
cd source
./debug_emu.sh
```

Launch a second container

``` bash
cd source
gdb-multiarch ./build/qemu_app.elf
target remote :1234
b main
```

## Tips:
- Kill a frozen Qemu session using CTRL-A X
- kill any process sitting on a socket fuser -k 1234/tcp 2>/dev/null

