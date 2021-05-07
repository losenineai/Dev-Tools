#!/bin/bash
UCL_PATH=`pwd`/ucl
echo $UCL_PATH
make -C src UPX_UCLDIR=$UCL_PATH
