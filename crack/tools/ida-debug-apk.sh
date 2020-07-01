#!/bin/bash

if [ $# != 1 ] 
then
    echo "please input the <pid>"
    exit 1
fi

pid=$1

adb forward tcp:8800 jdwp:$pid

jdb -connect com.sun.jdi.SocketAttach:hostname=localhost,port=8800
