#!/usr/bin/sh

# 执行前，先修改 template.log 的堆栈地址，和so名字

ndk-stack -sym obj/armeabi-v7a -dump template.log