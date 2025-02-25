

#FROM gcr.io/oss-fuzz-base/base-builder
FROM oss-fuzz-hopper

RUN mkdir -p /home
ENV SRC=/home
WORKDIR /home

RUN git clone --no-checkout https://github.com/ibireme/yyjson
RUN git -C yyjson checkout ce22cdbabb0542078cbdfe56dc4759629836abcb

WORKDIR yyjson

COPY build.sh $SRC/
COPY hopper_helper.py $SRC/

COPY hopper_results/ $SRC/yyjson/hopper_results/

# casr
RUN bash $SRC/build.sh
ENTRYPOINT ["/bin/bash", "/home/process_casr.sh"]
