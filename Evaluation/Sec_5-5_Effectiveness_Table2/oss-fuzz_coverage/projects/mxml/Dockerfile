
FROM gcr.io/oss-fuzz-base/base-builder
RUN git clone --no-checkout https://github.com/michaelrsweet/mxml
RUN git -C mxml checkout d650587449d9d43e2703674d9fcd04217ab53b4e

COPY build.sh $SRC/
COPY fuzz_loadfile.c $SRC/

COPY new_fuzzers/ $SRC/new_fuzzers/

WORKDIR $SRC/mxml
