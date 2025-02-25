#!/bin/bash -eu

FROM oss-fuzz-hopper

ARG TARGET

# ENVs
ENV HOPPER_RESULTS_DIR=/home/results/libpcap_hopper
ENV LIB_PATH="/home/libpcap/"

ENV DEPS_LDD=${LIB_PATH}/build/
ENV HEADER=${LIB_PATH}/pcap.h
ENV SHARED_LIB=${LIB_PATH}/build/libpcap.so

####################

RUN apt-get update && apt-get install -y make cmake flex bison libnl-genl-3-dev

WORKDIR /home
USER hopper:hopper

RUN git clone --no-checkout https://github.com/the-tcpdump-group/libpcap.git libpcap
RUN git -C libpcap checkout 15021c447e82edb2442c46eb72027d905a534c81

WORKDIR libpcap
COPY --chown=hopper:hopper ${TARGET}/hopper_build.sh /home/
COPY hopper_build-addon.sh /home/
RUN cat /home/hopper_build-addon.sh >> /home/hopper_build.sh
COPY --chown=hopper:hopper hopper_fuzz.sh /home/
COPY --chown=hopper:hopper hopper_sanitize.py /home/

RUN /home/hopper_build.sh
ENTRYPOINT ["/bin/bash", "/home/hopper_fuzz.sh"]
