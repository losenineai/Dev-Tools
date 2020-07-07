#!/bin/bash

set -xe

root_dir=`dirname $0`

rm -rf $root_dir/dx-libc-src/libs/*
rm -rf $root_dir/dx-libc-src/obj/*
rm -rf $root_dir/dist/*
