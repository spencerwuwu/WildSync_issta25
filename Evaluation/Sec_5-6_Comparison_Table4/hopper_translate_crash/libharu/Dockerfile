
FROM oss-fuzz-hopper

ARG TARGET

# ENVs
ENV HOPPER_RESULTS_DIR=/home/results/libharu_hopper
ENV BASE=/home/libharu/

#ENV HEADER=${BASE}/libtiff/tiff.h
ENV HEADER=${BASE}/include/hpdf.h
ENV SHARED_LIB=${BASE}/./build/src/libhpdf.so
ENV LIB_PATH="${BASE}/include/:${BASE}/win32/include/"

####################

RUN apt-get update && \
    apt-get install -y --no-install-recommends \
        build-essential \
        make libtool pkg-config gtk-doc-tools autotools-dev automake 
RUN apt-get update && apt-get install -y \
    zlib1g-dev \
    libjpeg-dev

WORKDIR /home
USER hopper:hopper

RUN git clone --no-checkout https://github.com/libharu/libharu.git libharu
RUN git -C libharu checkout 0c598becaadaef8e3d12b883f9fc2864a118c12d

WORKDIR libharu

COPY --chown=hopper:hopper ${TARGET}/hopper_build.sh /home/
COPY hopper_build-addon.sh /home/
RUN cat /home/hopper_build-addon.sh >> /home/hopper_build.sh
COPY --chown=hopper:hopper hopper_fuzz.sh /home/
COPY --chown=hopper:hopper hopper_sanitize.py /home/

RUN /home/hopper_build.sh
ENTRYPOINT ["/bin/bash", "/home/hopper_fuzz.sh"]

