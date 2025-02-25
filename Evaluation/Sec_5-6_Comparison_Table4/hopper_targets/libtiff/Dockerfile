
FROM oss-fuzz-hopper

ARG TARGET

# ENVs
ENV HOPPER_RESULTS_DIR=/home/results/libtiff_hopper
ENV BASE=/home/libtiff/

#ENV HEADER=${BASE}/libtiff/tiff.h
ENV HEADER=${BASE}/libtiff/tiffio.h
ENV SHARED_LIB=${BASE}/libtiff/libtiff.so
ENV LIB_PATH="${BASE}/libtiff/"

####################

RUN apt-get update && apt-get install -y make autoconf automake libtool cmake nasm
RUN apt-get update && apt-get install -y \
    zlib1g-dev \
    libjbig-dev \
    libjpeg-dev

WORKDIR /home
USER hopper:hopper

RUN git clone --no-checkout https://gitlab.com/libtiff/libtiff
RUN git -C libtiff checkout bb1ff4f80ecb7b7b635fd51fc2ebf04d8f09109a

WORKDIR libtiff

COPY --chown=hopper:hopper ${TARGET}/hopper_build.sh /home/
COPY hopper_build-addon.sh /home/
RUN cat /home/hopper_build-addon.sh >> /home/hopper_build.sh
COPY --chown=hopper:hopper hopper_fuzz.sh /home/
COPY --chown=hopper:hopper hopper_sanitize.py /home/

RUN /home/hopper_build.sh
ENTRYPOINT ["/bin/bash", "/home/hopper_fuzz.sh"]

