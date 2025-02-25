
#FROM gcr.io/oss-fuzz-base/base-builder
FROM oss-fuzz-hopper

RUN mkdir -p /home
ENV SRC=/home
WORKDIR /home

RUN git clone --no-checkout https://github.com/LMDB/lmdb
RUN git -C lmdb checkout 88d0531d3ec3a592cdd63ca77647d31c568c24bc

WORKDIR lmdb
COPY build.sh $SRC/
COPY hopper_helper.py $SRC/

COPY hopper_results/ $SRC/lmdb/hopper_results/

# casr
RUN bash $SRC/build.sh
ENTRYPOINT ["/bin/bash", "/home/process_casr.sh"]
