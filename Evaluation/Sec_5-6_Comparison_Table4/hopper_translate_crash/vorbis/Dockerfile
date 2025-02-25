
FROM oss-fuzz-hopper

ARG TARGET

# ENVs
ENV HOPPER_RESULTS_DIR=/home/results/vorbis_hopper
ENV BASE=/home/vorbis/

ENV HEADER=${BASE}/include/vorbis/vorbisfile.h
ENV SHARED_LIB=${BASE}/lib/.libs/libvorbis.so
ENV LIB_PATH="${BASE}/lib:${BASE}/include:${BASE}/"

####################

RUN apt-get update && apt-get install -y make autoconf automake ffmpeg libtool pkg-config python3 wget
RUN python3 -m pip install corpus-replicator

RUN apt-get install -y libogg-dev

WORKDIR /home
USER hopper:hopper

ENV WORK=/home/work

#RUN git clone --no-checkout https://gitlab.xiph.org/xiph/ogg.git
#RUN git -C ogg checkout db5c7a49ce7ebda47b15b78471e78fb7f2483e22

RUN git clone --no-checkout https://gitlab.xiph.org/xiph/vorbis.git
RUN git -C vorbis checkout 84c023699cdf023a32fa4ded32019f194afcdad0

WORKDIR vorbis

COPY --chown=hopper:hopper ${TARGET}/hopper_build.sh /home/
COPY hopper_build-addon.sh /home/
RUN cat /home/hopper_build-addon.sh >> /home/hopper_build.sh
COPY --chown=hopper:hopper hopper_fuzz.sh /home/
COPY --chown=hopper:hopper hopper_sanitize.py /home/

RUN /home/hopper_build.sh
ENTRYPOINT ["/bin/bash", "/home/hopper_fuzz.sh"]

