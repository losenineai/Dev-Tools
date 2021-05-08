#!/bin/bash

# 「ida 调试 Android 步骤」
# pre: 关闭android studio
# 1. adb push ida-server/ida-server64 /data/local/tmp # 调试64进程用ida-server64
# 2. 进入adb shell，su方式启动ida-server
# 3. adb forward tcp:<ida_port> tcp<ida_port>
# 4. adb shell am start -D <pkg>/<activity>
# 5. ida 开始attach 进程，F9
# 6. 最后运行此脚本：ida-debug-apk.sh <pid> # pid指被调试的进程


if [ $# != 1 ] 
then
    echo "please input the <pid>"
    exit 1
fi

pid=$1

adb forward tcp:8800 jdwp:$pid

jdb -connect com.sun.jdi.SocketAttach:hostname=localhost,port=8800
