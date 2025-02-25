
FROM oss-fuzz-hopper

ARG TARGET

# ENVs
ENV HOPPER_RESULTS_DIR=/home/results/libplacebo_hopper
ENV BASE=/home/libplacebo/

ENV HEADER=${BASE}/src/shaders.h
ENV SHARED_LIB=${BASE}/build/src/libplacebo.so
ENV LIB_PATH="${BASE}/src:${BASE}/src/include/:${BASE}/src/include/libplacebo/:${BASE}/build/src:${BASE}/build/src/include"

####################

RUN apt-get update && apt-get install -y python3-pip pkg-config && \
    pip3 install meson ninja

WORKDIR /home
USER hopper:hopper

RUN git clone --no-checkout https://code.videolan.org/videolan/libplacebo.git libplacebo
RUN git -C libplacebo checkout 1fd3c7bde7b943fe8985c893310b5269a09b46c5

WORKDIR libplacebo

COPY --chown=hopper:hopper ${TARGET}/hopper_build.sh /home/
COPY hopper_build-addon.sh /home/
RUN cat /home/hopper_build-addon.sh >> /home/hopper_build.sh
COPY --chown=hopper:hopper hopper_fuzz.sh /home/
COPY --chown=hopper:hopper hopper_sanitize.py /home/

RUN /home/hopper_build.sh
#ENTRYPOINT ["/bin/bash", "/home/hopper_fuzz.sh"]
