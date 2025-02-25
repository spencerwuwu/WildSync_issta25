
FROM oss-fuzz-hopper

ARG TARGET

# ENVs
ENV HOPPER_RESULTS_DIR=/home/results/libheif_hopper
ENV BASE=/home/libheif/

ENV HEADER=${BASE}/libheif/heif.h
ENV SHARED_LIB=${BASE}/build/libheif/libheif.so
ENV LIB_PATH="${BASE}/build/libheif/:${BASE}/build/:${BASE}/libheif/:${BASE}/libheif/plugins/"

####################

RUN apt-get update && apt-get install -y \
    autoconf \
    automake \
    build-essential \
    cmake \
    libtool \
    make \
    mercurial \
    pkg-config \
    yasm

WORKDIR /home
USER hopper:hopper

RUN git clone \
    --no-checkout \
    https://github.com/strukturag/libheif.git \
    libheif
RUN git -C libheif checkout c98a3fae1ac63cc6052e40b26733396a8598542d

WORKDIR libheif

COPY --chown=hopper:hopper ${TARGET}/hopper_build.sh /home/
COPY hopper_build-addon.sh /home/
RUN cat /home/hopper_build-addon.sh >> /home/hopper_build.sh
COPY --chown=hopper:hopper hopper_fuzz.sh /home/
COPY --chown=hopper:hopper hopper_sanitize.py /home/

RUN /home/hopper_build.sh
ENTRYPOINT ["/bin/bash", "/home/hopper_fuzz.sh"]
