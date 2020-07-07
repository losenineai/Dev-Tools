#!/bin/bash

set -xe

# 379192    orig
# 391416    -xse=100,-obf=0,-bcf=0,-fla=0,-sub=0,-spli=0
# 424184    -xse=100,-obf=0,-bcf=100,-fla=0,-sub=0,-spli=0
# 436472    -xse=100,-obf=0,-bcf=0,-fla=100,-sub=0,-spli=0
# 391416    -xse=100,-obf=0,-bcf=0,-fla=0,-sub=100,-spli=0
# 391416    -xse=100,-obf=0,-bcf=0,-fla=0,-sub=0,-spli=100
# 526584    -xse=100,-obf=0,-bcf=0,-fla=100,-sub=0,-spli=100

export STEEC_EXTRA_OPTS="-xse=100,-obf=100,-level=simple"
#export STEEC_EXTRA_OPTS="-xse,-bcf=100,-fla=100,-sub=100,-spli=100"

ROOT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

cp -r dx-hook anti-src/
cp -r dx-libc anti-src/
mkdir -p anti

function dx_crash() {

    target_dir=anti/$1

    # create out dirs
    mkdir -p ${target_dir}/libs
    (cd anti/$1/libs
    mkdir -p "armeabi" "armeabi-v7a" "arm64-v8a" "x86" "x86_64")

    # copy head file
    cp anti-headers/dx_anti.h ${target_dir}/dx_anti.h

    ndk-build clean -C anti-build/jni
    ndk-build -C anti-build/jni   -e "USE_STEE_NDK=true" $2 

    # copy general .a to out
    cp anti-build/obj/local/armeabi-v7a/libDxAnti.a ${target_dir}/libs/armeabi/
    for arch in "armeabi-v7a" "arm64-v8a" "x86" "x86_64"; do
        cp  anti-build/obj/local/${arch}/libDxAnti.a ${target_dir}/libs/${arch}/
    done

}

dx_crash anti-crash
dx_crash anti-no-crash "NO_CRASH=true"
