
FROM gcr.io/oss-fuzz-base/base-builder
RUN git clone --depth 1 https://github.com/LMDB/lmdb
WORKDIR lmdb
COPY build.sh lmdb_fuzzer.c $SRC/
COPY new_fuzzers/ $SRC/new_fuzzers/
