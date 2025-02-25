
FROM oss-fuzz-hopper

ARG TARGET

# ENVs
ENV HOPPER_RESULTS_DIR=/home/results/leptonica_hopper
ENV BASE=/home/leptonica/

ENV HEADER=${BASE}/src/allheaders.h
ENV SHARED_LIB=${BASE}/src/.libs/libleptonica.so
ENV LIB_PATH="${BASE}/src"

####################

RUN apt-get update && apt-get install -y make autoconf automake libtool \
  pkg-config cmake nasm

RUN apt-get update && apt-get install -y \
    zlib1g-dev \
    libtiff-dev \
    libpng-dev \
    libwebp-dev \
    libjbig-dev \
    libjpeg-dev \
    libzstd-dev 

WORKDIR /home
USER hopper:hopper

RUN git clone --no-checkout https://github.com/DanBloomberg/leptonica.git leptonica
RUN git -C leptonica checkout 32af1df77f4f6311e581f2e263fc51506a5b6340

RUN git clone --no-checkout https://github.com/madler/zlib.git zlib
RUN git -C zlib checkout 99b229487c2997b4b22eaef90fedfbff4d8826cc
RUN git clone --no-checkout https://gitlab.com/libtiff/libtiff libtiff
RUN git -C libtiff checkout 1d28fe638020f55644319599ece84b3fb0b39bb5
RUN git clone --no-checkout https://github.com/glennrp/libpng.git libpng
RUN git -C libpng checkout f1848a3b560ddcad065242268433af475948461e
RUN git clone --no-checkout https://chromium.googlesource.com/webm/libwebp libwebp
RUN git -C libwebp checkout a16d30cb578cedd3b3a2d75bf041f99310fdef63
RUN git clone --no-checkout https://www.cl.cam.ac.uk/~mgk25/git/jbigkit jbigkit
RUN git -C jbigkit checkout 7d3c1bea895d910907e2501fe9165e353eceabae
RUN git clone --no-checkout https://github.com/libjpeg-turbo/libjpeg-turbo libjpeg-turbo
RUN git -C libjpeg-turbo checkout dfde1f857d5bdc0ff214c5f920f04ea8287005ed
RUN git clone --no-checkout https://github.com/facebook/zstd zstd
RUN git -C zstd checkout 216099a73f6ec19c246019df12a2877dada45cca

WORKDIR leptonica
COPY --chown=hopper:hopper ${TARGET}/hopper_build.sh /home/
COPY hopper_build-addon.sh /home/
RUN cat /home/hopper_build-addon.sh >> /home/hopper_build.sh
COPY --chown=hopper:hopper hopper_fuzz.sh /home/
COPY --chown=hopper:hopper hopper_sanitize.py /home/

RUN /home/hopper_build.sh
ENTRYPOINT ["/bin/bash", "/home/hopper_fuzz.sh"]

