#!/bin/bash

if [ $# != 3 ]
then
    echo "./test.sh apks/path output.apk license_key"
    echo "eg: /test.sh so_protector xxx.so xxx_output.so"
    exit
fi

_soprotector=$1
_sopath=$2
_sooutput=$3

cp -r $_sopath $_sooutput


checksign=`java -jar ../CheckSignApk/SignApk.jar ../CheckSignApk/CERT.RSA`


#echo "../src/upx.out --android-shlib -1  --android-check-sign=0x"$checksign" lib/$dir/$file"
$_soprotector --android-shlib $_sooutput --android-check-integrity --anti-ida #--android-check-sign=0x"$checksign" --anti-ida #--android-phdrs-erase # #  # 


