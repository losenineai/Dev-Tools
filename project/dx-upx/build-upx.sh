#!/bin/bash
# set -xe

root_dir=`dirname $0`
cd $root_dir

mkdir -p src/ucl
if [ ! -f src/ucl/libucl.a ]; then
    cd ucl-1.03
    ./configure
    make
    cd ..
    cp ucl-1.03/src/.libs/libucl.a src/ucl/
fi

cd src
make all