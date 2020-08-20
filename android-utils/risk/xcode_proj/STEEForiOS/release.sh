#!/bin/sh

#  buildscript.sh
#  protobuf
#
#  Created by echonn on 17-07-04.
#  Copyright (c) 2017年 dingxiang. All rights reserved.

BUILDPACKAGE_STARTTIME=$(date +%s)

if [[ "$*" == *"-v"* ]]; then
set -x
rm -rf build/
fi

#默认源路径
echo "----------------------ENVIRONMENT VALUABLES----------------------"

if [ "$(dirname $0)" == "." ]; then
    SRCROOT=$PWD
else
    SRCROOT=$(cd $(dirname $0) && pwd)
fi

echo "SRCROOT = $SRCROOT"

XCODE4_PATH="/Users/dingxiang/Desktop/Xcode/Xcode.10.0.0.app"
#XCODE4_PATH="/Applications/Xcode.app"

#xcode编译环境
XCODEBUILD_PATH="$XCODE4_PATH/Contents/Developer/usr/bin/xcodebuild"

export DEVELOPER_DIR="$XCODE4_PATH/Contents/Developer"

#真机build生成的头文件的文件夹路径
DEVICE_DIR_INCLUDE=${SRCROOT}/DXRisk/ios/
echo "DEVICE_DIR_INCLUDE = ${DEVICE_DIR_INCLUDE}"

#检测设置编译使用SDK路径
SDKPath=""
if [ -d "$XCODE4_PATH/Contents/Developer/Platforms/iPhoneOS.platform/Developer/SDKs" ]; then
    SDKPath="$XCODE4_PATH/Contents/Developer/Platforms/iPhoneOS.platform/Developer/SDKs"
elif [ -d "/Developer/Platforms/iPhoneOS.platform/Developer/SDKs" ]; then
    SDKPath="/Developer/Platforms/iPhoneOS.platform/Developer/SDKs"
