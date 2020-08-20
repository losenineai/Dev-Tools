#!/bin/sh


function arfun()  
{  
    # echo "arg1 = $1"  
    FILE_NAME1="$1"


    FILE_DIR=$(dirname  $FILE_NAME1) 

    FILE_NAME="${FILE_NAME1##*/}"
    cd $FILE_DIR

    ar -x $FILE_NAME
    for file in ./*.o
    do 
    	 mv $file "$RANDOM$RANDOM$RANDOM".o
    done
    
    rm $FILE_NAME
    libtool -static -o $FILE_NAME *.o
    echo $FILE_DIR/$FILE_NAME
}  




FILE_ALL_NAME="$1"
echo $FILE_ALL_NAME

FILE_DIR=$(dirname  $FILE_ALL_NAME) 
echo $FILE_DIR

FILE_NAME="${FILE_ALL_NAME##*/}"
echo $FILE_NAME

rm -rf $FILE_DIR/x86_64
rm -rf $FILE_DIR/i386
rm -rf $FILE_DIR/arm64
rm -rf $FILE_DIR/armv7

if [ ! -d "$FILE_DIR/x86_64/" ];then
mkdir -p $FILE_DIR/x86_64
fi

if [ ! -d "$FILE_DIR/i386/" ];then
mkdir -p $FILE_DIR/i386
fi

if [ ! -d "$FILE_DIR/arm64/" ];then
mkdir -p $FILE_DIR/arm64
fi

if [ ! -d "$FILE_DIR/armv7/" ];then
mkdir -p $FILE_DIR/armv7
fi


lipo $FILE_ALL_NAME -thin x86_64 -output $FILE_DIR/x86_64/${FILE_NAME}
lipo $FILE_ALL_NAME -thin i386 -output   $FILE_DIR/i386/${FILE_NAME}
lipo $FILE_ALL_NAME -thin arm64 -output  $FILE_DIR/arm64/${FILE_NAME}
lipo $FILE_ALL_NAME -thin armv7 -output  $FILE_DIR/armv7/${FILE_NAME}

# echo $FILE_DIR/x86_64/$FILE_NAME
x86_64=`arfun "$FILE_DIR/x86_64/$FILE_NAME"`
i386=`arfun "$FILE_DIR/i386/$FILE_NAME"`
arm64=`arfun "$FILE_DIR/arm64/$FILE_NAME"`
armv7=`arfun "$FILE_DIR/armv7/$FILE_NAME"`
# echo $x86


rm $FILE_ALL_NAME

lipo -create -output $FILE_ALL_NAME $FILE_DIR/i386/${FILE_NAME} $FILE_DIR/x86_64/${FILE_NAME} $FILE_DIR/arm64/${FILE_NAME} $FILE_DIR/armv7/${FILE_NAME}
#lipo -create -output lib${TARGET}.a license/x86_64/lib${TARGET}.a license/i386/lib${TARGET}.a license/arm64/lib${TARGET}.a license/armv7/lib${TARGET}.a

rm -rf $FILE_DIR/arm64
rm -rf $FILE_DIR/i386
rm -rf $FILE_DIR/armv7
rm -rf $FILE_DIR/x86_64





