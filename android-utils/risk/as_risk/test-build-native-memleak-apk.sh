#!/bin/bash
##需要使用ndk-15版本编译，否则得更换jniLibs下对应版本的so

set -ex

curDir=$(cd `dirname $0` && pwd)

(
    cd $curDir
    cp -r sanitizer-native-leak-test/jniLibs ../dx-risk/src/main/
    rm -fr ../dx-risk/src/main/jniLibs/arm64-v8a
    rm -fr ../dx-risk/src/main/jniLibs/x86
    rm -fr ../dx-risk/src/main/jniLibs/x86_64
    cp -r sanitizer-native-leak-test/resources ../dx-risk/src/main/
    rm -fr ../dx-risk/src/main/resources/lib/arm64-v8a
    rm -fr ../dx-risk/src/main/resources/lib/x86
    rm -fr ../dx-risk/src/main/resources/lib/x86_64

    cp sanitizer-native-leak-test/Android.mk ../dx-risk/src/main/cpp/Android.mk
    cp sanitizer-native-leak-test/Application.mk ../dx-risk/src/main/cpp/Application.mk
)

(
    cd $curDir"/.."
    ./gradlew :app:clean && ./gradlew :app:assembleDebug
    mkdir -p $curDir"/../sdk-testin-apk"
    cp $curDir"/../app/build/outputs/apk/debug/app-debug.apk" $curDir"/../sdk-testin-apk/native-memleak-test.apk"
)