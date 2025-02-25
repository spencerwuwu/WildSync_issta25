
FROM oss-fuzz-hopper

ARG TARGET

# ENVs
ENV HOPPER_RESULTS_DIR=/home/results/libgd_hopper
ENV BASE=/home/libgd/

ENV HEADER=${BASE}/src/gd.h
ENV SHARED_LIB=${BASE}/src/.libs/libgd.so
ENV LIB_PATH="${BASE}/src/"

####################
RUN apt-get update && \
    apt-get install -y make autoconf automake libtool pkg-config libz-dev

WORKDIR /home
USER hopper:hopper

RUN git clone --no-checkout https://github.com/libgd/libgd
RUN git -C libgd checkout b3190785bbe1d1cf768773474debcb58efcdba42

WORKDIR libgd
COPY --chown=hopper:hopper ${TARGET}/hopper_build.sh /home/
COPY hopper_build-addon.sh /home/
RUN cat /home/hopper_build-addon.sh >> /home/hopper_build.sh
COPY --chown=hopper:hopper hopper_fuzz.sh /home/
COPY --chown=hopper:hopper hopper_sanitize.py /home/

RUN /home/hopper_build.sh
ENTRYPOINT ["/bin/bash", "/home/hopper_fuzz.sh"]

