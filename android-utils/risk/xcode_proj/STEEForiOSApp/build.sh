#!/bin/sh

set -x
rm -rf build

mkdir build

xcodebuild archive -project STEEForiOSApp.xcodeproj -sdk iphoneos -scheme STEEForiOSAppStaticFrameworkTest  -configuration Release -archivePath ./build/Demo

xcodebuild -exportArchive -archivePath ./build/Demo.xcarchive -exportPath ./build/exportBuild  -exportOptionsPlist ./build/Demo.xcarchive/Info.plist


cp ./build/exportBuild/STEEForiOSAppStaticFrameworkTest.ipa ../TestBuild/

