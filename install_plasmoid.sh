#!/bin/bash
#rm -rf build
#mkdir build
cmake -S . -B build \
      -DCMAKE_INSTALL_PREFIX=/usr \
      -DCMAKE_BUILD_TYPE=Debug \
      -DCMAKE_INSTALL_LIBEXECDIR=lib

cmake --build build/

sudo cmake --install build/

# killall plasmashell && kstart plasmashell
plasmashell --replace
