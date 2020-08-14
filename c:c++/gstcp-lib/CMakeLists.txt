cmake_minimum_required(VERSION 3.4)
project(libgstcp)

cmake_policy(SET CMP0057 NEW) # Enable IN_LIST operator
cmake_policy(SET CMP0064 NEW) # Support if (TEST) operator

list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")

include(CMakePackageConfigHelpers)
include(CMakeDependentOption)
include(CheckCCompilerFlag)
include(GNUInstallDirs)
include(CTest)

set(CMAKE_C_VISIBILITY_PRESET hidden)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)
set(CMAKE_C_STANDARD 90)



set(GSTCP_DIR ${CMAKE_SOURCE_DIR})
set(UV_DIR ${CMAKE_SOURCE_DIR}/src/uv)
set(UV_SRC
        ${UV_DIR}/src/fs-poll.c
        ${UV_DIR}/src/idna.c
        ${UV_DIR}/src/inet.c
        ${UV_DIR}/src/random.c
        ${UV_DIR}/src/strscpy.c
        ${UV_DIR}/src/threadpool.c
        ${UV_DIR}/src/timer.c
        ${UV_DIR}/src/uv-common.c
        ${UV_DIR}/src/uv-data-getter-setters.c
        ${UV_DIR}/src/version.c
        ${UV_DIR}/src/unix/android-ifaddrs.c
        ${UV_DIR}/src/unix/linux-core.c
        ${UV_DIR}/src/unix/linux-inotify.c
        ${UV_DIR}/src/unix/linux-syscalls.c
        ${UV_DIR}/src/unix/procfs-exepath.c
        ${UV_DIR}/src/unix/pthread-fixes.c
        ${UV_DIR}/src/unix/random-getentropy.c
        ${UV_DIR}/src/unix/random-getrandom.c
        ${UV_DIR}/src/unix/random-sysctl-linux.c
        ${UV_DIR}/src/unix/async.c
        ${UV_DIR}/src/unix/core.c
        ${UV_DIR}/src/unix/dl.c
        ${UV_DIR}/src/unix/fs.c
        ${UV_DIR}/src/unix/getaddrinfo.c
        ${UV_DIR}/src/unix/getnameinfo.c
        ${UV_DIR}/src/unix/loop-watcher.c
        ${UV_DIR}/src/unix/loop.c
        ${UV_DIR}/src/unix/pipe.c
        ${UV_DIR}/src/unix/poll.c
        ${UV_DIR}/src/unix/process.c
        ${UV_DIR}/src/unix/random-devurandom.c
        ${UV_DIR}/src/unix/signal.c
        ${UV_DIR}/src/unix/stream.c
        ${UV_DIR}/src/unix/tcp.c
        ${UV_DIR}/src/unix/thread.c
        ${UV_DIR}/src/unix/tty.c
        ${UV_DIR}/src/unix/udp.c
        )
include_directories(
        ${UV_DIR}/include
        ${UV_DIR}/src
)

aux_source_directory(${GSTCP_DIR}/src/net GSTCP_SRC)
aux_source_directory(${GSTCP_DIR}/src/net/http GSTCP_SRC)
include_directories(
        ${GSTCP_DIR}/include
)



# add_library(gstcp STATIC 
add_library(gstcp SHARED 
        ${UV_SRC}
        ${GSTCP_SRC}
        )



message(STATUS "summary of build options:
    Install prefix:  ${CMAKE_INSTALL_PREFIX}
    Target system:   ${CMAKE_SYSTEM_NAME}
    SRC_DIR:         ${SRC_DIR}
    GSTCP_SRC:       ${GSTCP_SRC}
    UV_SRC:          ${UV_SRC}
    Compiler:
      C compiler:    ${CMAKE_C_COMPILER}
      CFLAGS:        ${CMAKE_C_FLAGS_${_build_type}} ${CMAKE_C_FLAGS}
      CXXFLAGS:      ${CMAKE_CXX_FLAGS}
      ANDROID_ABI:   ${ANDROID_ABI}
")
