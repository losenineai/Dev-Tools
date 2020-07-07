# APP_STL := gnustl_static
APP_STL := c++_static

APP_ABI := armeabi-v7a x86 #arm64-v8a x86 x86_64
APP_DEPRECATED_HEADERS := true

APP_CPPFLAGS += -std=c++11
APP_CPPFLAGS += -Wall
APP_CFLAGS += -Wall
