#!/bin/bash

if [ $# != 2 ]
then
    echo "./test.sh apks/path output.apk license_key"
    echo "eg: /test.sh ../src/upx.out output_libs/ "
    exit
fi

_soprotector=$1
_libs_path=$2


#echo $_apkpath" is a dir"
if [ ! -d $_libs_path ]
then
    echo "can not find libs dir"
    exit 1
fi


for apknames in `ls $_libs_path`
do
    for dirname in `ls $_libs_path/$apknames`
    do
        for file in `ls $_libs_path/$apknames/$dirname`
        do
            filepath=$_libs_path$apknames/$dirname/$file
            echo $filepath
            if [ $apknames == "upx-test.apk" ]
            then
                $_soprotector --android-shlib $filepath
            else
                $_soprotector $filepath --android-check-integrity --android-check-sign=0xA28BA208 --android-shlib --anti-ida --android-anti-dump --android-anti-debug
                # $_soprotector $filepath --android-shlib --fix-sdk-build
                # apkProtect $_soprotector $file $_key
            fi
        done
    done
done



