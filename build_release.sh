#!/bin/sh

set -x

cur_dir=`pwd`
build_dir=${build_dir:-./build-release}

mkdir -p $build_dir

cd $build_dir \
    && cmake \
        -DCMAKE_BUILD_TYPE=Release \
        $cur_dir \
    && make -j16 $* \
    && make package
