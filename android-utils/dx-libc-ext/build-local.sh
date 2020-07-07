#!/bin/bash
# set -xe

stee_name="stee-build-env"
root_dir=`dirname $0`
cd $root_dir
path=`pwd`

echo $path

isexist=`docker ps | grep $stee_name`
if [ x"$isexist" != x ]; then
    docker stop $stee_name
fi

isexist=`docker ps -a | grep $stee_name`
if [ x"isexist" != x ]; then
    docker rm $stee_name
fi

docker run --name $stee_name -it -v $path:/root/dxlibc dev.dingxiang-inc.com:4567/stee/dockers/dx-ndk:r15c ./root/dxlibc/build.sh

