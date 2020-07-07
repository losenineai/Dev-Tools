#!/bin/sh

set -xe

export STEEC_EXTRA_OPTS="-xse,-bcf=100,-fla=100,-sub=100"

ROOT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
TARGET_DIR=${ROOT_DIR}/out

# create out dirs
cd ${ROOT_DIR}
mkdir ${TARGET_DIR}
mkdir ${TARGET_DIR}/libs
mkdir ${TARGET_DIR}/libs/armeabi
mkdir ${TARGET_DIR}/libs/armeabi-v7a
mkdir ${TARGET_DIR}/libs/x86
mkdir ${TARGET_DIR}/libs/arm64-v8a

# copy head file
cd ${ROOT_DIR}
cp headers/* ${TARGET_DIR}/

# compile armv7/arm64/x86 by substrate
cd ${ROOT_DIR}/build/jni
stee-ndk-build -e "USE_STEE_NDK=true"
# ndk-build

cd ${ROOT_DIR}/build

cp -R obj/local/armeabi/*.a ${TARGET_DIR}/libs/armeabi/
cp -R obj/local/armeabi-v7a/*.a ${TARGET_DIR}/libs/armeabi-v7a/
cp -R obj/local/arm64-v8a/*.a ${TARGET_DIR}/libs/arm64-v8a/
cp -R obj/local/x86/*.a ${TARGET_DIR}/libs/x86/

# rm stee tmp file
rm -fr ${TARGET_DIR}/libs/armeabi/*stee*
rm -fr ${TARGET_DIR}/libs/armeabi-v7a/*stee*
rm -fr ${TARGET_DIR}/libs/arm64-v8a/*stee*
rm -fr ${TARGET_DIR}/libs/x86/*stee*

# # 虚机编译不过
# # compile x86_64 by hookzz
# cd ${ROOT_DIR}/HookZz
# #stee-ndk-build  NDK_PROJECT_PATH=. APP_BUILD_SCRIPT=./Android.mk APP_ABI=arm64-v8a
# ndk-build  NDK_PROJECT_PATH=. APP_BUILD_SCRIPT=./Android.mk APP_ABI=arm64-v8a
# cp obj/local/arm64-v8a/libDxHook.a ${TARGET_DIR}/libs/arm64-v8a/
