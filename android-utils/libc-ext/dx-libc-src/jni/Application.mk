# APP_STL := gnustl_static
APP_STL := c++_static
APP_ABI := armeabi armeabi-v7a arm64-v8a x86 x86_64

APP_CPPFLAGS += -std=c++11
APP_CPPFLAGS += -Wall -fPIC
APP_CFLAGS += -Wall -fPIC