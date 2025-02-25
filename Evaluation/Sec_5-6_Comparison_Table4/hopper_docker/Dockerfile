FROM gcr.io/oss-fuzz-base/base-builder


ENV SHARED=/hopper_shared

ARG USER_ID=1000
ARG GROUP_ID=1000

RUN apt-get update && apt-get install -y sudo

RUN mkdir -p /home/ && \
    groupadd -g ${GROUP_ID} hopper && \
    useradd -l -u ${USER_ID} -K UMASK=0000 -d /home -g hopper hopper && \
    chown hopper:hopper /home

RUN echo "hopper:hopper" | chpasswd && usermod -a -G sudo hopper
RUN echo '%sudo ALL=(ALL) NOPASSWD:ALL' >> /etc/sudoers


RUN apt-get update && apt-get install -y cmake vim

# For Hopper
ENV HOPPER_BIN=/home/hopper/hopper \
    RUSTUP_HOME=/home/rustup \
    CARGO_HOME=/home/cargo \
    PATH=/home/hopper:/home/cargo/bin:/home/.cargo/bin:$PATH \
    DEBIAN_FRONTEND=noninteractive 

RUN apt-get update \
    && apt-get -y upgrade \
    && apt-get -y install build-essential wget curl cmake git unzip xxd protobuf-compiler libprotobuf-dev daemontools \
    && apt-get -y install llvm llvm-dev libclang-dev clang \
    && apt-get clean

RUN apt -y install manpages-dev software-properties-common
RUN add-apt-repository ppa:ubuntu-toolchain-r/test && apt update

RUN apt-get -y install libnl-3-dev libnl-route-3-dev libibverbs-dev libsystemd-dev gcc-11-plugin-dev lsb-release build-essential rustfmt libtool libtool-bin libpixman-1-dev gdb
RUN pip3 install pyelftools lief

RUN curl --proto '=https' --tlsv1.2 -sSf https://sh.rustup.rs | sh -s -- -y --default-toolchain stable

ENV SCRIPT_DIR=/home
RUN rm -rf $SCRIPT_DIR/casr && git clone https://github.com/ispras/casr $SCRIPT_DIR/casr && cd $SCRIPT_DIR/casr && cargo update && cargo build --release

RUN chown -R hopper:hopper /home

ENV PATH=/home/casr/target/release/:$PATH

USER hopper:hopper

RUN git clone --no-checkout https://github.com/spencerwuwu/Hopper.git /home/hopper
RUN git -C /home/hopper checkout 5a4fa085075a12ded3e0c40bfbf13359339a086d
RUN cd /home/hopper && ./build.sh

COPY --chown=hopper:hopper hopper_casr.py /home
COPY --chown=hopper:hopper process_casr.sh /home

USER root:root
