#!/bin/bash

_apkpath=$1
_apkoutput=$2

if [ $# != 2 ]
then
    echo "./extract_lib.sh apks_path/ libs_output_path/"
    echo "eg: ./extract_lib.sh apks/ output_libs/"
    exit
fi

libsExtract(){
    apkpath=$1
    apkoutput=$2
 
    platforms=(
        'armeabi-v7a'  'x86' 'armeabi' 'arm64-v8a'
    )


    echo "apkpath : $keypass"

    echo $apkpath 
    unzip $apkpath lib/*  

    for dir in `ls lib`
    do
        index=0
        for pf in ${platforms[@]}
        do
            if [ "$dir" == "$pf" ]
            then
                #echo $dir
                index=1
            fi
        done

        if [ $index == 0 ]
        then
            echo "remove $dir"
            rm -r lib/$dir
            zip -d $apkoutput lib/$dir/*
        fi
    done

    mv lib $apkoutput/

}





#echo $_apkpath" is a dir"
if [ -d $_apkoutput ]
then
    rm -r $_apkoutput
fi
mkdir $_apkoutput

if [ $TRIGGER_VALUE ]; then 
    lists=(
        #'upx-test.apk'
        # 'app-debug.apk'
        'aikucun-2.6.8.apk' ##must
        'bishijie.apk'  ##must
        'com_superkor_glpy.apk'
        'Joywok_v2.0.2.2_20181204_saic.apk'
        'huisuoping-3.9.6.apk'
        'snjr-6.6.2-release.apk'  ##must
        'aizipai.apk' 
        'lifangtong.apk'
        'midun-app-release20180822.apk'
        'yunmanman.apk'  ##must
        'migu.apk'
        'dianrong.apk'
        'kuaidi_debug.apk'
        'wutiao_ori.apk'
        'youli.apk'
        'youxian.apk'
        'zhihuribao_795.apk'  ##must
        # 'zhongtong.apk'
        'hnnxmbank-1.0.0-no_verify.apk'
        'hangyunduoduo.apk'
    )
else
    lists=(
        #'upx-test.apk'
        # 'app-debug.apk'
        'aikucun-2.6.8.apk' ##must
        'bishijie.apk'  ##must
        # 'com_superkor_glpy.apk'
        # 'Joywok_v2.0.2.2_20181204_saic.apk'
        # 'huisuoping-3.9.6.apk'
        'snjr-6.6.2-release.apk'  ##must
        # 'aizipai.apk' 
        # 'lifangtong.apk'
        # 'midun-app-release20180822.apk'
        'yunmanman.apk'  ##must
        # 'migu.apk'
        # 'dianrong.apk'
        # 'kuaidi_debug.apk'
        # 'wutiao_ori.apk'
        # 'youli.apk'
        # 'youxian.apk'
        'zhihuribao_795.apk'  ##must
        # 'zhongtong.apk'
        # 'hnnxmbank-1.0.0-no_verify.apk'
        # 'hangyunduoduo.apk'
    )
fi

for file in `ls $_apkpath`
do  
    index=0
    for list in ${lists[@]}
    do
        if [ "$file" == $list ]
        then
            echo "find : "$file
            index=1
        fi
    done
    if [ $index == 1 ]
    then
        libsExtract $_apkpath/$file $_apkoutput/$file
    fi
done

libsExtract "test-case-apks/upx-test.apk" $_apkoutput/"upx-test.apk"


