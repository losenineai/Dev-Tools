#!/bin/bash
# -*- coding: utf-8 -*-

# set -x

# echo count $#
# echo 1 $1
# echo 2 $2
# echo 3 $3
# echo 4 $4

# if [ $1 == "e" ]; then exit; fi

# $(realpath $0) "e" """$2""" $3
# exit

function help()
{
cat << EOF
**************************************************************************
* Script: $(realpath $0)
*     Binary protector.
*
* Usage:
*    -z    is zip file. depends on '-s'
*    -i    input file path                                  (required)
*    -o    out file path                                    (required)
*    -p    pic dir path
*    -r    out report file path. depends on '-p'
*    -s    so list file path
*    -f    flag list file path                              (required)
*    -t    time check
*    -h    help info
**************************************************************************
EOF
}

tmp_dir=$(pwd)/tmp
function exit_process
{
    if test -d $tmp_dir; then rm -rf $tmp_dir; fi
    exit $1
}

realpath() {
    python -c "import os,sys; print(os.path.realpath(sys.argv[1]))" "$1"
}

# 参数检查
IS_ZIP=false
while [ -n "$1" ] 
do 
    case "$1" in 
        -z)  IS_ZIP=true ;;
        -i)  INPUT=$(realpath "$2")
            shift;;
        -o)  OUTPUT=$(realpath "$2") 
            shift;;
        -p)  PIC=$(realpath "$2") 
            shift;;
        -r)  REPORT=$(realpath "$2") 
            shift;;
        -s)  SOS=$(realpath "$2")
            shift;;
        -f)  FLAGS=$(realpath "$2") 
            shift;;
        -t)  EXPIRE_TIME="$2" 
            shift;;
        \?) help
            exit_process 1 ;;
    esac
    shift
done

# 环境检查
if [ "x${INPUT}" == "x" ]; then 
    echo "Error: Not input -i param." && help && exit_process 1;
elif ! test -f "$INPUT"; then
    echo "Error: Not found -i param: $INPUT." && help && exit_process 1;
fi

if [ "x${OUTPUT}" == "x" ]; then 
    echo "Error: Not input -o param." && help && exit_process 1; 
elif test -f "$OUTPUT"; then
    rm $OUTPUT
fi

if [ "x${FLAGS}" == "x" ]; then 
    echo "Error: Not input -f param." && help && exit_process 1;
elif ! test -s "$FLAGS"; then
    echo "Error: Not found -f param: $FLAGS." && help && exit_process 1;
fi

if [ $IS_ZIP == true ]; then
    if [ "x${SOS}" == "x" ]; then
        echo "Error: Not input -s param." && help && exit_process 1;
    elif ! test -s "$SOS"; then
        echo "Error: Not found -s param: $SOS." && help && exit_process 1;
    fi
fi

if [ "x${REPORT}" != "x" ]; then
    if [ "x${PIC}" == "x" ]; then 
        echo "Error: Not input -p param." && help && exit_process 1; 
    elif ! test -d "$PIC"; then
        echo "Error: Not found -p param: $PIC." && help && exit_process 1;
    fi

    if test -f "$REPORT"; then
        rm $REPORT
    fi
fi

BINARY=$(realpath $(dirname $0)/upx.out) 
if ! test -x $BINARY; then 
    BINARY=$(realpath $(dirname $0)/binary-shell) 
fi
if ! test -x $BINARY; then
    BINARY=$(realpath $(dirname $0)/binary-shell-$(uname))
fi

# create tmp dir
if test -d $tmp_dir; then rm -rf $tmp_dir; fi
mkdir -p $tmp_dir

# get temp_path && so_files
so_files=""
if [ $IS_ZIP == true ]; then
    temp_path="$(dirname $OUTPUT)/temp.zip"
    unzip "$INPUT" -d $tmp_dir >/dev/null 2>&1
    for line in `cat $SOS`; do so_files=$so_files" "$line; done
else
    temp_path="$(dirname $OUTPUT)/temp.so"
    so_files=$temp_path
fi
cp "$INPUT" $temp_path

# get flags
for flag in `cat $FLAGS`; do flags=$flags" "$flag; done

# get other arg
other_cmd=""
if [ "x${REPORT}" != "x" ]; 
then 
    other_cmd=" --report=$REPORT --pic=$PIC";
fi

if [ "x${EXPIRE_TIME}" != "x" ]; 
then 
    other_cmd=$other_cmd" --expire-time=$EXPIRE_TIME";
fi

if [ "x${DX_HOME}" != "x" ];
then
    other_cmd=$other_cmd" --licfile=$DX_HOME/etc/license.key"
fi

cd $tmp_dir

## echo "orig-input = $INPUT"

### do protect
if [ $IS_ZIP == true ]; then
    $BINARY $so_files "--orig=$INPUT" $flags $other_cmd
else
    $BINARY $so_files "--orig=$INPUT" $flags $other_cmd
fi

ret=$?

if [ $ret != "0" ] && [ $ret != "2" ] ; then
    exit_process $ret
fi

# update
if [ $IS_ZIP == true ]; then
    for item in $so_files; do
        echo "item: $item"
        echo "temp_path: $temp_path"
        echo "zip -u $temp_path $item"
        zip -u $temp_path $item
    done
fi

# temp -> output
mv "$temp_path" "$OUTPUT"

if ! test -r "$OUTPUT"; then 
    echo "can not find output file"
    exit_process 1
fi

exit_process 0

