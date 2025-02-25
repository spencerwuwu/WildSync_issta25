#!/bin/bash -eu

FROM oss-fuzz-hopper

ARG TARGET

# ENVs
ENV HOPPER_RESULTS_DIR=/home/results/zlib_hopper
ENV LIB_PATH="/home/zlib/"

ENV DEPS_LDD=${LIB_PATH}
ENV HEADER=${LIB_PATH}/zlib.h
ENV SHARED_LIB=${LIB_PATH}/libz.so

####################

RUN apt-get update && apt-get install -y make autoconf automake libtool vim

WORKDIR /home
USER hopper:hopper

RUN git clone --no-checkout -b develop https://github.com/madler/zlib.git
RUN git -C zlib checkout d476828316d05d54c6fd6a068b121b30c147b5cd

WORKDIR zlib
COPY --chown=hopper:hopper ${TARGET}/hopper_build.sh /home/
COPY hopper_build-addon.sh /home/
RUN cat /home/hopper_build-addon.sh >> /home/hopper_build.sh
COPY --chown=hopper:hopper hopper_fuzz.sh /home/
COPY --chown=hopper:hopper hopper_sanitize.py /home/

RUN /home/hopper_build.sh
ENTRYPOINT ["/bin/bash", "/home/hopper_fuzz.sh"]
