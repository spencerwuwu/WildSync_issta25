

FROM gcr.io/oss-fuzz-base/base-builder
RUN git clone --no-checkout https://github.com/ibireme/yyjson
RUN git -C yyjson checkout ce22cdbabb0542078cbdfe56dc4759629836abcb

WORKDIR yyjson
COPY build.sh fuzzer.c $SRC/
COPY new_fuzzers/ $SRC/new_fuzzers
