#!/bin/bash

if [ $# != 5 ]
then
    echo "./test.sh apks/path output.apk platform license_key"
    echo "eg: ./pull_libs.sh apks/ output_libs/ apks_output/ platform rightkey.jks"
    exit
fi

_apkspath=$1
_libspath=$2
_apkoutput=$3
_platform=$4
_key=$5

pullLibs(){
    apkpath=$1
    libpath=$2
    apkoutput=$3
    apkname=$4
    key=$5
    keypass=""
    
    echo $apkpath
    echo $libpath
    echo $apkoutput
    echo $apkname 
    
    if [ "$key" == "rightkey.jks" ]
    then
        keypass="myckey"
    elif [ "$key" == "wrongkey.jks" ]
    then
        keypass="anotherkey"
    else
        echo "please input rightkey.jks or wrongkey.jks"
        exit
    fi

    cp $apkpath $apkoutput$apkname
    cp -r $libpath $apkoutput"lib"

    (
        cd $apkoutput 
        zip -r $apkname lib/*
        rm -r lib
    )

    java -jar apksigner.jar sign --v1-signing-enabled --v2-signing-enabled --ks=$key --ks-pass=pass:$keypass --ks-key-alias=key0 --key-pass=pass:$keypass --out=$apkoutput$apkname $apkoutput$apkname #)>/dev/null    

}


#echo $_apkpath" is a dir"

for file in `ls $_libspath`
do
    if [ $file == "upx-test.apk" ]
    then
        pullLibs "test-case-apks/upx-test.apk" $_libspath"upx-test.apk" $_apkoutput $_platform"_"$file  $_key
    else
        echo "apkpath file : "$_apkspath$file
        pullLibs $_apkspath$file  $_libspath$file  $_apkoutput  $_platform"_"$file  $_key
    fi
done

