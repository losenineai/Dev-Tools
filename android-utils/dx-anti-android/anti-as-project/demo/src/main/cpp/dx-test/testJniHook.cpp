//
// Created by weaiken on 2019-11-12.
//

#include "testJniHook.h"


JavaVM *g_vm;

typedef void (*Bridge_DalvikBridgeFunc)(const void **, void *, const void *,        void *);

typedef jobject (*Native_nativeSetUpFunc)(JNIEnv *, jclass, jobject weak_this,      jint cameraId, jint halVersion, jstring clientPackageName);

typedef jobject (*Native_nativeSetUpDalvikFunc)(JNIEnv *, jclass,
                                                jobject weak_this,  jint halVersion, jstring clientPackageName);


jstring  mark() {
    // Do nothing
    JNIEnv *env = NULL;
    g_vm->GetEnv((void **) &env, JNI_VERSION_1_4);
    return env->NewStringUTF("weaiken");
};


static struct {
    bool isArt;
    int nativeOffset;
    void *art_work_around_app_jni_bugs;
    char *(*GetCstrFromString)(void *);
    void *(*GetStringFromCstr)(const char *);
    Bridge_DalvikBridgeFunc orig_DalvikBridgeFunc;
    Native_nativeSetUpFunc orig_native_openDexNativeFunc;
    Native_nativeSetUpDalvikFunc orig_native_openDexNativeDalvikFunc;
} gOffset;

char *newPkgName;
static jobject new_nativeSetUpFunc(JNIEnv *env, jclass jclazz,
                                   jobject weak_this, jint cameraId, jint halVersion, jstring clientPackageName) {

    jstring newPkg = env->NewStringUTF(newPkgName);
    return gOffset.orig_native_openDexNativeFunc(env, jclazz, weak_this, cameraId, halVersion, newPkg);

}

static jobject new_nativeSetUpDalvikFunc(JNIEnv *env, jclass jclazz,
                                         jobject weak_this, jint halVersion, jstring clientPackageName) {

    jstring newPkg = env->NewStringUTF(newPkgName);

    return gOffset.orig_native_openDexNativeDalvikFunc(env, jclazz, weak_this, halVersion, newPkg);

}

static void new_bridge_nativeSetUpFunc(const void **args, void *pResult, const      void *method,  void *self) {

    JNIEnv *env = NULL;
    g_vm->GetEnv((void **) &env, JNI_VERSION_1_4);
    g_vm->AttachCurrentThread(&env, NULL);

    typedef char* (*GetCstrFromString)(void *);
    typedef void* (*GetStringFromCstr)(const char*);

    const char* origPkg0 = args[0] == NULL ? NULL : gOffset.GetCstrFromString((void*) args[0]);
    LOGD("The original package0 is: %s", origPkg0);

    const char* origPkg = args[2] == NULL ? NULL : gOffset.GetCstrFromString((void*) args[2]);
    LOGD("The original package2 is: %s", origPkg);

    const char* origPkg3 = args[3] == NULL ? NULL : gOffset.GetCstrFromString((void*) args[3]);
    LOGD("The original package3 is: %s", origPkg3);

    args[3] = gOffset.GetStringFromCstr(newPkgName);
    gOffset.orig_DalvikBridgeFunc(args, pResult, method, self);
}


void searchJniOffset(JNIEnv *env, bool isArt) {

    jclass g_class = env->FindClass(JAVA_CLASS);
    char st[] = "stringFromJNI";
    jmethodID mtd_nativeHook = env->GetStaticMethodID(g_class, st, "(Landroid/content/Context;)Ljava/lang/String;");

    size_t startAddress = (size_t) mtd_nativeHook;
    size_t targetAddress = (size_t) mark;
    if (isArt && gOffset.art_work_around_app_jni_bugs) {
        targetAddress = (size_t) gOffset.art_work_around_app_jni_bugs;
    }

    int offset = 0;
    bool found = false;
    while (true) {
        if (*((size_t *) (startAddress + offset)) == targetAddress) {
            found = true;
            break;
        }
        offset += 4;
        if (offset >= 100) {
            LOGE("Ops: Unable to find the jni function.");
            break;
        }
    }

    if (found) {
        gOffset.nativeOffset = offset;
        if (!isArt) {
            gOffset.nativeOffset += (sizeof(int) + sizeof(void *));
        }
        LOGD("Hoho, Get the offset : %d.", gOffset.nativeOffset);
    }
}

