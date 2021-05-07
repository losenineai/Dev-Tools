
all: build_arm #build_x86 build_arm build_arm64

#build_x86: clock_check.c
#	gcc -masm=intel -m32 -S clock_check.c -o clock_check_x86.s

build_arm: clock_check.c
	~/local/bin/bin-upx/arm-linux-gcc-4.1.0 -S clock_check.c syscall.S -o clock_check_arm.s

#build_arm64: clock_check.c
#	~/local/bin/bin-upx/arm64-linux-gcc-4.9.2 -S clock_check.c -o clock_check_arm64.s
