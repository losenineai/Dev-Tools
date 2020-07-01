#!/bin/sh

set -xe

ROOT_DIR=$(cd `dirname $0`; pwd)

mkdir -p $ROOT_DIR/anti-as-project/demo/src/main/cpp/dx-anti
mkdir -p $ROOT_DIR/anti-as-project/demo/src/main/cpp/dx-hook
mkdir -p $ROOT_DIR/anti-as-project/demo/src/main/cpp/dx-libc

cp -rf $ROOT_DIR/anti/anti-no-crash/* $ROOT_DIR/anti-as-project/demo/src/main/cpp/dx-anti/
cp -rf dx-hook/* $ROOT_DIR/anti-as-project/demo/src/main/cpp/dx-hook/
cp -rf dx-libc/* $ROOT_DIR/anti-as-project/demo/src/main/cpp/dx-libc/
# cp -rf $ROOT_DIR/anti-src/dx-hook/* $ROOT_DIR/anti-as-project/demo/src/main/cpp/dx-hook/


(cd anti-as-project
./gradlew :demo:clean
./gradlew :demo:assembleRelease
)

if [ -d apks ]; then
    rm -rf apks
fi

mkdir apks
cp anti-as-project/demo/build/outputs/apk/release/*.apk apks