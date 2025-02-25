
FROM oss-fuzz-hopper

ARG TARGET

# ENVs
ENV HOPPER_RESULTS_DIR=/home/results/libarchive_hopper
ENV BASE=/home/libarchive/

ENV HEADER=${BASE}/libarchive/archive.h
ENV SHARED_LIB=${BASE}/build2/libarchive/libarchive.so
ENV LIB_PATH="${BASE}/libarchive:${BASE}/build2"

####################

RUN apt-get update && apt-get install -y make autoconf libtool pkg-config \
        libbz2-dev liblzo2-dev liblzma-dev liblz4-dev libz-dev \
        libssl-dev libacl1-dev libattr1-dev lrzip liblzo2-dev \
	      liblz4-tool lzop zstd lcab genisoimage jlha-utils rar default-jdk sharutils
RUN curl -LO http://mirrors.kernel.org/ubuntu/pool/main/a/automake-1.16/automake_1.16.5-1.3_all.deb && \
    apt install ./automake_1.16.5-1.3_all.deb

WORKDIR /home
USER hopper:hopper

RUN git clone --no-checkout https://github.com/libarchive/libarchive.git
RUN git -C libarchive checkout 62c8caf6611a7d0662d80176c4fdb40f85794699
#RUN git clone --no-checkout https://gitlab.gnome.org/GNOME/libxml2.git
#RUN git -C libxml2 checkout 5732ce56f3e15644e917161d071938f6e3d82fe8

WORKDIR libarchive
COPY --chown=hopper:hopper ${TARGET}/hopper_build.sh /home/
COPY hopper_build-addon.sh /home/
RUN cat /home/hopper_build-addon.sh >> /home/hopper_build.sh
COPY --chown=hopper:hopper hopper_fuzz.sh /home/
COPY --chown=hopper:hopper hopper_sanitize.py /home/

RUN /home/hopper_build.sh
ENTRYPOINT ["/bin/bash", "/home/hopper_fuzz.sh"]

