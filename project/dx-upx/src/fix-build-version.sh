#!/bin/bash
version=$1
filename=version.h

sed -e "s/tagversion/$version/" $filename > $filename"_temp"
ret=$?

if [ $ret != 0 ]; then
    echo "fix version tag error"
    if [ -f $filename"_temp" ]; then
        rm $filename"_temp"
    fi
    exit 1
fi

time=$(date "+%Y%m%d-%H%M%S")
sed -e "s/buildtime/$time/" $filename"_temp" > $filename
ret=$?

if [ -f $filename"_temp" ]; then
    rm $filename"_temp"
fi

if [ $ret != 0 ]; then
    echo "fix build time error"
    exit 1
fi

exit 0