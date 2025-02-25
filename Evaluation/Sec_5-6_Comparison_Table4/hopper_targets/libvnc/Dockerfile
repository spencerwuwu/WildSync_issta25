
FROM oss-fuzz-hopper

ARG TARGET

# ENVs
ENV HOPPER_RESULTS_DIR=/home/results/libvnc_hopper
ENV BASE=/home/libvncserver/

ENV HEADER=${BASE}/include/rfb/rfb.h
ENV SHARED_LIB=${BASE}/build/libvncserver.so
ENV LIB_PATH="${BASE}/src/:${BASE}/include/:${BASE}/build/include/"

####################

RUN apt-get update && apt-get install -y cmake zlib1g-dev

WORKDIR /home
USER hopper:hopper

RUN git clone --no-checkout https://github.com/LibVNC/libvncserver.git libvncserver
RUN git -C libvncserver checkout 6e777e6243e01fc7ebc37654ab678c61513bb6c0

WORKDIR libvncserver

COPY --chown=hopper:hopper ${TARGET}/hopper_build.sh /home/
COPY hopper_build-addon.sh /home/
RUN cat /home/hopper_build-addon.sh >> /home/hopper_build.sh
COPY --chown=hopper:hopper hopper_fuzz.sh /home/
COPY --chown=hopper:hopper hopper_sanitize.py /home/

RUN /home/hopper_build.sh
ENTRYPOINT ["/bin/bash", "/home/hopper_fuzz.sh"]

