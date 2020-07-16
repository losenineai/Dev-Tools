/*
 * properties.cpp
 *
 *  Created on: 2016年12月14日
 *      Author: boyliang
 */

#include <dlfcn.h>
#include <stee.h>
#include <memory.h>
#include <string>
#include "properties.h"
#include "DXLog.h"
#include "dx_libc.h"

typedef int (*property_get_type)(const char *, char *, const char *);
typedef int (*property_set_type)(const char *, const char *);

STEE
const char *dx_get_sys_lib_path(const char *name){

    char *path = NULL;
    const char *prePath = (sizeof(void*) == 8) ? "/system/lib64/" : "/system/lib/";

    int len = strlen(prePath) + strlen(name)+1;
    path = (char *)malloc(sizeof(char) * len);
    strcpy(path, prePath);
    strcat(path, name);

    return path;
}

string property_get_ex(const string &key) {
    char value[512] = { 0 };
    property_get(key.c_str(), value, "");
    return string(value);
};

int property_get(const char *key, char *value, const char *default_value){
    static bool is_init = false;
    if(is_init == false){
        init_properties();
        is_init = true;
    }

    return get_property_value(key, value, default_value);

//	static property_get_type property_get_;
//
//	if(!property_get_) {
//        //property_get_ = (property_get_type) dlsym(RTLD_DEFAULT, "property_get");
//
//        void* libc_handle = dlopen(dx_get_sys_lib_path("libc.so"), RTLD_LAZY);
//        property_get_ = (property_get_type) dlsym(libc_handle, "__system_property_get");
//	}
//
//	if(property_get_){
//		return property_get_(key, value, default_value);
//	}else{
//		return -1;
//	}
}

int property_set(const char *key, const char *value){

    static property_set_type property_set_;

    if(!property_set_) {
        //property_set_ = (property_set_type) dlsym(RTLD_DEFAULT, "property_set");

        void* libc_handle = dlopen(dx_get_sys_lib_path("libc.so"), RTLD_LAZY);
        property_set_ = (property_set_type) dlsym(libc_handle, "__system_property_set");
    }

	if (property_set_) {
		return property_set_(key, value);
	} else {
		return -1;
	}
}




