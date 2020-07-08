ifneq ($(ENABLE_ASAN),)
    APP_STL := c++_shared
else
    APP_STL := c++_static
endif



