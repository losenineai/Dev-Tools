#!/bin/sh


check_vm(){
	filename=$1
	isExist=`nm -g $filename | grep '__..$..$_.$__.$_.._$..$._$_.$__.'`
	echo $isExist
	if [ "$isExist"x == ""x ];then
            echo "$filename is not obfuscated"
            exit 1
    else
            echo "$filename is obfuscated"
    fi
}


check_vm $1
