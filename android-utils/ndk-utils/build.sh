#/bin/sh
set -ex

NDK_BUILD=ndk-build

$NDK_BUILD NDK_PROJECT_PATH=src NDK_APPLICATION_MK=src/Application.mk APP_BUILD_SCRIPT=src/Android.mk NDK_LIBS_OUT=build NDK_OUT=build