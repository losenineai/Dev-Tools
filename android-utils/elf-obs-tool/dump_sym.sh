#!/bin/sh

# set -ex

# TOOL_CHAINS=${ANDROID_NDK_HOME:-"/opt/dx-android-ndk"}/toolchains
# ARM_TOOL_BIN=$TOOL_CHAINS/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/arm-linux-androideabi/bin
# ARM64_TOOL_BIN=$TOOL_CHAINS/aarch64-linux-android-4.9/prebuilt/linux-x86_64/aarch64-linux-android/bin
# X86_TOOL_BIN=$TOOL_CHAINS/x86-4.9/prebuilt/linux-x86_64/i686-linux-android/bin

TOOL_CHAINS=~/Work/Android/android-ndk-r15c/toolchains
ARM_TOOL_BIN=$TOOL_CHAINS/arm-linux-androideabi-4.9/prebuilt/darwin-x86_64/arm-linux-androideabi/bin
ARM64_TOOL_BIN=$TOOL_CHAINS/aarch64-linux-android-4.9/prebuilt/darwin-x86_64/aarch64-linux-android/bin
X86_TOOL_BIN=$TOOL_CHAINS/x86-4.9/prebuilt/darwin-x86_64/i686-linux-android/bin


SRC_A=$1
OUT_FILE=$2
TMP=./tmp_o

# set cmd by abi
if [[ "$SRC_A" =~ "armeabi" ]] || [[ "$SRC_A" =~ "armeabi-v7a" ]]; then
    TOOL_BIN=$ARM_TOOL_BIN
elif [[ "$SRC_A" =~ "arm64-v8a" ]]; then
    TOOL_BIN=$ARM64_TOOL_BIN
elif [[ "$SRC_A" =~ "x86" ]]; then
    TOOL_BIN=$X86_TOOL_BIN
fi
nm=$TOOL_BIN/nm
ar=$TOOL_BIN/ar


rm -fr $TMP
mkdir -p $TMP


# extract .a to .o
cd $TMP
$ar x ../$SRC_A
cd ..

# output all o symbol
for o_file in $TMP/*.o; do
	name=$(basename $o_file .o)

	# get define symbol from o
	`echo `$nm --defined-only $o_file`` > tmp.txt
	# echo $all_print

	cat tmp.txt | while read line; do
		# echo $line
		sym=`echo $line | cut -d' ' -f3`
		if [ "$sym" != "" ];then
			echo $sym
			echo $sym >> $OUT_FILE
		fi
	done

	rm -fr tmp.txt

done


rm -fr $TMP









