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
OUT=$2
# OUT=./out
# SRC_A=./src/libDXLock_2.1.1.a
TMP_SYM=$OUT/dump_sym.txt
OUT_MAPPING=$OUT/sym_mapping.txt

# set cmd by abi
if [[ "$SRC_A" =~ "armeabi" ]] || [[ "$SRC_A" =~ "armeabi-v7a" ]]; then
    TOOL_BIN=$ARM_TOOL_BIN
elif [[ "$SRC_A" =~ "arm64-v8a" ]]; then
    TOOL_BIN=$ARM64_TOOL_BIN
elif [[ "$SRC_A" =~ "x86" ]]; then
    TOOL_BIN=$X86_TOOL_BIN
fi
ar=$TOOL_BIN/ar
objcopy=$TOOL_BIN/objcopy
echo objcopy:$objcopy


rm -fr $TMP_SYM
# dump sym
./dump_sym.sh $SRC_A $TMP_SYM


OUT_A=$OUT/$(basename $SRC_A)
cp $SRC_A $OUT_A

echo "\n\n------------ $OUT_A : " >> $OUT_MAPPING

while read line; 
do
	old_sym=$line

	# symbol no work in black list
	b_black=false
	while read black;
	do
		# old_sym 包含 black
		if [[ $old_sym =~ $black ]];then
			echo ----- black list: $old_sym
			b_black=true
			break
		fi
	done < ./sym_black.cfg
	if [[ $b_black == true ]]; then
		continue
	fi

	# generate new symbol
    # new_sym=dx.`python ../get_md5.py $old_sym`
    new_sym=`python ./random_symbol.py $old_sym`
    mapping=`echo "$old_sym = $new_sym"`
    echo $mapping >> $OUT_MAPPING
    # do redefine sym from a
	$objcopy --redefine-sym $old_sym=$new_sym $OUT_A $OUT_A
	echo finished redefine sym: $mapping
done < $TMP_SYM

rm -fr $TMP_SYM
