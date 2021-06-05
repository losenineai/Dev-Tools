# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.

cmake_minimum_required(VERSION 3.4.1)

include_directories(${PROJECT_SOURCE_DIR}/src/main/cpp)
include_directories(${PROJECT_SOURCE_DIR}/src/main/cpp/crypto)

aux_source_directory(${PROJECT_SOURCE_DIR}/src/main/cpp native_src)

list(APPEND SRC_LIST ${native_src})

add_library(native-lib SHARED ${SRC_LIST})

add_library(dx_crypto STATIC IMPORTED)
set_target_properties(dx_crypto
  PROPERTIES IMPORTED_LOCATION
  ${PROJECT_SOURCE_DIR}/src/main/cpp/crypto/libs/${ANDROID_ABI}/libDxCrypto.a)

find_library(z_lib z)
find_library(log_lib log)

target_link_libraries(native-lib ${z_lib})
target_link_libraries(native-lib ${log_lib})
target_link_libraries(native-lib dx_crypto)