
FROM oss-fuzz-hopper

ARG TARGET

# ENVs
ENV HOPPER_RESULTS_DIR=/home/results/fribidi_hopper
ENV BASE=/home/fribidi/

ENV HEADER=${BASE}/lib/fribidi.h
ENV SHARED_LIB=${BASE}/build/lib/libfribidi.so
ENV LIB_PATH="${BASE}/lib:${BASE}/build:${BASE}/build/lib:${BASE}/build/gen.tab/:${BASE}/gen.tab/"

####################

RUN apt-get update && apt-get install -y python3-pip pkg-config && \
    pip3 install meson==0.54.0 ninja

WORKDIR /home
USER hopper:hopper

RUN git clone --no-checkout https://github.com/fribidi/fribidi.git
RUN git -C fribidi checkout bfb0385ef862cd40af7d5d33648fbb4bfe8eee65

WORKDIR fribidi
COPY --chown=hopper:hopper ${TARGET}/hopper_build.sh /home/
COPY hopper_build-addon.sh /home/
RUN cat /home/hopper_build-addon.sh >> /home/hopper_build.sh
COPY --chown=hopper:hopper hopper_fuzz.sh /home/
COPY --chown=hopper:hopper hopper_sanitize.py /home/

RUN /home/hopper_build.sh
ENTRYPOINT ["/bin/bash", "/home/hopper_fuzz.sh"]
