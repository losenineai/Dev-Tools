#/bin/sh
set -e

# TODO 根据编译环境设置
CMAKE=$ANDROID_HOME/cmake/3.10.2.4988404/bin/cmake
CMAKE_TOOLCHAIN=$ANDROID_HOME/ndk/21.3.6528147/build/cmake/android.toolchain.cmake

rm -fr build-android
mkdir -p build-android
cd build-android

rm -fr libs
mkdir -p libs

abis=("armeabi-v7a" "arm64-v8a" "x86" "x86_64")
for abi in ${abis[*]};
do
	echo ------------- start build $abi

	$CMAKE -DCMAKE_TOOLCHAIN_FILE=$CMAKE_TOOLCHAIN -DCMAKE_BUILD_TYPE=Release -DANDROID_ABI=$abi -DANDROID_PLATFORM=android-16 .. && $CMAKE --build .

	name=libgstcp.so
	mkdir libs/$abi
	mv $name libs/$abi/$name
	
	make clean
	
	echo ------------- $abi finished
	echo
	echo
done
