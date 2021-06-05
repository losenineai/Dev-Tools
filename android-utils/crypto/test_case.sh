#!/bin/sh

set -xe

ROOT_DIR=$(cd `dirname $0`; pwd)

cp -r $ROOT_DIR/out/* $ROOT_DIR/crypto-as-project/demo/src/main/cpp/crypto

(
cd crypto-as-project
./gradlew :demo:assembleRelease
)

mkdir apks
cp crypto-as-project/demo/build/outputs/apk/release/*.apk apks