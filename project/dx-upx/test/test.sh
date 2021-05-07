#!/bin/bash

if [ $# != 4 ]
then
    echo "./test.sh apks/path output.apk license_key"
    echo "eg: /test.sh apks/zhihuribao.zip output.apk myckey.jks"
    exit
fi

_soprotector=$1
_apkpath=$2
_apkoutput=$3
_key=$4

apkProtect(){
    soprotector=$1
    apkpath=$2
    apkoutput=$3
    key=$4
    keypass=""

    platforms=(
        # 'x86_64'
        # 'x86' 
        'arm64-v8a' 
        # 'armeabi' 
        # 'armeabi-v7a'   
    )


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

    echo "$keypass"
    if [ -d lib ]
    then 
        rm -r lib
    fi

    cp $apkpath $apkoutput

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



    checksign=`java -jar ../CheckSignApk/SignApk.jar ../CheckSignApk/CERT.RSA`

    index=0
    for dir in `ls lib`
    do
        for file in `ls lib/$dir`
        do
            if [[ $file == "libmisc-ind.so" || $file == "libJNIAKuCun.so " ]] #|| $file == "libQuCore.so" || $file == " libRSSupport.so" ]] libQuCore-ThirdParty.so
            then
                #echo "../src/upx.out --android-shlib -1  --android-check-sign=0x"$checksign" lib/$dir/$file"
                $soprotector --android-shlib lib/$dir/$file #--android-check-integrity # --android-check-sign=0x"$checksign" #--android-anti-debug #--android-check-integrity # #--android-anti-dump #  --android-check-sign=0x"$checksign" --anti-ida
                echo "pass..."
            else
                echo "pass..."
                $soprotector --android-shlib lib/$dir/$file # --android-check-sign=0x"$checksign"  --expire-time=1560848512 #--anti-ida --android-anti-debug --android-anti-dump #--anti-ida  #--android-anti-debug # --android-anti-dump --android-anti-debug --android-check-integrity 
            fi      
            #--android-check-sign=0x"$checksign" --android-check-integrity
            # echo "do nothing"
            #--fix-sdk-build --anti-ida #--android-anti-dump  --android-check-integrity --android-anti-debug --android-check-sign=0x"$checksign" --anti-ida
        done
        index=$index+1

        if [ $index == 6 ]
        then
            break
        fi

    done


    zip -r $apkoutput lib/*

    java -jar apksigner.jar sign --v1-signing-enabled --v2-signing-enabled --ks=$key --ks-pass=pass:$keypass --ks-key-alias=key0 --key-pass=pass:$keypass --out=$apkoutput $apkoutput #)>/dev/null

}




if [ -f $_apkpath ]
then
    #echo $_apkpath" is a file"
    apkProtect $_soprotector $_apkpath $_apkoutput $_key 
elif [ -d $_apkpath ]
then
    #echo $_apkpath" is a dir"
    if [ -d output ]
    then
        rm -r output
    fi
    mkdir output
    for file in `ls $_apkpath`
    do
        echo $_apkpath$file
        apkProtect $_soprotector $_apkpath$file output/$file $_key
    done
fi


# 7f2966ec8000-7f29671d1000 r-xp 00000000 fd:20 76                         /data/app/com.zhihu.daily.android-1/lib/x86_64/librealm-jni.so
# 7f29671d1000-7f29671e2000 r--p 00308000 fd:20 76                         /data/app/com.zhihu.daily.android-1/lib/x86_64/librealm-jni.so
# 7f29671e2000-7f29671e3000 rw-p 00319000 fd:20 76                         /data/app/com.zhihu.daily.android-1/lib/x86_64/librealm-jni.so


# 7f2966dcc000-7f29670d5000 r-xp 00000000 fd:20 81                         /data/app/com.zhihu.daily.android-2/lib/x86_64/librealm-jni.so
# 7f29670d5000-7f29670e6000 r--p 00308000 fd:20 81                         /data/app/com.zhihu.daily.android-2/lib/x86_64/librealm-jni.so
# 7f29670e6000-7f29670e7000 rw-p 00319000 fd:20 81                         /data/app/com.zhihu.daily.android-2/lib/x86_64/librealm-jni.so



# 7f296606a000-7f296606b000 r-xp 00000000 fd:20 73                         /data/app/com.zhihu.daily.android-2/lib/x86_64/libencrypt.so
# 7f296606b000-7f296606d000 r-xp 00000000 00:00 0
# 7f296606d000-7f296606e000 ---p 00000000 00:00 0
# 7f296606e000-7f296606f000 r--p 00005000 fd:20 73                         /data/app/com.zhihu.daily.android-2/lib/x86_64/libencrypt.so
# 7f296606f000-7f2966070000 rw-p 00006000 fd:20 73                         /data/app/com.zhihu.daily.android-2/lib/x86_64/libencrypt.so


# 7f2966ccb000-7f2966cfd000 r-xp 00000000 fd:20 82                         /data/app/com.zhihu.daily.android-1/lib/x86_64/librealm-jni.so
# 7f2966cfd000-7f2966fd5000 r-xp 00000000 00:00 0
# 7f2966fd5000-7f2966fe6000 r--p 00127000 fd:20 82                         /data/app/com.zhihu.daily.android-1/lib/x86_64/librealm-jni.so
# 7f2966fe6000-7f2966fe7000 rw-p 00138000 fd:20 82                         /data/app/com.zhihu.daily.android-1/lib/x86_64/librealm-jni.so
