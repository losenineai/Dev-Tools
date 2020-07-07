#!/bin/bash
dirpath=$1
if [ x"$dirpath" == x ]; then
    dirpath="logout/test/"
fi

#### root检测
for file in `ls $dirpath`
do
    ret=`echo "$file" | grep Google_Pixel`  ##inject && root
    if [ x"$ret" != x ];then
        # echo "[$file] detecting root "
        ret=`cat "$dirpath/$file" | grep detect-root`
        if [ x"$ret" == x ];then
            echo "[warnning] $file can not detect root"
            #exit 1
        fi 

    fi 
done


#### inject检测
for file in `ls $dirpath`
do
    ret=`echo "$file" | grep Google_Pixel`  ##inject && root
    if [ x"$ret" != x ];then
        # echo "[$file] detecting inject "
        ret=`cat "$dirpath/$file" | grep detect-inject`
        if [ x"$ret" == x ];then
            echo "[warnning] $file can not detect inject"
            #exit 1
        fi 
    fi 
done


#### 日志过滤检测
for file in `ls $dirpath`
do
    ret=`cat "$dirpath/$file" | grep  ===detect-log`
    if [ x"$ret" != x ];then
        echo "[warnning] $file can not filter log"
        # exit 1
    fi 
done


#### 模拟器检测
for file in `ls $dirpath`
do
    ret=`echo "$file" | grep motorola` 
    if [ x"$ret" != x ];then
        ret=`cat "$dirpath/$file" | grep  detect-emulator`
        if [ x"$ret" != x ];then
            echo "[warnning] $file can not detect emulator"
            # exit 1
        fi
    fi
done


