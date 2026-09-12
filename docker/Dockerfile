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
    software-properties-common \
    gcc-arm-none-eabi \
    libstdc++-arm-none-eabi-newlib \
    libnewlib-arm-none-eabi \
    gdb-multiarch \
    qemu-system-arm \
    libusb-1.0-0 \
    libusb-1.0-0-dev \
    zip \
    unzip \
    xxd \
    mercurial \
    && rm -rf /var/lib/apt-get/lists/*


# Install ARM Mbed CLI
# note: --break-system-packages is required primarily due to conflicts with the
# mbed install
RUN python3 -m pip install --no-cache-dir --break-system-packages mbed-cli
#install mbed tools
RUN python3 -m pip install --no-cache-dir --break-system-packages mbed-tools


# Install Python dependencies for tinyml benchmark runner
# TBD These may be deleted if tinyML is not used
RUN python3 -m pip install --no-cache-dir --break-system-packages \
    pyusb \
    pyyaml \
    pyserial \
    numpy \
    pandas

#TF Lite Micro Deps
RUN add-apt-repository -y ppa:deadsnakes/ppa && \
    apt-get update && \
    apt-get install -y --no-install-recommends \
        python3.10 \
        python3.10-venv \
        python3.10-dev \
    && rm -rf /var/lib/apt/lists/*

RUN python3.10 -m venv /opt/venv
ENV PATH="/opt/venv/bin:$PATH"

COPY ./source/tflite-micro/python/python_requirements_3_10.txt /tmp/tflm_reqs.txt
RUN pip3 install --no-cache-dir --break-system-packages -r /tmp/tflm_reqs.txt
#End of TF Lite Micro deps

# Set up non-root developer user
RUN useradd -m -s /bin/bash developer

USER developer
WORKDIR /workspace

CMD ["/bin/bash"]