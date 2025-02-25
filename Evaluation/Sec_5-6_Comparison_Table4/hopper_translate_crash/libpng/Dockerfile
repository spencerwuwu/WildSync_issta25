
FROM oss-fuzz-hopper

ARG TARGET

# ENVs
ENV HOPPER_RESULTS_DIR=/home/results/libpng_hopper
ENV LIB_PATH="/home/libpng/"

ENV DEPS_LDD=${LIB_PATH}/
ENV HEADER=${LIB_PATH}/png.h
ENV SHARED_LIB=${LIB_PATH}/.libs/libpng16.so

####################

RUN apt-get update && \
    apt-get install -y make autoconf automake libtool zlib1g-dev vim

WORKDIR /home
USER hopper:hopper

RUN git clone --no-checkout https://github.com/pnggroup/libpng.git
RUN git -C libpng checkout f1848a3b560ddcad065242268433af475948461e

WORKDIR libpng
COPY --chown=hopper:hopper ${TARGET}/hopper_build.sh /home/
COPY hopper_build-addon.sh /home/
RUN cat /home/hopper_build-addon.sh >> /home/hopper_build.sh
COPY --chown=hopper:hopper hopper_fuzz.sh /home/
COPY --chown=hopper:hopper hopper_sanitize.py /home/

RUN /home/hopper_build.sh
ENTRYPOINT ["/bin/bash", "/home/hopper_fuzz.sh"]
