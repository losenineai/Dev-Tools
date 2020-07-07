# set -x
ROOT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
TARGET_DIR=${ROOT_DIR}/../../utils/dx-inner-sdk/dx-hook/temp

cd ${ROOT_DIR}
rm -rf ${TARGET_DIR}
rm -rf substrate/obj
rm -rf hookzz/obj

rm -rf demo/obj/ demo/libs/

rm -rf out