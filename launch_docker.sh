#!/usr/bin/env bash
# launches the stm32-env
# assumes stm32-env has already been built

#Note --network host uses the host system's network space.
#TODO consider making a docker specific network
sudo docker run --rm -it --network host --user $(id -u):$(id -g) -v $(pwd):/workspace -w /workspace stm32-env
