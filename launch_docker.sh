#!/usr/bin/env bash
# launches the stm32-env
# assumes stm32-env has already been built

sudo docker run --rm -it \
  --user $(id -u):$(id -g) \
  -v $(pwd):/workspace \
  -w /workspace \
  stm32-env
