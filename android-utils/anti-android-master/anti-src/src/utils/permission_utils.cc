//
// Created by amlloc on 18-7-13.
//

//public static boolean checkPermissions(String... perms){
//        Context ctx = ContextFinder.getApplication();
//
//        for(String perm : perms){
//            if(ctx.checkPermission(perm, Process.myPid(), Process.myUid()) != PackageManager.PERMISSION_GRANTED){
//                return false;
//            }
//        }
//
//        return true;
//    }
#include <jni.h>
#include <string>
#include "jni_comm_utils.h"
#include "jni_utils.h"

using namespace std;
bool checkPermissions(const char*  perm){

    DXAnti_JNIEnvGuard guard;
    JNIEnv *env = anti_get_jni_env();
    jobject jcontext = anti_jni_getGlobalContext(env);

    //android.os.Process
    //android.content.Context
    //checkPermission(String var1, int var2, int var3);
    jclass jProcess = anti_jni_findClassByName(env, "android/os/Process");

    jint myPid = anti_jni_callStaticMethodReturnInt(env, jProcess, "myPid", "()I");
    jint myUid = anti_jni_callStaticMethodReturnInt(env, jProcess, "myUid", "()I");
    jstring jperm = env->NewStringUTF(perm);
    jint jretInt = anti_jni_callIntMethodByName(env, jcontext, "checkPermission",
                                                         "(Ljava/lang/String;II)I", jperm, myPid,
                                                         myUid);
    bool isPermitted = true;
    if (jretInt != 0){
        //anti_err_log("permission needed: %s", perm);
        isPermitted = false;
    }

    anti_jni_delete_ref(env, jProcess);
    anti_jni_delete_ref(env, jperm);
    return isPermitted;
}