inline void replaceImplementation(JNIEnv *env, jobject javaMethod, jboolean isArt) {

    size_t mtd_openDexNative = (size_t) env->FromReflectedMethod(javaMethod);
    int nativeFuncOffset = gOffset.nativeOffset;
    void **jniFuncPtr = (void **) (mtd_openDexNative + nativeFuncOffset);

    if (!isArt) {
        LOGD("The vm is dalvik");
        gOffset.orig_DalvikBridgeFunc = (Bridge_DalvikBridgeFunc) (*jniFuncPtr);
        *jniFuncPtr = (void *) new_bridge_nativeSetUpFunc;
    } else {
        char vmSoName[4] = {0};
        __system_property_get("ro.build.version.sdk", vmSoName);
        int sdk;
        sscanf(vmSoName, "%d", &sdk);
        LOGD("The vm is art and the sdk int is %d", sdk);

        if (sdk < 21) {
            gOffset.orig_native_openDexNativeDalvikFunc = (Native_nativeSetUpDalvikFunc) (*jniFuncPtr);
            *jniFuncPtr = (void *) new_nativeSetUpDalvikFunc;
        } else {
            gOffset.orig_native_openDexNativeFunc = (Native_nativeSetUpFunc) (*jniFuncPtr);
            *jniFuncPtr = (void *) new_nativeSetUpFunc;
        }

    }

}

static JNINativeMethod gMarkMethods[] = {
        {"stringFromJNI", "(Landroid/content/Context;)Ljava/lang/String;", (jstring ) mark}
};

void native_hook(JNIEnv *env, jclass clazz, jobject method, jstring pkg, jboolean isArt) {

    newPkgName = (char *) env->GetStringUTFChars(pkg, NULL);
    g_vm->GetEnv((void **) &env, JNI_VERSION_1_6);
    g_vm->AttachCurrentThread(&env, NULL);

    jclass g_class = env->FindClass(JAVA_CLASS);
    if (env->RegisterNatives(g_class, gMarkMethods, 1) < 0) {
        return;
    }

    gOffset.isArt = isArt;

    char vmSoName[15] = {0};
    __system_property_get("persist.sys.dalvik.vm.lib", vmSoName);
    LOGD("Find the so name : %s.", strlen(vmSoName) == 0 ? "<EMPTY>" : vmSoName);

    void *vmHandle = dlopen(vmSoName, 0);
    if (!vmHandle) {
        LOGE("Unable to open the %s.", vmSoName);
        vmHandle = RTLD_DEFAULT;
    }


    if (isArt) {
        gOffset.art_work_around_app_jni_bugs = dlsym(vmHandle, "art_work_around_app_jni_bugs");
    } else {
        gOffset.GetCstrFromString = (char *(*)(void *)) dlsym(vmHandle,                                                 "_Z23dvmCreateCstrFromStringPK12StringObject");
        if (!gOffset.GetCstrFromString) {
            gOffset.GetCstrFromString = (char *(*)(void *)) dlsym(vmHandle,
                                                                  "dvmCreateCstrFromString");
        }

        gOffset.GetStringFromCstr = (void *(*)(const char *)) dlsym(vmHandle,
                                                                    "_Z23dvmCreateStringFromCstrPKc");
        if (!gOffset.GetStringFromCstr) {
            gOffset.GetStringFromCstr = (void *(*)(const char *)) dlsym(vmHandle,  "dvmCreateStringFromCstr");
        }
    }

    searchJniOffset(env, isArt);
    replaceImplementation(env, method, isArt);
}


static JNINativeMethod gMethods[] = {
        { "testHook", "(Landroid/content/Context;)Ljava/lang/String;",
                (void *) native_hook }
};


//jstring MainActivity_testHook(JNIEnv *env, jobject obj, jobject jContext) {
//    JNINativeMethod methods[] = {
//            {
//                    "stringFromJNI",
//                    "(" JCONTEXT ")" JSTRING,
//            (void *) MainActivity_stringFromJNI
//            }
//    };
//
//    registerNativeMethods(env, "mobile/blue/com/anti_as_project/MainActivity", methods, NELEM(methods));
//}



JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *vm, void *reserved) {
    g_vm = vm;
    JNIEnv *env;

    LOGE("JNI_Onload start");
    if (vm->GetEnv((void **) &env, JNI_VERSION_1_4) != JNI_OK) {
        LOGE("GetEnv() FAILED!!!");
        return JNI_ERR;
    }

    jclass javaClass = env->FindClass(JAVA_CLASS);
    LOGE("we have found the class: %s", JAVA_CLASS);
    if (javaClass == NULL) {
        LOGE("unable to find class: %s", JAVA_CLASS);
        return JNI_ERR;
    }

    env->UnregisterNatives(javaClass);
    if (env->RegisterNatives(javaClass, gMethods, 1) < 0) {
        LOGE("register methods FAILED!!!");
        return JNI_ERR;
    }

    env->DeleteLocalRef(javaClass);
    LOGI("JavaVM::GetEnv() SUCCESS!");
    return JNI_VERSION_1_4;
}