fi
echo "SDKPath = $SDKPath"
SDKVERSION=$(ls $SDKPath|sort -r|tr '[A-Z]' '[a-z]'|head -n 1)
SDKVERSION=${SDKVERSION:0:${#SDKVERSION}-4}
echo "SDKVERSION = $SDKVERSION"

SDKPath_S=""
if [ -d "$XCODE4_PATH/Contents/Developer/Platforms/iPhoneSimulator.platform/Developer/SDKs" ]; then
    SDKPath_S="$XCODE4_PATH/Contents/Developer/Platforms/iPhoneSimulator.platform/Developer/SDKs"
elif [ -d "/Developer/Platforms/iPhoneSimulator.platform/Developer/SDKs" ]; then
    SDKPath_S="/Developer/Platforms/iPhoneSimulator.platform/Developer/SDKs"
fi
echo "SDKPath_S = $SDKPath_S"
SDKVERSION_S=$(ls $SDKPath_S|sort -r|tr '[A-Z]' '[a-z]'|head -n 1)
SDKVERSION_S=${SDKVERSION_S:0:${#SDKVERSION_S}-4}
echo "SDKVERSION_S = $SDKVERSION_S"
echo "----------------------ENVIRONMENT VALUABLES----------------------"


STEE_VERSION=$(cat ${DEVICE_DIR_INCLUDE}/DXRisk.h | grep "#define DXRISK_SDK_VERSION" | awk -F'"' '{print $2}')

echo "Current STEE version: \033[32m${STEE_VERSION}\033[m"

#检测git uncommitted changes
GIT_VERSION=$(git rev-parse --short HEAD)
echo "Current git revision: \033[32m${GIT_VERSION}\033[m"

git diff-index --quiet HEAD --


MODULE=""
SUBPATH=""
DYMODULE=""
FRAMEWORK_MODULE=""
#DYSUBPATH=""
MODULE="STEERiskMgrForiOS"
DYMODULE="DXRisk"
SUBPATH="DXRisk"
FRAMEWORK_MODULE="DXRiskStatic"

echo "1. Compiling $MODULE..."
OUTDIR="$SRCROOT/out/$SUBPATH"
if [ -e "$OUTDIR" ]; then
    rm -rf $OUTDIR
fi
mkdir -p "$OUTDIR/header"

function build_error()
{
	echo "\033[31mError occured while building, please check $OUTDIR/build.log\033[m"
	exit 1;
}

ARCH_IPHONEOS="-arch armv7 -arch arm64"
ARCH_SIMULATOR="-arch i386 -arch x86_64"

LOGLOCATION="$OUTDIR/build.log"
if [[ "$*" == *"-v"* ]]; then
LOGLOCATION="/dev/stdout"
fi

echo "1.1 Compiling libstee for iphoneos ..."
xcodebuild -toolchain dx-vm${CI_TEST_TOOLCHAIN_SUFFIX} -target ${DYMODULE} -sdk $SDKVERSION $ARCH_IPHONEOS -configuration Release >> $LOGLOCATION || build_error
xcodebuild -toolchain dx-vm${CI_TEST_TOOLCHAIN_SUFFIX} -target ${FRAMEWORK_MODULE} -sdk $SDKVERSION $ARCH_IPHONEOS -configuration Release >> $LOGLOCATION || build_error

echo "1.2 Compiling libstee for iphonesimulator ..."
xcodebuild -toolchain dx-vm${CI_TEST_TOOLCHAIN_SUFFIX} -target ${DYMODULE} -sdk $SDKVERSION_S $ARCH_SIMULATOR -configuration Release >> $LOGLOCATION || build_error
xcodebuild -toolchain dx-vm${CI_TEST_TOOLCHAIN_SUFFIX} -target ${FRAMEWORK_MODULE} -sdk $SDKVERSION_S $ARCH_SIMULATOR -configuration Release >> $LOGLOCATION || build_error

if [ "$MODULE" == "STEERiskMgrForiOS" ]; then
echo "1.3 Compiling libstee-with-idfa for iphoneos ..."
xcodebuild -toolchain dx-vm${CI_TEST_TOOLCHAIN_SUFFIX} -target ${DYMODULE}WithIDFA -sdk $SDKVERSION $ARCH_IPHONEOS -configuration Release >> $LOGLOCATION || build_error
xcodebuild -toolchain dx-vm${CI_TEST_TOOLCHAIN_SUFFIX} -target ${FRAMEWORK_MODULE}WithIDFA -sdk $SDKVERSION $ARCH_IPHONEOS -configuration Release >> $LOGLOCATION || build_error

echo "1.4 Compiling libstee-with-idfa for iphonesimulator ..."
xcodebuild -toolchain dx-vm${CI_TEST_TOOLCHAIN_SUFFIX} -target ${DYMODULE}WithIDFA -sdk $SDKVERSION_S $ARCH_SIMULATOR -configuration Release >> $LOGLOCATION || build_error
xcodebuild -toolchain dx-vm${CI_TEST_TOOLCHAIN_SUFFIX} -target ${FRAMEWORK_MODULE}WithIDFA -sdk $SDKVERSION_S $ARCH_SIMULATOR -configuration Release >> $LOGLOCATION || build_error
fi

echo "Done"

#创建framework目录
echo "create framework dic"
if [ -d "$OUTDIR/${DYMODULE}.framework" ]
then
rm -rf "$OUTDIR/${DYMODULE}.framework"
fi
mkdir -p "$OUTDIR/${DYMODULE}.framework"
cp -r "$SRCROOT/build/Release-iphoneos/${DYMODULE}.framework/" "$OUTDIR/${DYMODULE}.framework"
rm -f "$OUTDIR/${DYMODULE}.framework/${DYMODULE}"
rm -f "$OUTDIR/${DYMODULE}.framework/Headers/DXRiskManager.h"

if [ "$MODULE" == "STEERiskMgrForiOS" ]; then
if [ -d "$OUTDIR/${DYMODULE}WithIDFA.framework" ]
then
rm -rf "$OUTDIR/${DYMODULE}WithIDFA.framework"
fi
mkdir -p "$OUTDIR/${DYMODULE}WithIDFA.framework"
cp -r "$SRCROOT/build/Release-iphoneos/${DYMODULE}WithIDFA.framework/" "$OUTDIR/${DYMODULE}WithIDFA.framework"
rm -f "$OUTDIR/${DYMODULE}WithIDFA.framework/${DYMODULE}WithIDFA"
rm -f "$OUTDIR/${DYMODULE}WithIDFA.framework/Headers/DXRiskManager.h"
fi

#创建framework目录
echo "create framework dic"
if [ -d "$OUTDIR/${FRAMEWORK_MODULE}.framework" ]
then
rm -rf "$OUTDIR/${FRAMEWORK_MODULE}.framework"
fi
mkdir -p "$OUTDIR/${FRAMEWORK_MODULE}.framework"
cp -r "$SRCROOT/build/Release-iphoneos/${FRAMEWORK_MODULE}.framework/" "$OUTDIR/${FRAMEWORK_MODULE}.framework"
rm -f "$OUTDIR/${FRAMEWORK_MODULE}.framework/${FRAMEWORK_MODULE}"
rm -f "$OUTDIR/${FRAMEWORK_MODULE}.framework/Headers/DXRiskManager.h"

if [ "$MODULE" == "STEERiskMgrForiOS" ]; then
if [ -d "$OUTDIR/${FRAMEWORK_MODULE}WithIDFA.framework" ]
then
rm -rf "$OUTDIR/${FRAMEWORK_MODULE}WithIDFA.framework"
fi
mkdir -p "$OUTDIR/${FRAMEWORK_MODULE}WithIDFA.framework"
cp -r "$SRCROOT/build/Release-iphoneos/${FRAMEWORK_MODULE}WithIDFA.framework/" "$OUTDIR/${FRAMEWORK_MODULE}WithIDFA.framework"
rm -f "$OUTDIR/${FRAMEWORK_MODULE}WithIDFA.framework/${FRAMEWORK_MODULE}WithIDFA"
rm -f "$OUTDIR/${FRAMEWORK_MODULE}WithIDFA.framework/Headers/DXRiskManager.h"
fi



echo "2. Copy header files to $OUTDIR/header"

# 复制头文件到目标文件夹的
cp "${DEVICE_DIR_INCLUDE}/DXRiskManager.h" "$OUTDIR/${DYMODULE}.framework/Headers"
cp "${DEVICE_DIR_INCLUDE}/DXRiskManager.h" "$OUTDIR/${DYMODULE}WithIDFA.framework/Headers"

cp "${DEVICE_DIR_INCLUDE}/DXRiskManager.h" "$OUTDIR/${FRAMEWORK_MODULE}.framework/Headers"
cp "${DEVICE_DIR_INCLUDE}/DXRiskManager.h" "$OUTDIR/${FRAMEWORK_MODULE}WithIDFA.framework/Headers"
echo "Done"

echo "3. Merge iphone and simulator static libraries" 
#合成模拟器和真机.a包
PACKAGENAME="libDXRisk"

lipo -create "$SRCROOT/build/Release-iphoneos/${DYMODULE}.framework/${DYMODULE}" "$SRCROOT/build/Release-iphonesimulator/${DYMODULE}.framework/${DYMODULE}" -output "$OUTDIR/${DYMODULE}.framework/${DYMODULE}"
lipo -create "$SRCROOT/build/Release-iphoneos/${FRAMEWORK_MODULE}.framework/${FRAMEWORK_MODULE}" "$SRCROOT/build/Release-iphonesimulator/${FRAMEWORK_MODULE}.framework/${FRAMEWORK_MODULE}" -output "$OUTDIR/${FRAMEWORK_MODULE}.framework/${FRAMEWORK_MODULE}"
if [ "$MODULE" == "STEERiskMgrForiOS" ]; then
lipo -create "$SRCROOT/build/Release-iphoneos/${DYMODULE}WithIDFA.framework/${DYMODULE}WithIDFA" "$SRCROOT/build/Release-iphonesimulator/${DYMODULE}WithIDFA.framework/${DYMODULE}WithIDFA" -output "$OUTDIR/${DYMODULE}WithIDFA.framework/${DYMODULE}WithIDFA"
lipo -create "$SRCROOT/build/Release-iphoneos/${FRAMEWORK_MODULE}WithIDFA.framework/${FRAMEWORK_MODULE}WithIDFA" "$SRCROOT/build/Release-iphonesimulator/${FRAMEWORK_MODULE}WithIDFA.framework/${FRAMEWORK_MODULE}WithIDFA" -output "$OUTDIR/${FRAMEWORK_MODULE}WithIDFA.framework/${FRAMEWORK_MODULE}WithIDFA"
STEE_IDFA_DY="${DYMODULE}WithIDFA.framework"
STEE_IDFA_ST="${FRAMEWORK_MODULE}WithIDFA.framework"
else
STEE_IDFA_DY=""
STEE_IDFA_ST=""
fi
echo "Done" 

echo "4. Clean Up"

rm -rf "$SRCROOT/../DXRiskPocDemo/DXRisk.framework"
cp -rf "$OUTDIR/${DYMODULE}.framework" "$SRCROOT/../DXRiskPocDemo/"
# rm -rf "$SRCROOT/build"
echo "Done"

echo "fix static file O list"
./build_fix.sh ${OUTDIR}/${FRAMEWORK_MODULE}.framework/${FRAMEWORK_MODULE}
./build_fix.sh ${OUTDIR}/${FRAMEWORK_MODULE}WithIDFA.framework/${FRAMEWORK_MODULE}WithIDFA


echo "5. pack sdk files"
cd ${OUTDIR}

TARGET_DEMO_FILE=dx-risk-demo.zip
zip -r $TARGET_DEMO_FILE ../../../DXRiskPocDemo

TARGET_FILE=dx-risk-iOS-${STEE_VERSION}-${GIT_VERSION}.zip
zip -r $TARGET_FILE   ${DYMODULE}.framework $STEE_IDFA_DY ${FRAMEWORK_MODULE}.framework $STEE_IDFA_ST


cd - >> /dev/null
echo "Done"

echo "Finish Build using: \033[31m$(($(date +%s)-$BUILDPACKAGE_STARTTIME))s\033[m"
echo "STEE SDK released at \033[32m${OUTDIR}/${TARGET_FILE}\033[m"
