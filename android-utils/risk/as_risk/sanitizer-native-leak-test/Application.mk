# APP_STL := gnustl_static
# APP_STL := c++_static
APP_ABI := armeabi-v7a # arm64-v8a x86
APP_DEPRECATED_HEADERS := true

APP_CPPFLAGS += -std=c++11
APP_CPPFLAGS += -Wall
APP_CFLAGS += -Wall

APP_STL := c++_shared # Or system, or none.
APP_CFLAGS := -fsanitize=address -fno-omit-frame-pointer
APP_LDFLAGS := -fsanitize=address

