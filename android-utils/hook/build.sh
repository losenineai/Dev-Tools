#!/bin/sh

set -xe

export STEEC_EXTRA_OPTS="-xse=100,-obf=100,-level=simple" # r15c
# export STEEC_EXTRA_OPTS="-xse,-bcf=100,-fla=100,-sub=100,-spli=100" # r15

ROOT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
TARGET_DIR=${ROOT_DIR}/out

# create out dirs
cd ${ROOT_DIR}
# for arch in "armeabi" "armeabi-v7a" "arm64-v8a" "x86" "x86_64"; do
for arch in "armeabi" "armeabi-v7a" "arm64-v8a" "x86" "x86_64"; do
    mkdir -p ${TARGET_DIR}/libs/${arch}
done

# copy head file
cd ${ROOT_DIR}
cp headers/* ${TARGET_DIR}/

# compile armv7/arm64/x86 by substrate
cd ${ROOT_DIR}/substrate
ndk-build
#stee-ndk-build
cp obj/local/armeabi-v7a/libDxHook.a ${TARGET_DIR}/libs/armeabi/
cp obj/local/armeabi-v7a/libDxHook.a ${TARGET_DIR}/libs/armeabi-v7a/
cp obj/local/x86/libDxHook.a ${TARGET_DIR}/libs/x86/
cp obj/local/x86_64/libDxHook.a ${TARGET_DIR}/libs/x86_64/

# Hookzz 虚机编译不过
# compile x86_64 by hookzz
cd ${ROOT_DIR}/HookZz
ndk-build  NDK_PROJECT_PATH=. APP_BUILD_SCRIPT=./Android.mk APP_ABI=arm64-v8a
#stee-ndk-build  NDK_PROJECT_PATH=. APP_BUILD_SCRIPT=./Android.mk APP_ABI=arm64-v8a
cp obj/local/arm64-v8a/libDxHook.a ${TARGET_DIR}/libs/arm64-v8a/
