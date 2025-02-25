
FROM oss-fuzz-hopper

ARG TARGET

# ENVs
ENV HOPPER_RESULTS_DIR=/home/results/krb5_hopper
ENV BASE="/home/krb5/"
ENV LIB_PATH="${BASE}/src/include"

ENV DEPS_LDD=${BASE}/
ENV HEADER=${BASE}/src/include/krb5.h
ENV SHARED_LIB=${BASE}/src/lib/libkrb5.so

####################

RUN apt-get update && apt-get install -y autoconf bison

WORKDIR /home
USER hopper:hopper

RUN git clone --no-checkout https://github.com/krb5/krb5.git
RUN git -C krb5 checkout 2063e72ca296a55768c071fb70727ff4688c3496

WORKDIR krb5

COPY --chown=hopper:hopper ${TARGET}/hopper_build.sh /home/
COPY hopper_build-addon.sh /home/
RUN cat /home/hopper_build-addon.sh >> /home/hopper_build.sh
COPY --chown=hopper:hopper hopper_fuzz.sh /home/
COPY --chown=hopper:hopper hopper_sanitize.py /home/

COPY --chown=hopper:hopper ${TARGET}/setenv.sh /home/

RUN /home/hopper_build.sh
ENTRYPOINT ["/bin/bash", "/home/hopper_fuzz.sh"]
