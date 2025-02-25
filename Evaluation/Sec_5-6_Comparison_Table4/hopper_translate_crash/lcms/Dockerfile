#!/bin/bash -eu

FROM oss-fuzz-hopper

ARG TARGET

# ENVs
ENV HOPPER_RESULTS_DIR=/home/results/lcms_hopper
ENV LIB_PATH="/home/lcms/"

ENV DEPS_LDD=${LIB_PATH}
ENV HEADER=${LIB_PATH}/include/lcms2.h
ENV SHARED_LIB=${LIB_PATH}/src/.libs/liblcms2.so

####################

RUN apt-get update && apt-get install -y make autoconf automake libtool

WORKDIR /home
USER hopper:hopper

RUN git clone --no-checkout https://github.com/mm2/Little-CMS.git lcms
RUN git -C lcms checkout 47a261e0b1579558184499ae5aadd0a04b1fb76f

WORKDIR lcms
COPY --chown=hopper:hopper ${TARGET}/hopper_build.sh /home/
COPY hopper_build-addon.sh /home/
RUN cat /home/hopper_build-addon.sh >> /home/hopper_build.sh
COPY --chown=hopper:hopper hopper_fuzz.sh /home/
COPY --chown=hopper:hopper hopper_sanitize.py /home/

RUN /home/hopper_build.sh
ENTRYPOINT ["/bin/bash", "/home/hopper_fuzz.sh"]
