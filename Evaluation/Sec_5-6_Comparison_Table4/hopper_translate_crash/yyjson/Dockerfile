
FROM oss-fuzz-hopper

ARG TARGET

# ENVs
ENV HOPPER_RESULTS_DIR=/home/results/yyjson_hopper
ENV BASE=/home/yyjson/

ENV HEADER=${BASE}/src/yyjson.h
ENV SHARED_LIB=${BASE}/build/libyyjson.so
ENV LIB_PATH="${BASE}/src/"

####################


WORKDIR /home
USER hopper:hopper

RUN git clone --no-checkout https://github.com/ibireme/yyjson
RUN git -C yyjson checkout ce22cdbabb0542078cbdfe56dc4759629836abcb

WORKDIR yyjson

COPY --chown=hopper:hopper ${TARGET}/hopper_build.sh /home/
COPY hopper_build-addon.sh /home/
RUN cat /home/hopper_build-addon.sh >> /home/hopper_build.sh
COPY --chown=hopper:hopper hopper_fuzz.sh /home/
COPY --chown=hopper:hopper hopper_sanitize.py /home/

RUN /home/hopper_build.sh
ENTRYPOINT ["/bin/bash", "/home/hopper_fuzz.sh"]

