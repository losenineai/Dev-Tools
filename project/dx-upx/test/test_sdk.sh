#!/bin/bash

if [ $# != 1 ]
then
    echo "./test_sdk.sh upx_path"
    echo "eg: /test.sh ../src/upx.out"
    exit
fi

_soprotector=$1


so_lists=(
    'testsdks/test1/LIEF/lib/libLIEF.so' 
    'testsdks/test2/jni/libs/arm64-v8a/libDXLock_2.3.0.so'
    'testsdks/test2/jni/libs/armeabi-v7a/libDXLock_2.3.0.so'
    'testsdks/test2/jni/libs/x86/libDXLock_2.3.0.so'
)

for so_file in ${so_lists[@]}
do
    ./$_soprotector $so_file --android-shlib --fix-sdk-build
done

echo "test1..."
(
    cd testsdks/test1
    mkdir build && cd build
    cmake ..
    make
)

echo "test2..."
(
    cd testsdks/test2/jni
    ndk-build
)
