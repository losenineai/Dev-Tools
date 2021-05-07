#!/bin/bash

# set -xe

root_dir=`dirname $0`

cd $root_dir

root_dir=`pwd`

if [ ! -d bin-upx-20160918 ]; then
    xz -d bin-upx-20160918.tar.xz
    tar xvf bin-upx-20160918.tar
fi

export PATH="$PATH:$root_dir/bin-upx-20160918"

(
    cd src/stub/src
    make
)