#!/bin/bash

cmake .. \
      -G Ninja \
      -DCMAKE_BUILD_TYPE=Debug \
      -DCMAKE_LIBRARY_PATH:FILEPATH="$CONDA_PREFIX/lib" \
      -DCMAKE_INCLUDE_PATH:FILEPATH="$CONDA_PREFIX/include" \
      -DCMAKE_FIND_FRAMEWORK="NEVER" \
      -DCMAKE_INSTALL_PREFIX=${CONDA_PREFIX} \
      -DBUILD_PLUGIN_I3S=ON \
      -DBUILD_PLUGIN_NITF=ON \
      -DBUILD_PLUGIN_TILEDB=ON \
      -DBUILD_PLUGIN_ICEBRIDGE=ON \
      -DBUILD_PLUGIN_HDF=ON \
      -DBUILD_PLUGIN_PGPOINTCLOUD=ON \
      -DBUILD_PLUGIN_E57=ON \
      -DBUILD_PLUGIN_DRACO=ON \
      -DBUILD_PGPOINTCLOUD_TESTS=OFF \
      -DWITH_LAZPERF=ON \
      -DWITH_LASZIP=ON \
      -DWITH_ZSTD=ON \
      -DWITH_ZLIB=ON \
      -DWITH_TESTS=ON
