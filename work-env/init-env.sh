#!/bin/bash
set -e



function white(){
	getInt
	echo "getInt:" $?
	str=$(getStr)ssssss
	echo "test:" $str
}

function getStr(){
	echo "aaa"
}

function getInt(){
	return 123
}

function test_equals(){
	var1=teststr

	if [[ "$var1" == *est* ]];then
		echo equals1
	fi

	if [ "$var1" == "teststr" ];then
		echo equals2
	fi
}

function test_path_cut(){
	path=./aaa/bbb/ccc.txt
	dir=${path%.*}
	suf=${path##*.}
	name=$(basename $path .txt)

	echo path:$path
	echo dir:$dir
	echo name:$name
	echo suf:$suf
}


# 输入参数
echo 输入参数个数 $#

test_equals
test_path_cut

