#!/bin/sh

set -xe

# for dxn in "dx-crypto" "dx-hook"; do
#     ./dx-pull.sh $dxn anti-src/$dxn
# done 

./dx-pull.sh dx-hook dx-hook

# for arch in "armeabi" "armeabi-v7a" "arm64-v8a" "x86"; do
# #     mv anti-src/dx-crypto/libs/$arch/libDxCrypto*.a anti-src/dx-crypto/libs/$arch/libDxCrypto.a 
#     mv anti-src/dx-hook/libs/$arch/libDxHook*.a anti-src/dx-hook/libs/$arch/libDxHook.a 
# done
 