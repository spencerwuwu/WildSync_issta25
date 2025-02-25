#!/bin/bash -eu

# Disable logging via library build configuration control.
cat scripts/pnglibconf.dfa | \
  sed -e "s/option STDIO/option STDIO disabled/" \
      -e "s/option WARNING /option WARNING disabled/" \
      -e "s/option WRITE enables WRITE_INT_FUNCTIONS/option WRITE disabled/" \
> scripts/pnglibconf.dfa.temp
mv scripts/pnglibconf.dfa.temp scripts/pnglibconf.dfa

# build the libpng library.
autoreconf -f -i
./configure --with-libpng-prefix=OSS_FUZZ_ --enable-tests=no
make -j$(nproc) clean
make -j$(nproc)
