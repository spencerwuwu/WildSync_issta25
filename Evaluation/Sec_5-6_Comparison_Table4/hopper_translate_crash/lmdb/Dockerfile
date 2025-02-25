
FROM oss-fuzz-hopper

ARG TARGET

# ENVs
ENV HOPPER_RESULTS_DIR=/home/results/lmdb_hopper
ENV BASE=/home/lmdb/

ENV HEADER=${BASE}/libraries/liblmdb/lmdb.h
ENV SHARED_LIB=${BASE}/libraries/liblmdb/liblmdb.so
ENV LIB_PATH="${BASE}/libraries:${BASE}/libraries/liblmdb"

####################


WORKDIR /home
USER hopper:hopper

RUN git clone --no-checkout https://github.com/LMDB/lmdb
RUN git -C lmdb checkout 88d0531d3ec3a592cdd63ca77647d31c568c24bc

WORKDIR lmdb

COPY --chown=hopper:hopper ${TARGET}/hopper_build.sh /home/
COPY hopper_build-addon.sh /home/
RUN cat /home/hopper_build-addon.sh >> /home/hopper_build.sh
COPY --chown=hopper:hopper hopper_fuzz.sh /home/
COPY --chown=hopper:hopper hopper_sanitize.py /home/

RUN /home/hopper_build.sh
ENTRYPOINT ["/bin/bash", "/home/hopper_fuzz.sh"]

