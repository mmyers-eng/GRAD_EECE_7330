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
    libusb-1.0-0 \
    libusb-1.0-0-dev \
    zip \
    unzip \
    && rm -rf /var/lib/apt-get/lists/*


# Install ARM Mbed CLI
# note: --break-system-packages is required primarily due to conflicts with the
# mbed install
RUN python3 -m pip install --no-cache-dir --break-system-packages mbed-cli

# Install Python dependencies for tinyml benchmark runner
RUN python3 -m pip install --no-cache-dir --break-system-packages \
    pyusb \
    pyyaml \
    pyserial \
    numpy \
    pandas

# Set up non-root developer user
RUN useradd -m -s /bin/bash developer

USER developer
WORKDIR /workspace

CMD ["/bin/bash"]