# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.

cmake_minimum_required(VERSION 3.4.1)

# Creates and names a library, sets it as either STATIC
# or SHARED, and provides the relative paths to its source code.
# You can define multiple libraries, and CMake builds them for you.
# Gradle automatically packages shared libraries with your APK.

set(AS_PROJECT_ROOT /Users/mt/Documents/dingxiang/git/matao/dx-hook/as_demo)
include_directories(${AS_PROJECT_ROOT}/../include)

set(ZPLATFORM_X86 1)

if(ZPLATFORM_ARMV7A)
    message(armeabi-v7a)
    set(SUBSTRATE_SOURCE_DIR ${AS_PROJECT_ROOT}/../substrate/jni)
    include_directories(${SUBSTRATE_SOURCE_DIR})
    set(HOOK_LIB_SOURCES
        ${SUBSTRATE_SOURCE_DIR}/SubstrateDebug.cpp
        ${SUBSTRATE_SOURCE_DIR}/SubstrateHook.cpp
        ${SUBSTRATE_SOURCE_DIR}/SubstratePosixMemory.cpp
        ${SUBSTRATE_SOURCE_DIR}/hde64.cpp
        ${SUBSTRATE_SOURCE_DIR}/utils.cpp
        )
elseif(ZPLATFORM_X86)
     message(x86)
     set(SUBSTRATE_SOURCE_DIR ${AS_PROJECT_ROOT}/../substrate/jni)
     include_directories(${SUBSTRATE_SOURCE_DIR})
     set(HOOK_LIB_SOURCES
         ${SUBSTRATE_SOURCE_DIR}/SubstrateDebug.cpp
         ${SUBSTRATE_SOURCE_DIR}/SubstrateHook.cpp
         ${SUBSTRATE_SOURCE_DIR}/SubstratePosixMemory.cpp
         ${SUBSTRATE_SOURCE_DIR}/hde64.cpp
         ${SUBSTRATE_SOURCE_DIR}/utils.cpp
         )
elseif(ZPLATFORM_ARM64)
    message(arm64-v8a)
    set(HOOKZZ_SOURCE_DIR ${AS_PROJECT_ROOT}/../HookZz)
    include_directories(${HOOKZZ_SOURCE_DIR}/include)
    include_directories(${HOOKZZ_SOURCE_DIR}/src)
    include_directories(${HOOKZZ_SOURCE_DIR}/src/platforms/backend-arm64)
    include_directories(${HOOKZZ_SOURCE_DIR}/src/platforms/arch-arm64)
    include_directories(${HOOKZZ_SOURCE_DIR}/src/platforms/backend-linux)
    include_directories(${HOOKZZ_SOURCE_DIR}/src/SaitamaKit)
    include_directories(${HOOKZZ_SOURCE_DIR}/src/SaitamaKit/include)
    include_directories(${HOOKZZ_SOURCE_DIR}/src/SaitamaKit/PosixKit/memory)
    include_directories(${HOOKZZ_SOURCE_DIR}/src/SaitamaKit/CommonKit/memory)
    include_directories(${HOOKZZ_SOURCE_DIR}/src/SaitamaKit/LinuxKit/memory)
    include_directories(${HOOKZZ_SOURCE_DIR}/src/SaitamaKit/PosixKit/thread)
    set(HOOK_LIB_SOURCES
      ${HOOKZZ_SOURCE_DIR}/src/platforms/backend-linux/interceptor-linux.c
      ${HOOKZZ_SOURCE_DIR}/src/platforms/backend-linux/memory-linux.c
      ${HOOKZZ_SOURCE_DIR}/src/platforms/backend-posix/thread-posix.c
      ${HOOKZZ_SOURCE_DIR}/src/SaitamaKit/PosixKit/memory/posix_memory_kit.c
      ${HOOKZZ_SOURCE_DIR}/src/SaitamaKit/CommonKit/memory/common_memory_kit.c
      ${HOOKZZ_SOURCE_DIR}/src/SaitamaKit/CommonKit/memory/memory_kit.c
      ${HOOKZZ_SOURCE_DIR}/src/SaitamaKit/LinuxKit/memory/linux_memory_kit.c
      ${HOOKZZ_SOURCE_DIR}/src/SaitamaKit/PosixKit/thread/posix_thread_kit.c
      ${HOOKZZ_SOURCE_DIR}/src/platforms/backend-arm64/backend-arm64-helper.c
      ${HOOKZZ_SOURCE_DIR}/src/platforms/backend-arm64/closure-bridge-arm64.c
      ${HOOKZZ_SOURCE_DIR}/src/platforms/backend-arm64/closure-bridge-template-arm64.c
      ${HOOKZZ_SOURCE_DIR}/src/platforms/backend-arm64/custom-bridge-handler.c
      ${HOOKZZ_SOURCE_DIR}/src/platforms/backend-arm64/interceptor-arm64.c
      ${HOOKZZ_SOURCE_DIR}/src/platforms/backend-arm64/closure-stub-template-arm64.S
      ${HOOKZZ_SOURCE_DIR}/src/platforms/backend-arm64/dynamic-closure-bridge-template-arm64.S
      ${HOOKZZ_SOURCE_DIR}/src/platforms/arch-arm64/ARM64AssemblyCore.c
      ${HOOKZZ_SOURCE_DIR}/src/platforms/arch-arm64/instructions.c
      ${HOOKZZ_SOURCE_DIR}/src/platforms/arch-arm64/reader-arm64.c
      ${HOOKZZ_SOURCE_DIR}/src/platforms/arch-arm64/regs-arm64.c
      ${HOOKZZ_SOURCE_DIR}/src/platforms/arch-arm64/relocator-arm64.c
      ${HOOKZZ_SOURCE_DIR}/src/platforms/arch-arm64/writer-arm64.c
      ${HOOKZZ_SOURCE_DIR}/src/debuglog.c
      ${HOOKZZ_SOURCE_DIR}/src/interceptor.c
      ${HOOKZZ_SOURCE_DIR}/src/memhelper.c
      ${HOOKZZ_SOURCE_DIR}/src/memory.c
      ${HOOKZZ_SOURCE_DIR}/src/stack.c
      ${HOOKZZ_SOURCE_DIR}/src/trampoline.c
      ${HOOKZZ_SOURCE_DIR}/src/hookzz.c
      )
endif()

ENABLE_LANGUAGE( ASM)



add_library( # Sets the name of the library.
             native-lib

             # Sets the library as a shared library.
             SHARED

             # Provides a relative path to your source file(s).
             src/main/cpp/native-lib.cpp
             src/main/cpp/xxtea.c
             src/main/cpp/ufile.cpp
             ${HOOK_LIB_SOURCES}
              )

# Searches for a specified prebuilt library and stores the path as a
# variable. Because CMake includes system libraries in the search path by
# default, you only need to specify the name of the public NDK library
# you want to add. CMake verifies that the library exists before
# completing its build.

find_library( # Sets the name of the path variable.
              log-lib

              # Specifies the name of the NDK library that
              # you want CMake to locate.
              log )

# Specifies libraries CMake should link to your target library. You
# can link multiple libraries, such as libraries you define in this
# build script, prebuilt third-party libraries, or system libraries.

target_link_libraries( # Specifies the target library.
                       native-lib

                       # Links the target library to the log library
                       # included in the NDK.
                       ${log-lib}
                       # ${CMAKE_CURRENT_SOURCE_DIR}/libs/armeabi-v7a/libDxHook.a
                       # ${CMAKE_CURRENT_SOURCE_DIR}/libs/arm64-v8a/libDxHook.a
                       )