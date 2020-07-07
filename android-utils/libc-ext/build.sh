#!/bin/bash

set -xe

# 379192    orig
# 391416    -xse=100,-obf=0,-bcf=0,-fla=0,-sub=0,-spli=0
# 424184    -xse=100,-obf=0,-bcf=100,-fla=0,-sub=0,-spli=0
# 436472    -xse=100,-obf=0,-bcf=0,-fla=100,-sub=0,-spli=0
# 391416    -xse=100,-obf=0,-bcf=0,-fla=0,-sub=100,-spli=0
# 391416    -xse=100,-obf=0,-bcf=0,-fla=0,-sub=0,-spli=100
# 526584    -xse=100,-obf=0,-bcf=0,-fla=100,-sub=0,-spli=100

export STEEC_EXTRA_OPTS="-xse=100,-obf=50,-level=simple"
# export STEEC_EXTRA_OPTS="-xse=100,-bcf=50,-fla=50,-sub=30,-spli=30"

root_dir=`dirname $0`

(
    cd "$root_dir"/dx-libc-src/jni
    ndk-build clean
    ndk-build
)
