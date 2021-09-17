#!/bin/bash
#rm -rf build
#mkdir build
cmake -B build -S . -DCMAKE_INSTALL_PREFIX=/usr \
      -DCMAKE_BUILD_TYPE=Debug \
      -DKDE_INSTALL_LIBDIR=lib -DKDE_INSTALL_USE_QT_SYS_PATHS=ON \
      -DCMAKE_EXPORT_COMPILE_COMMANDS=ON

cmake --build build/

sudo cmake --install build/

killall plasmashell && kstart5 plasmashell
