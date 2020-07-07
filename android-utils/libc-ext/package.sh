#!/bin/bash
set -ex

root_dir=`dirname $0`

(
    cd "$root_dir"
    ./build.sh
)

if [ -r "$root_dir"/dist ];then
    rm -r "$root_dir"/dist
fi

mkdir "$root_dir"/dist

cp "$root_dir"/dx-libc-src/jni/dx_libc_prefix.h "$root_dir"/dist/

cp "$root_dir"/dx-libc-src/jni/dx_libc.h "$root_dir"/dist/

abis=(
    "armeabi"
    "armeabi-v7a"
    "arm64-v8a"
    "x86"
    "x86_64"
)

mkdir "$root_dir"/dist/libs

for abi in ${abis[@]}
do
    mkdir "$root_dir"/dist/libs/"$abi"
    if [ $abi == "armeabi" ];then
        cp "$root_dir"/dx-libc-src/obj/local/armeabi-v7a/*.a "$root_dir"/dist/libs/"$abi"/
    else
        cp "$root_dir"/dx-libc-src/obj/local/"$abi"/*.a "$root_dir"/dist/libs/"$abi"/
    fi
done