FROM ubuntu:24.04

ENV DEBIAN_FRONTEND=noninteractive

# Install essential build tools, dependencies, and QEMU
RUN apt-get update && apt-get install -y --no-install-recommends \
    build-essential \
    cmake \
    ninja-build \
    git \
    python3 \
    python3-pip \
    wget \
    curl \
    ca-certificates \
    gcc-arm-none-eabi \
    libstdc++-arm-none-eabi-newlib \
    libnewlib-arm-none-eabi \
    gdb-multiarch \
    qemu-system-arm \
    && rm -rf /var/lib/apt-get/lists/*

# Set up non-root developer user
RUN useradd -m -s /bin/bash developer
USER developer
WORKDIR /workspace

CMD ["/bin/bash"]