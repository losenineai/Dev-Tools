#!/bin/sh

ARCH_ABIS=(armeabi-v7a arm64-v8a x86)

SRC_DIR=./src
OUT_DIR=./out

rm -fr $OUT_DIR

for abi in ${ARCH_ABIS[*]};
do
	echo abi:$abi
	mkdir -p $OUT_DIR/$abi
	for a in $SRC_DIR/$abi/*.a;
	do
		echo a:$a
		./sym_obscure_4_a.sh $a $OUT_DIR/$abi
	done
done
