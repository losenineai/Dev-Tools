# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.

cmake_minimum_required(VERSION 3.4.1)

set(CRYPTO_DIR ${PROJECT_SOURCE_DIR}/src/main/cpp/crypto)
set(CRYPTO_DIR2 ${PROJECT_SOURCE_DIR}/src/main/cpp/crypto)

include_directories(${PROJECT_SOURCE_DIR}/src/main/cpp)
include_directories(${CRYPTO_DIR}/)
include_directories(${CRYPTO_DIR}/dx_headers)
include_directories(${CRYPTO_DIR}/dx_tools)
include_directories(${CRYPTO_DIR}/base64)
include_directories(${CRYPTO_DIR}/md5)
include_directories(${CRYPTO_DIR}/aes)
include_directories(${CRYPTO_DIR}/mt)
include_directories(${CRYPTO_DIR}/sha1)
include_directories(${CRYPTO_DIR}/sha256)
include_directories(${CRYPTO_DIR}/sm3)
include_directories(${CRYPTO_DIR}/sm4)
include_directories(${CRYPTO_DIR}/rc4)
include_directories(${CRYPTO_DIR}/wbaes)
include_directories(${CRYPTO_DIR}/xxtea)

aux_source_directory(${CRYPTO_DIR}/dx_api       crypto_src)
aux_source_directory(${CRYPTO_DIR}/dx_tools     crypto_src)
aux_source_directory(${CRYPTO_DIR}/base64       crypto_src)
aux_source_directory(${CRYPTO_DIR}/md5          crypto_src)
aux_source_directory(${CRYPTO_DIR}/aes          crypto_src)
aux_source_directory(${CRYPTO_DIR}/mt           crypto_src)
aux_source_directory(${CRYPTO_DIR}/sha1         crypto_src)
aux_source_directory(${CRYPTO_DIR}/sha256       crypto_src)
aux_source_directory(${CRYPTO_DIR}/sm3          crypto_src)
aux_source_directory(${CRYPTO_DIR}/sm4          crypto_src)
aux_source_directory(${CRYPTO_DIR}/rc4          crypto_src)
aux_source_directory(${CRYPTO_DIR}/wbaes        crypto_src)
aux_source_directory(${CRYPTO_DIR}/xxtea        crypto_src)

aux_source_directory(${PROJECT_SOURCE_DIR}/src/main/cpp native_src)

list(APPEND SRC_LIST ${native_src} ${crypto_src} )

add_library(native-lib SHARED ${SRC_LIST})

find_library(z_lib z)
find_library(log_lib log)

target_link_libraries(native-lib ${z_lib})
target_link_libraries(native-lib log)