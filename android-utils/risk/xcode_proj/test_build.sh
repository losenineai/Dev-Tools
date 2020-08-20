#!/bin/sh


set -x
rm -rf TestBuild

mkdir TestBuild

rm -rf ./DXRiskTestDemo/DXRiskStatic.framework
cp -rf ./STEEForiOS/out/DXRisk/DXRiskStatic.framework ./DXRiskTestDemo
cp -rf ./STEEForiOS/out/DXRisk/DXRiskStatic.framework ./TestBuild

#check Framework
sh ./DXRiskTestDemo/TestVM.sh ./STEEForiOS/out/DXRisk/DXRiskStatic.framework/DXRiskStatic
sh ./DXRiskTestDemo/TestVM.sh ./STEEForiOS/out/DXRisk/DXRiskWithIDFA.framework/DXRiskWithIDFA
sh ./DXRiskTestDemo/TestVM.sh ./STEEForiOS/out/DXRisk/DXRiskStaticWithIDFA.framework/DXRiskStaticWithIDFA
sh ./DXRiskTestDemo/TestVM.sh ./STEEForiOS/out/DXRisk/DXRisk.framework/DXRisk


#build test app
cd ./STEEForiOSApp && sh build.sh && cd ..
cd ./DXRiskTestDemo && sh build.sh && cd ..



