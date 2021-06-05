#!/bin/sh

set -xe

export STEEC_EXTRA_OPTS="-xse=100,-stee=100,-obf=100,-level=normal"
#export STEEC_EXTRA_OPTS="-xse,-bcf=100,-fla=100,-sub=100,-spli=100"

ROOT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
TARGET_DIR=${ROOT_DIR}/out

# create out dirs
cd ${ROOT_DIR}
for arch in "armeabi" "armeabi-v7a" "arm64-v8a" "x86"; do
    mkdir -p ${TARGET_DIR}/libs/${arch}
done

# copy head file
cd ${ROOT_DIR}
cp crypto-headers/* ${TARGET_DIR}/

# compile armv7/arm64/x86 by substrate
cd ${ROOT_DIR}/crypto-build/jni
#stee-ndk-build
ndk-build

# copy general .a to out
cp ../obj/local/armeabi-v7a/libDxCrypto.a ${TARGET_DIR}/libs/armeabi/
for arch in "armeabi-v7a" "arm64-v8a" "x86"; do
    cp ../obj/local/${arch}/libDxCrypto.a ${TARGET_DIR}/libs/${arch}/
done
