#!/bin/bash
set -xe

root_dir=`dirname $0`
cd $root_dir

mkdir -p src/linux
if [ ! -f src/linux/libucl.a ];
    cd ucl-1.03
    ./configure
    make
    cd ..
    cp ucl-1.03/src/.libs/libucl.a src/linux/
fi