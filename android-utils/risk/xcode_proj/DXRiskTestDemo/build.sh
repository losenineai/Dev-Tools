#!/bin/sh

set -x
rm -rf build

mkdir build

xcodebuild archive -project DXRiskTestDemo.xcodeproj -sdk iphoneos -scheme DXRiskTestDemo -configuration Release -archivePath ./build/Demo
xcodebuild -exportArchive -archivePath ./build/Demo.xcarchive -exportPath ./build/exportBuild  -exportOptionsPlist ./build/Demo.xcarchive/Info.plist


cp ./build/exportBuild/DXRiskTestDemo.ipa ../TestBuild/
