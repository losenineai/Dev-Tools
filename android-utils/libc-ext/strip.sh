#!/bin/bash

#set -xe

# ./clean.sh
# ./build.sh

ROOT_DIR=$(cd `dirname $0`; pwd)
# NAME_FILE=${ROOT_DIR}/anti-build/nameList.txt
LIBRARY_NAME=libDxLibc.a

# TOOL_CHAINS=/Users/white/Work/Android/android-ndk-r15c/toolchains
# ARM_TOOL_BIN=$TOOL_CHAINS/arm-linux-androideabi-4.9/prebuilt/darwin-x86_64/arm-linux-androideabi/bin
# ARM64_TOOL_BIN=$TOOL_CHAINS/aarch64-linux-android-4.9/prebuilt/darwin-x86_64/aarch64-linux-android/bin
# X86_TOOL_BIN=$TOOL_CHAINS/x86-4.9/prebuilt/darwin-x86_64/i686-linux-android/bin

TOOL_CHAINS=${ANDROID_NDK_HOME:-"/opt/dx-android-ndk"}/toolchains
ARM_TOOL_BIN=$TOOL_CHAINS/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/arm-linux-androideabi/bin
ARM64_TOOL_BIN=$TOOL_CHAINS/aarch64-linux-android-4.9/prebuilt/linux-x86_64/aarch64-linux-android/bin
X86_TOOL_BIN=$TOOL_CHAINS/x86-4.9/prebuilt/linux-x86_64/i686-linux-android/bin


function dxlibc_strip() {
    
    target_dir=${ROOT_DIR}/$1/libs

    for arch in "armeabi" "armeabi-v7a" "arm64-v8a" "x86"; do

        if [ -d "${target_dir}/$arch" ] && [ -f "${target_dir}/$arch/$LIBRARY_NAME" ];then
            cd "${target_dir}/$arch"
        else
            continue
        fi

        if [ "$arch" = "armeabi" ] || [ "$arch" = "armeabi-v7a" ]; then
            TOOL_BIN=$ARM_TOOL_BIN
        elif [ "$arch" = "arm64-v8a" ]; then
            TOOL_BIN=$ARM64_TOOL_BIN
        elif [ "$arch" = "x86" ]; then
            TOOL_BIN=$X86_TOOL_BIN
        fi

        echo "= = = = = = = = = = " $arch " = = = = = = = = = ="

        (
            $TOOL_BIN/next-ar -x $LIBRARY_NAME
            rm -rf $LIBRARY_NAME
            
            dir=$(ls)
            for i in $dir
            do
                $TOOL_BIN/strip --strip-debug $i
                $TOOL_BIN/strip --strip-unneeded $i
                
            done   
            
            $TOOL_BIN/next-ar crv $LIBRARY_NAME *.o
            rm -rf *\.o
        )
    done
}

dxlibc_strip dist

