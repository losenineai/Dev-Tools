// ignore0 Not Found
// ignore1 Not Found
// ignore2 Not Found
// ignore3 Not Found
static jint wrapper_GetVersion(JNIEnv * env) {    return ((JniEnvProxy::Env0*)env)->next->GetVersion();}
static jclass wrapper_DefineClass(JNIEnv * env, const char * p0, jobject p1, const jbyte * p2, jsize p3) {    return ((JniEnvProxy::Env0*)env)->next->DefineClass(p0, p1, p2, p3);}
static jclass wrapper_FindClass(JNIEnv * env, const char * p0) {    return ((JniEnvProxy::Env0*)env)->next->FindClass(p0);}
static jmethodID wrapper_FromReflectedMethod(JNIEnv * env, jobject p0) {    return ((JniEnvProxy::Env0*)env)->next->FromReflectedMethod(p0);}
static jfieldID wrapper_FromReflectedField(JNIEnv * env, jobject p0) {    return ((JniEnvProxy::Env0*)env)->next->FromReflectedField(p0);}
static jobject wrapper_ToReflectedMethod(JNIEnv * env, jclass p0, jmethodID p1, jboolean p2) {    return ((JniEnvProxy::Env0*)env)->next->ToReflectedMethod(p0, p1, p2);}
static jclass wrapper_GetSuperclass(JNIEnv * env, jclass p0) {    return ((JniEnvProxy::Env0*)env)->next->GetSuperclass(p0);}
static jboolean wrapper_IsAssignableFrom(JNIEnv * env, jclass p0, jclass p1) {    return ((JniEnvProxy::Env0*)env)->next->IsAssignableFrom(p0, p1);}
static jobject wrapper_ToReflectedField(JNIEnv * env, jclass p0, jfieldID p1, jboolean p2) {    return ((JniEnvProxy::Env0*)env)->next->ToReflectedField(p0, p1, p2);}
static jint wrapper_Throw(JNIEnv * env, jthrowable p0) {    return ((JniEnvProxy::Env0*)env)->next->Throw(p0);}
static jint wrapper_ThrowNew(JNIEnv * env, jclass p0, const char * p1) {    return ((JniEnvProxy::Env0*)env)->next->ThrowNew(p0, p1);}
static jthrowable wrapper_ExceptionOccurred(JNIEnv * env) {    return ((JniEnvProxy::Env0*)env)->next->ExceptionOccurred();}
static void wrapper_ExceptionDescribe(JNIEnv * env) {    ((JniEnvProxy::Env0*)env)->next->ExceptionDescribe();}
static void wrapper_ExceptionClear(JNIEnv * env) {    ((JniEnvProxy::Env0*)env)->next->ExceptionClear();}
static void wrapper_FatalError(JNIEnv * env, const char * p0) {    ((JniEnvProxy::Env0*)env)->next->FatalError(p0);}
static jint wrapper_PushLocalFrame(JNIEnv * env, jint p0) {    return ((JniEnvProxy::Env0*)env)->next->PushLocalFrame(p0);}
static jobject wrapper_PopLocalFrame(JNIEnv * env, jobject p0) {    return ((JniEnvProxy::Env0*)env)->next->PopLocalFrame(p0);}
static jobject wrapper_NewGlobalRef(JNIEnv * env, jobject p0) {    return ((JniEnvProxy::Env0*)env)->next->NewGlobalRef(p0);}
static void wrapper_DeleteGlobalRef(JNIEnv * env, jobject p0) {    ((JniEnvProxy::Env0*)env)->next->DeleteGlobalRef(p0);}
static void wrapper_DeleteLocalRef(JNIEnv * env, jobject p0) {    ((JniEnvProxy::Env0*)env)->next->DeleteLocalRef(p0);}
static jboolean wrapper_IsSameObject(JNIEnv * env, jobject p0, jobject p1) {    return ((JniEnvProxy::Env0*)env)->next->IsSameObject(p0, p1);}
static jobject wrapper_NewLocalRef(JNIEnv * env, jobject p0) {    return ((JniEnvProxy::Env0*)env)->next->NewLocalRef(p0);}
static jint wrapper_EnsureLocalCapacity(JNIEnv * env, jint p0) {    return ((JniEnvProxy::Env0*)env)->next->EnsureLocalCapacity(p0);}
static jobject wrapper_AllocObject(JNIEnv * env, jclass p0) {    return ((JniEnvProxy::Env0*)env)->next->AllocObject(p0);}
static jobject wrapper_NewObjectV(JNIEnv * env, jclass p0, jmethodID p1, va_list p2);
static jobject wrapper_NewObject(JNIEnv * env, jclass p0, jmethodID p1, ...) {
    va_list args; va_start(args, p1);
    jobject result= wrapper_NewObjectV(env, p0, p1, args);
    va_end(args);
    return result;
}
static jobject wrapper_NewObjectV(JNIEnv * env, jclass p0, jmethodID p1, va_list p2) {    return ((JniEnvProxy::Env0*)env)->next->NewObjectV(p0, p1, p2);}
static jobject wrapper_NewObjectA(JNIEnv * env, jclass p0, jmethodID p1, jvalue * p2) {    return ((JniEnvProxy::Env0*)env)->next->NewObjectA(p0, p1, p2);}
static jclass wrapper_GetObjectClass(JNIEnv * env, jobject p0) {    return ((JniEnvProxy::Env0*)env)->next->GetObjectClass(p0);}
static jboolean wrapper_IsInstanceOf(JNIEnv * env, jobject p0, jclass p1) {    return ((JniEnvProxy::Env0*)env)->next->IsInstanceOf(p0, p1);}
static jmethodID wrapper_GetMethodID(JNIEnv * env, jclass p0, const char * p1, const char * p2) {    return ((JniEnvProxy::Env0*)env)->next->GetMethodID(p0, p1, p2);}
static jobject wrapper_CallObjectMethodV(JNIEnv * env, jobject p0, jmethodID p1, va_list p2);
static jobject wrapper_CallObjectMethod(JNIEnv * env, jobject p0, jmethodID p1, ...) {
    va_list args; va_start(args, p1);
    jobject result= wrapper_CallObjectMethodV(env, p0, p1, args);
    va_end(args);
    return result;
}
static jobject wrapper_CallObjectMethodV(JNIEnv * env, jobject p0, jmethodID p1, va_list p2) {    return ((JniEnvProxy::Env0*)env)->next->CallObjectMethodV(p0, p1, p2);}
static jobject wrapper_CallObjectMethodA(JNIEnv * env, jobject p0, jmethodID p1, jvalue * p2) {    return ((JniEnvProxy::Env0*)env)->next->CallObjectMethodA(p0, p1, p2);}
static jboolean wrapper_CallBooleanMethodV(JNIEnv * env, jobject p0, jmethodID p1, va_list p2);
static jboolean wrapper_CallBooleanMethod(JNIEnv * env, jobject p0, jmethodID p1, ...) {
    va_list args; va_start(args, p1);
    jboolean result= wrapper_CallBooleanMethodV(env, p0, p1, args);
    va_end(args);
    return result;
}
static jboolean wrapper_CallBooleanMethodV(JNIEnv * env, jobject p0, jmethodID p1, va_list p2) {    return ((JniEnvProxy::Env0*)env)->next->CallBooleanMethodV(p0, p1, p2);}
static jboolean wrapper_CallBooleanMethodA(JNIEnv * env, jobject p0, jmethodID p1, jvalue * p2) {    return ((JniEnvProxy::Env0*)env)->next->CallBooleanMethodA(p0, p1, p2);}
static jbyte wrapper_CallByteMethodV(JNIEnv * env, jobject p0, jmethodID p1, va_list p2);
static jbyte wrapper_CallByteMethod(JNIEnv * env, jobject p0, jmethodID p1, ...) {
    va_list args; va_start(args, p1);
    jbyte result= wrapper_CallByteMethodV(env, p0, p1, args);
    va_end(args);
    return result;
}
static jbyte wrapper_CallByteMethodV(JNIEnv * env, jobject p0, jmethodID p1, va_list p2) {    return ((JniEnvProxy::Env0*)env)->next->CallByteMethodV(p0, p1, p2);}
static jbyte wrapper_CallByteMethodA(JNIEnv * env, jobject p0, jmethodID p1, jvalue * p2) {    return ((JniEnvProxy::Env0*)env)->next->CallByteMethodA(p0, p1, p2);}
static jchar wrapper_CallCharMethodV(JNIEnv * env, jobject p0, jmethodID p1, va_list p2);
static jchar wrapper_CallCharMethod(JNIEnv * env, jobject p0, jmethodID p1, ...) {
    va_list args; va_start(args, p1);
    jchar result= wrapper_CallCharMethodV(env, p0, p1, args);
    va_end(args);
    return result;
}
static jchar wrapper_CallCharMethodV(JNIEnv * env, jobject p0, jmethodID p1, va_list p2) {    return ((JniEnvProxy::Env0*)env)->next->CallCharMethodV(p0, p1, p2);}
static jchar wrapper_CallCharMethodA(JNIEnv * env, jobject p0, jmethodID p1, jvalue * p2) {    return ((JniEnvProxy::Env0*)env)->next->CallCharMethodA(p0, p1, p2);}
static jshort wrapper_CallShortMethodV(JNIEnv * env, jobject p0, jmethodID p1, va_list p2);
static jshort wrapper_CallShortMethod(JNIEnv * env, jobject p0, jmethodID p1, ...) {
    va_list args; va_start(args, p1);
    jshort result= wrapper_CallShortMethodV(env, p0, p1, args);
    va_end(args);
    return result;
}
static jshort wrapper_CallShortMethodV(JNIEnv * env, jobject p0, jmethodID p1, va_list p2) {    return ((JniEnvProxy::Env0*)env)->next->CallShortMethodV(p0, p1, p2);}
static jshort wrapper_CallShortMethodA(JNIEnv * env, jobject p0, jmethodID p1, jvalue * p2) {    return ((JniEnvProxy::Env0*)env)->next->CallShortMethodA(p0, p1, p2);}
static jint wrapper_CallIntMethodV(JNIEnv * env, jobject p0, jmethodID p1, va_list p2);
static jint wrapper_CallIntMethod(JNIEnv * env, jobject p0, jmethodID p1, ...) {
    va_list args; va_start(args, p1);
    jint result= wrapper_CallIntMethodV(env, p0, p1, args);
    va_end(args);
    return result;
}
static jint wrapper_CallIntMethodV(JNIEnv * env, jobject p0, jmethodID p1, va_list p2) {    return ((JniEnvProxy::Env0*)env)->next->CallIntMethodV(p0, p1, p2);}
static jint wrapper_CallIntMethodA(JNIEnv * env, jobject p0, jmethodID p1, jvalue * p2) {    return ((JniEnvProxy::Env0*)env)->next->CallIntMethodA(p0, p1, p2);}
static jlong wrapper_CallLongMethodV(JNIEnv * env, jobject p0, jmethodID p1, va_list p2);
static jlong wrapper_CallLongMethod(JNIEnv * env, jobject p0, jmethodID p1, ...) {
    va_list args; va_start(args, p1);
    jlong result= wrapper_CallLongMethodV(env, p0, p1, args);
    va_end(args);
    return result;
}
static jlong wrapper_CallLongMethodV(JNIEnv * env, jobject p0, jmethodID p1, va_list p2) {    return ((JniEnvProxy::Env0*)env)->next->CallLongMethodV(p0, p1, p2);}
static jlong wrapper_CallLongMethodA(JNIEnv * env, jobject p0, jmethodID p1, jvalue * p2) {    return ((JniEnvProxy::Env0*)env)->next->CallLongMethodA(p0, p1, p2);}
static jfloat wrapper_CallFloatMethodV(JNIEnv * env, jobject p0, jmethodID p1, va_list p2);
static jfloat wrapper_CallFloatMethod(JNIEnv * env, jobject p0, jmethodID p1, ...) {
    va_list args; va_start(args, p1);
    jfloat result= wrapper_CallFloatMethodV(env, p0, p1, args);
    va_end(args);
    return result;
}
static jfloat wrapper_CallFloatMethodV(JNIEnv * env, jobject p0, jmethodID p1, va_list p2) {    return ((JniEnvProxy::Env0*)env)->next->CallFloatMethodV(p0, p1, p2);}
static jfloat wrapper_CallFloatMethodA(JNIEnv * env, jobject p0, jmethodID p1, jvalue * p2) {    return ((JniEnvProxy::Env0*)env)->next->CallFloatMethodA(p0, p1, p2);}
static jdouble wrapper_CallDoubleMethodV(JNIEnv * env, jobject p0, jmethodID p1, va_list p2);
static jdouble wrapper_CallDoubleMethod(JNIEnv * env, jobject p0, jmethodID p1, ...) {
    va_list args; va_start(args, p1);
    jdouble result= wrapper_CallDoubleMethodV(env, p0, p1, args);
    va_end(args);
    return result;
}
static jdouble wrapper_CallDoubleMethodV(JNIEnv * env, jobject p0, jmethodID p1, va_list p2) {    return ((JniEnvProxy::Env0*)env)->next->CallDoubleMethodV(p0, p1, p2);}
static jdouble wrapper_CallDoubleMethodA(JNIEnv * env, jobject p0, jmethodID p1, jvalue * p2) {    return ((JniEnvProxy::Env0*)env)->next->CallDoubleMethodA(p0, p1, p2);}
static void wrapper_CallVoidMethodV(JNIEnv * env, jobject p0, jmethodID p1, va_list p2);
static void wrapper_CallVoidMethod(JNIEnv * env, jobject p0, jmethodID p1, ...) {
    va_list args; va_start(args, p1);
    wrapper_CallVoidMethodV(env, p0, p1, args);
    va_end(args);
}
static void wrapper_CallVoidMethodV(JNIEnv * env, jobject p0, jmethodID p1, va_list p2) {    ((JniEnvProxy::Env0*)env)->next->CallVoidMethodV(p0, p1, p2);}
static void wrapper_CallVoidMethodA(JNIEnv * env, jobject p0, jmethodID p1, jvalue * p2) {    ((JniEnvProxy::Env0*)env)->next->CallVoidMethodA(p0, p1, p2);}
static jobject wrapper_CallNonvirtualObjectMethodV(JNIEnv * env, jobject p0, jclass p1, jmethodID p2, va_list p3);
static jobject wrapper_CallNonvirtualObjectMethod(JNIEnv * env, jobject p0, jclass p1, jmethodID p2, ...) {
    va_list args; va_start(args, p2);
    jobject result= wrapper_CallNonvirtualObjectMethodV(env, p0, p1, p2, args);
    va_end(args);
    return result;
}
static jobject wrapper_CallNonvirtualObjectMethodV(JNIEnv * env, jobject p0, jclass p1, jmethodID p2, va_list p3) {    return ((JniEnvProxy::Env0*)env)->next->CallNonvirtualObjectMethodV(p0, p1, p2, p3);}
static jobject wrapper_CallNonvirtualObjectMethodA(JNIEnv * env, jobject p0, jclass p1, jmethodID p2, jvalue * p3) {    return ((JniEnvProxy::Env0*)env)->next->CallNonvirtualObjectMethodA(p0, p1, p2, p3);}
static jboolean wrapper_CallNonvirtualBooleanMethodV(JNIEnv * env, jobject p0, jclass p1, jmethodID p2, va_list p3);
static jboolean wrapper_CallNonvirtualBooleanMethod(JNIEnv * env, jobject p0, jclass p1, jmethodID p2, ...) {
    va_list args; va_start(args, p2);
    jboolean result= wrapper_CallNonvirtualBooleanMethodV(env, p0, p1, p2, args);
    va_end(args);
    return result;
}
static jboolean wrapper_CallNonvirtualBooleanMethodV(JNIEnv * env, jobject p0, jclass p1, jmethodID p2, va_list p3) {    return ((JniEnvProxy::Env0*)env)->next->CallNonvirtualBooleanMethodV(p0, p1, p2, p3);}
static jboolean wrapper_CallNonvirtualBooleanMethodA(JNIEnv * env, jobject p0, jclass p1, jmethodID p2, jvalue * p3) {    return ((JniEnvProxy::Env0*)env)->next->CallNonvirtualBooleanMethodA(p0, p1, p2, p3);}
static jbyte wrapper_CallNonvirtualByteMethodV(JNIEnv * env, jobject p0, jclass p1, jmethodID p2, va_list p3);
static jbyte wrapper_CallNonvirtualByteMethod(JNIEnv * env, jobject p0, jclass p1, jmethodID p2, ...) {
    va_list args; va_start(args, p2);
    jbyte result= wrapper_CallNonvirtualByteMethodV(env, p0, p1, p2, args);
    va_end(args);
    return result;
}
static jbyte wrapper_CallNonvirtualByteMethodV(JNIEnv * env, jobject p0, jclass p1, jmethodID p2, va_list p3) {    return ((JniEnvProxy::Env0*)env)->next->CallNonvirtualByteMethodV(p0, p1, p2, p3);}
static jbyte wrapper_CallNonvirtualByteMethodA(JNIEnv * env, jobject p0, jclass p1, jmethodID p2, jvalue * p3) {    return ((JniEnvProxy::Env0*)env)->next->CallNonvirtualByteMethodA(p0, p1, p2, p3);}
static jchar wrapper_CallNonvirtualCharMethodV(JNIEnv * env, jobject p0, jclass p1, jmethodID p2, va_list p3);
static jchar wrapper_CallNonvirtualCharMethod(JNIEnv * env, jobject p0, jclass p1, jmethodID p2, ...) {
    va_list args; va_start(args, p2);
    jchar result= wrapper_CallNonvirtualCharMethodV(env, p0, p1, p2, args);
    va_end(args);
    return result;
}
static jchar wrapper_CallNonvirtualCharMethodV(JNIEnv * env, jobject p0, jclass p1, jmethodID p2, va_list p3) {    return ((JniEnvProxy::Env0*)env)->next->CallNonvirtualCharMethodV(p0, p1, p2, p3);}
static jchar wrapper_CallNonvirtualCharMethodA(JNIEnv * env, jobject p0, jclass p1, jmethodID p2, jvalue * p3) {    return ((JniEnvProxy::Env0*)env)->next->CallNonvirtualCharMethodA(p0, p1, p2, p3);}
static jshort wrapper_CallNonvirtualShortMethodV(JNIEnv * env, jobject p0, jclass p1, jmethodID p2, va_list p3);
static jshort wrapper_CallNonvirtualShortMethod(JNIEnv * env, jobject p0, jclass p1, jmethodID p2, ...) {
    va_list args; va_start(args, p2);
    jshort result= wrapper_CallNonvirtualShortMethodV(env, p0, p1, p2, args);
    va_end(args);
    return result;
}
static jshort wrapper_CallNonvirtualShortMethodV(JNIEnv * env, jobject p0, jclass p1, jmethodID p2, va_list p3) {    return ((JniEnvProxy::Env0*)env)->next->CallNonvirtualShortMethodV(p0, p1, p2, p3);}
static jshort wrapper_CallNonvirtualShortMethodA(JNIEnv * env, jobject p0, jclass p1, jmethodID p2, jvalue * p3) {    return ((JniEnvProxy::Env0*)env)->next->CallNonvirtualShortMethodA(p0, p1, p2, p3);}
static jint wrapper_CallNonvirtualIntMethodV(JNIEnv * env, jobject p0, jclass p1, jmethodID p2, va_list p3);
static jint wrapper_CallNonvirtualIntMethod(JNIEnv * env, jobject p0, jclass p1, jmethodID p2, ...) {
    va_list args; va_start(args, p2);
    jint result= wrapper_CallNonvirtualIntMethodV(env, p0, p1, p2, args);
    va_end(args);
    return result;
}
static jint wrapper_CallNonvirtualIntMethodV(JNIEnv * env, jobject p0, jclass p1, jmethodID p2, va_list p3) {    return ((JniEnvProxy::Env0*)env)->next->CallNonvirtualIntMethodV(p0, p1, p2, p3);}
static jint wrapper_CallNonvirtualIntMethodA(JNIEnv * env, jobject p0, jclass p1, jmethodID p2, jvalue * p3) {    return ((JniEnvProxy::Env0*)env)->next->CallNonvirtualIntMethodA(p0, p1, p2, p3);}
static jlong wrapper_CallNonvirtualLongMethodV(JNIEnv * env, jobject p0, jclass p1, jmethodID p2, va_list p3);
static jlong wrapper_CallNonvirtualLongMethod(JNIEnv * env, jobject p0, jclass p1, jmethodID p2, ...) {
    va_list args; va_start(args, p2);
    jlong result= wrapper_CallNonvirtualLongMethodV(env, p0, p1, p2, args);
    va_end(args);
    return result;
}
static jlong wrapper_CallNonvirtualLongMethodV(JNIEnv * env, jobject p0, jclass p1, jmethodID p2, va_list p3) {    return ((JniEnvProxy::Env0*)env)->next->CallNonvirtualLongMethodV(p0, p1, p2, p3);}
static jlong wrapper_CallNonvirtualLongMethodA(JNIEnv * env, jobject p0, jclass p1, jmethodID p2, jvalue * p3) {    return ((JniEnvProxy::Env0*)env)->next->CallNonvirtualLongMethodA(p0, p1, p2, p3);}
static jfloat wrapper_CallNonvirtualFloatMethodV(JNIEnv * env, jobject p0, jclass p1, jmethodID p2, va_list p3);
static jfloat wrapper_CallNonvirtualFloatMethod(JNIEnv * env, jobject p0, jclass p1, jmethodID p2, ...) {
    va_list args; va_start(args, p2);
    jfloat result= wrapper_CallNonvirtualFloatMethodV(env, p0, p1, p2, args);
    va_end(args);
    return result;
}
static jfloat wrapper_CallNonvirtualFloatMethodV(JNIEnv * env, jobject p0, jclass p1, jmethodID p2, va_list p3) {    return ((JniEnvProxy::Env0*)env)->next->CallNonvirtualFloatMethodV(p0, p1, p2, p3);}
static jfloat wrapper_CallNonvirtualFloatMethodA(JNIEnv * env, jobject p0, jclass p1, jmethodID p2, jvalue * p3) {    return ((JniEnvProxy::Env0*)env)->next->CallNonvirtualFloatMethodA(p0, p1, p2, p3);}
static jdouble wrapper_CallNonvirtualDoubleMethodV(JNIEnv * env, jobject p0, jclass p1, jmethodID p2, va_list p3);
static jdouble wrapper_CallNonvirtualDoubleMethod(JNIEnv * env, jobject p0, jclass p1, jmethodID p2, ...) {
    va_list args; va_start(args, p2);
    jdouble result= wrapper_CallNonvirtualDoubleMethodV(env, p0, p1, p2, args);
    va_end(args);
    return result;
}
static jdouble wrapper_CallNonvirtualDoubleMethodV(JNIEnv * env, jobject p0, jclass p1, jmethodID p2, va_list p3) {    return ((JniEnvProxy::Env0*)env)->next->CallNonvirtualDoubleMethodV(p0, p1, p2, p3);}
static jdouble wrapper_CallNonvirtualDoubleMethodA(JNIEnv * env, jobject p0, jclass p1, jmethodID p2, jvalue * p3) {    return ((JniEnvProxy::Env0*)env)->next->CallNonvirtualDoubleMethodA(p0, p1, p2, p3);}
static void wrapper_CallNonvirtualVoidMethodV(JNIEnv * env, jobject p0, jclass p1, jmethodID p2, va_list p3);
static void wrapper_CallNonvirtualVoidMethod(JNIEnv * env, jobject p0, jclass p1, jmethodID p2, ...) {
    va_list args; va_start(args, p2);
    wrapper_CallNonvirtualVoidMethodV(env, p0, p1, p2, args);
    va_end(args);
}
static void wrapper_CallNonvirtualVoidMethodV(JNIEnv * env, jobject p0, jclass p1, jmethodID p2, va_list p3) {    ((JniEnvProxy::Env0*)env)->next->CallNonvirtualVoidMethodV(p0, p1, p2, p3);}
static void wrapper_CallNonvirtualVoidMethodA(JNIEnv * env, jobject p0, jclass p1, jmethodID p2, jvalue * p3) {    ((JniEnvProxy::Env0*)env)->next->CallNonvirtualVoidMethodA(p0, p1, p2, p3);}
static jfieldID wrapper_GetFieldID(JNIEnv * env, jclass p0, const char * p1, const char * p2) {    return ((JniEnvProxy::Env0*)env)->next->GetFieldID(p0, p1, p2);}
static jobject wrapper_GetObjectField(JNIEnv * env, jobject p0, jfieldID p1) {    return ((JniEnvProxy::Env0*)env)->next->GetObjectField(p0, p1);}
static jboolean wrapper_GetBooleanField(JNIEnv * env, jobject p0, jfieldID p1) {    return ((JniEnvProxy::Env0*)env)->next->GetBooleanField(p0, p1);}
static jbyte wrapper_GetByteField(JNIEnv * env, jobject p0, jfieldID p1) {    return ((JniEnvProxy::Env0*)env)->next->GetByteField(p0, p1);}
static jchar wrapper_GetCharField(JNIEnv * env, jobject p0, jfieldID p1) {    return ((JniEnvProxy::Env0*)env)->next->GetCharField(p0, p1);}
static jshort wrapper_GetShortField(JNIEnv * env, jobject p0, jfieldID p1) {    return ((JniEnvProxy::Env0*)env)->next->GetShortField(p0, p1);}
static jint wrapper_GetIntField(JNIEnv * env, jobject p0, jfieldID p1) {    return ((JniEnvProxy::Env0*)env)->next->GetIntField(p0, p1);}
static jlong wrapper_GetLongField(JNIEnv * env, jobject p0, jfieldID p1) {    return ((JniEnvProxy::Env0*)env)->next->GetLongField(p0, p1);}
static jfloat wrapper_GetFloatField(JNIEnv * env, jobject p0, jfieldID p1) {    return ((JniEnvProxy::Env0*)env)->next->GetFloatField(p0, p1);}
static jdouble wrapper_GetDoubleField(JNIEnv * env, jobject p0, jfieldID p1) {    return ((JniEnvProxy::Env0*)env)->next->GetDoubleField(p0, p1);}
static void wrapper_SetObjectField(JNIEnv * env, jobject p0, jfieldID p1, jobject p2) {    ((JniEnvProxy::Env0*)env)->next->SetObjectField(p0, p1, p2);}
static void wrapper_SetBooleanField(JNIEnv * env, jobject p0, jfieldID p1, jboolean p2) {    ((JniEnvProxy::Env0*)env)->next->SetBooleanField(p0, p1, p2);}
static void wrapper_SetByteField(JNIEnv * env, jobject p0, jfieldID p1, jbyte p2) {    ((JniEnvProxy::Env0*)env)->next->SetByteField(p0, p1, p2);}
static void wrapper_SetCharField(JNIEnv * env, jobject p0, jfieldID p1, jchar p2) {    ((JniEnvProxy::Env0*)env)->next->SetCharField(p0, p1, p2);}
static void wrapper_SetShortField(JNIEnv * env, jobject p0, jfieldID p1, jshort p2) {    ((JniEnvProxy::Env0*)env)->next->SetShortField(p0, p1, p2);}
static void wrapper_SetIntField(JNIEnv * env, jobject p0, jfieldID p1, jint p2) {    ((JniEnvProxy::Env0*)env)->next->SetIntField(p0, p1, p2);}
static void wrapper_SetLongField(JNIEnv * env, jobject p0, jfieldID p1, jlong p2) {    ((JniEnvProxy::Env0*)env)->next->SetLongField(p0, p1, p2);}
static void wrapper_SetFloatField(JNIEnv * env, jobject p0, jfieldID p1, jfloat p2) {    ((JniEnvProxy::Env0*)env)->next->SetFloatField(p0, p1, p2);}
static void wrapper_SetDoubleField(JNIEnv * env, jobject p0, jfieldID p1, jdouble p2) {    ((JniEnvProxy::Env0*)env)->next->SetDoubleField(p0, p1, p2);}
static jmethodID wrapper_GetStaticMethodID(JNIEnv * env, jclass p0, const char * p1, const char * p2) {    return ((JniEnvProxy::Env0*)env)->next->GetStaticMethodID(p0, p1, p2);}
static jobject wrapper_CallStaticObjectMethodV(JNIEnv * env, jclass p0, jmethodID p1, va_list p2);
static jobject wrapper_CallStaticObjectMethod(JNIEnv * env, jclass p0, jmethodID p1, ...) {
    va_list args; va_start(args, p1);
    jobject result= wrapper_CallStaticObjectMethodV(env, p0, p1, args);
    va_end(args);
    return result;
}
static jobject wrapper_CallStaticObjectMethodV(JNIEnv * env, jclass p0, jmethodID p1, va_list p2) {    return ((JniEnvProxy::Env0*)env)->next->CallStaticObjectMethodV(p0, p1, p2);}
static jobject wrapper_CallStaticObjectMethodA(JNIEnv * env, jclass p0, jmethodID p1, jvalue * p2) {    return ((JniEnvProxy::Env0*)env)->next->CallStaticObjectMethodA(p0, p1, p2);}
static jboolean wrapper_CallStaticBooleanMethodV(JNIEnv * env, jclass p0, jmethodID p1, va_list p2);
static jboolean wrapper_CallStaticBooleanMethod(JNIEnv * env, jclass p0, jmethodID p1, ...) {
    va_list args; va_start(args, p1);
    jboolean result= wrapper_CallStaticBooleanMethodV(env, p0, p1, args);
    va_end(args);
    return result;
}
static jboolean wrapper_CallStaticBooleanMethodV(JNIEnv * env, jclass p0, jmethodID p1, va_list p2) {    return ((JniEnvProxy::Env0*)env)->next->CallStaticBooleanMethodV(p0, p1, p2);}
static jboolean wrapper_CallStaticBooleanMethodA(JNIEnv * env, jclass p0, jmethodID p1, jvalue * p2) {    return ((JniEnvProxy::Env0*)env)->next->CallStaticBooleanMethodA(p0, p1, p2);}
static jbyte wrapper_CallStaticByteMethodV(JNIEnv * env, jclass p0, jmethodID p1, va_list p2);
static jbyte wrapper_CallStaticByteMethod(JNIEnv * env, jclass p0, jmethodID p1, ...) {
    va_list args; va_start(args, p1);
    jbyte result= wrapper_CallStaticByteMethodV(env, p0, p1, args);
    va_end(args);
    return result;
}
static jbyte wrapper_CallStaticByteMethodV(JNIEnv * env, jclass p0, jmethodID p1, va_list p2) {    return ((JniEnvProxy::Env0*)env)->next->CallStaticByteMethodV(p0, p1, p2);}
static jbyte wrapper_CallStaticByteMethodA(JNIEnv * env, jclass p0, jmethodID p1, jvalue * p2) {    return ((JniEnvProxy::Env0*)env)->next->CallStaticByteMethodA(p0, p1, p2);}
static jchar wrapper_CallStaticCharMethodV(JNIEnv * env, jclass p0, jmethodID p1, va_list p2);
static jchar wrapper_CallStaticCharMethod(JNIEnv * env, jclass p0, jmethodID p1, ...) {
    va_list args; va_start(args, p1);
    jchar result= wrapper_CallStaticCharMethodV(env, p0, p1, args);
    va_end(args);
    return result;
}
static jchar wrapper_CallStaticCharMethodV(JNIEnv * env, jclass p0, jmethodID p1, va_list p2) {    return ((JniEnvProxy::Env0*)env)->next->CallStaticCharMethodV(p0, p1, p2);}
static jchar wrapper_CallStaticCharMethodA(JNIEnv * env, jclass p0, jmethodID p1, jvalue * p2) {    return ((JniEnvProxy::Env0*)env)->next->CallStaticCharMethodA(p0, p1, p2);}
static jshort wrapper_CallStaticShortMethodV(JNIEnv * env, jclass p0, jmethodID p1, va_list p2);
static jshort wrapper_CallStaticShortMethod(JNIEnv * env, jclass p0, jmethodID p1, ...) {
    va_list args; va_start(args, p1);
    jshort result= wrapper_CallStaticShortMethodV(env, p0, p1, args);
    va_end(args);
    return result;
}
static jshort wrapper_CallStaticShortMethodV(JNIEnv * env, jclass p0, jmethodID p1, va_list p2) {    return ((JniEnvProxy::Env0*)env)->next->CallStaticShortMethodV(p0, p1, p2);}
static jshort wrapper_CallStaticShortMethodA(JNIEnv * env, jclass p0, jmethodID p1, jvalue * p2) {    return ((JniEnvProxy::Env0*)env)->next->CallStaticShortMethodA(p0, p1, p2);}
static jint wrapper_CallStaticIntMethodV(JNIEnv * env, jclass p0, jmethodID p1, va_list p2);
static jint wrapper_CallStaticIntMethod(JNIEnv * env, jclass p0, jmethodID p1, ...) {
    va_list args; va_start(args, p1);
    jint result= wrapper_CallStaticIntMethodV(env, p0, p1, args);
    va_end(args);
    return result;
}
static jint wrapper_CallStaticIntMethodV(JNIEnv * env, jclass p0, jmethodID p1, va_list p2) {    return ((JniEnvProxy::Env0*)env)->next->CallStaticIntMethodV(p0, p1, p2);}
static jint wrapper_CallStaticIntMethodA(JNIEnv * env, jclass p0, jmethodID p1, jvalue * p2) {    return ((JniEnvProxy::Env0*)env)->next->CallStaticIntMethodA(p0, p1, p2);}
static jlong wrapper_CallStaticLongMethodV(JNIEnv * env, jclass p0, jmethodID p1, va_list p2);
static jlong wrapper_CallStaticLongMethod(JNIEnv * env, jclass p0, jmethodID p1, ...) {
    va_list args; va_start(args, p1);
    jlong result= wrapper_CallStaticLongMethodV(env, p0, p1, args);
    va_end(args);
    return result;
}
static jlong wrapper_CallStaticLongMethodV(JNIEnv * env, jclass p0, jmethodID p1, va_list p2) {    return ((JniEnvProxy::Env0*)env)->next->CallStaticLongMethodV(p0, p1, p2);}
static jlong wrapper_CallStaticLongMethodA(JNIEnv * env, jclass p0, jmethodID p1, jvalue * p2) {    return ((JniEnvProxy::Env0*)env)->next->CallStaticLongMethodA(p0, p1, p2);}
static jfloat wrapper_CallStaticFloatMethodV(JNIEnv * env, jclass p0, jmethodID p1, va_list p2);
static jfloat wrapper_CallStaticFloatMethod(JNIEnv * env, jclass p0, jmethodID p1, ...) {
    va_list args; va_start(args, p1);
    jfloat result= wrapper_CallStaticFloatMethodV(env, p0, p1, args);
    va_end(args);
    return result;
}
static jfloat wrapper_CallStaticFloatMethodV(JNIEnv * env, jclass p0, jmethodID p1, va_list p2) {    return ((JniEnvProxy::Env0*)env)->next->CallStaticFloatMethodV(p0, p1, p2);}
static jfloat wrapper_CallStaticFloatMethodA(JNIEnv * env, jclass p0, jmethodID p1, jvalue * p2) {    return ((JniEnvProxy::Env0*)env)->next->CallStaticFloatMethodA(p0, p1, p2);}
static jdouble wrapper_CallStaticDoubleMethodV(JNIEnv * env, jclass p0, jmethodID p1, va_list p2);
static jdouble wrapper_CallStaticDoubleMethod(JNIEnv * env, jclass p0, jmethodID p1, ...) {
    va_list args; va_start(args, p1);
    jdouble result= wrapper_CallStaticDoubleMethodV(env, p0, p1, args);
    va_end(args);
    return result;
}
static jdouble wrapper_CallStaticDoubleMethodV(JNIEnv * env, jclass p0, jmethodID p1, va_list p2) {    return ((JniEnvProxy::Env0*)env)->next->CallStaticDoubleMethodV(p0, p1, p2);}
static jdouble wrapper_CallStaticDoubleMethodA(JNIEnv * env, jclass p0, jmethodID p1, jvalue * p2) {    return ((JniEnvProxy::Env0*)env)->next->CallStaticDoubleMethodA(p0, p1, p2);}
static void wrapper_CallStaticVoidMethodV(JNIEnv * env, jclass p0, jmethodID p1, va_list p2);
static void wrapper_CallStaticVoidMethod(JNIEnv * env, jclass p0, jmethodID p1, ...) {
    va_list args; va_start(args, p1);
    wrapper_CallStaticVoidMethodV(env, p0, p1, args);
    va_end(args);
}
static void wrapper_CallStaticVoidMethodV(JNIEnv * env, jclass p0, jmethodID p1, va_list p2) {    ((JniEnvProxy::Env0*)env)->next->CallStaticVoidMethodV(p0, p1, p2);}
static void wrapper_CallStaticVoidMethodA(JNIEnv * env, jclass p0, jmethodID p1, jvalue * p2) {    ((JniEnvProxy::Env0*)env)->next->CallStaticVoidMethodA(p0, p1, p2);}
static jfieldID wrapper_GetStaticFieldID(JNIEnv * env, jclass p0, const char * p1, const char * p2) {    return ((JniEnvProxy::Env0*)env)->next->GetStaticFieldID(p0, p1, p2);}
static jobject wrapper_GetStaticObjectField(JNIEnv * env, jclass p0, jfieldID p1) {    return ((JniEnvProxy::Env0*)env)->next->GetStaticObjectField(p0, p1);}
static jboolean wrapper_GetStaticBooleanField(JNIEnv * env, jclass p0, jfieldID p1) {    return ((JniEnvProxy::Env0*)env)->next->GetStaticBooleanField(p0, p1);}
static jbyte wrapper_GetStaticByteField(JNIEnv * env, jclass p0, jfieldID p1) {    return ((JniEnvProxy::Env0*)env)->next->GetStaticByteField(p0, p1);}
static jchar wrapper_GetStaticCharField(JNIEnv * env, jclass p0, jfieldID p1) {    return ((JniEnvProxy::Env0*)env)->next->GetStaticCharField(p0, p1);}
static jshort wrapper_GetStaticShortField(JNIEnv * env, jclass p0, jfieldID p1) {    return ((JniEnvProxy::Env0*)env)->next->GetStaticShortField(p0, p1);}
static jint wrapper_GetStaticIntField(JNIEnv * env, jclass p0, jfieldID p1) {    return ((JniEnvProxy::Env0*)env)->next->GetStaticIntField(p0, p1);}
static jlong wrapper_GetStaticLongField(JNIEnv * env, jclass p0, jfieldID p1) {    return ((JniEnvProxy::Env0*)env)->next->GetStaticLongField(p0, p1);}
static jfloat wrapper_GetStaticFloatField(JNIEnv * env, jclass p0, jfieldID p1) {    return ((JniEnvProxy::Env0*)env)->next->GetStaticFloatField(p0, p1);}
static jdouble wrapper_GetStaticDoubleField(JNIEnv * env, jclass p0, jfieldID p1) {    return ((JniEnvProxy::Env0*)env)->next->GetStaticDoubleField(p0, p1);}
static void wrapper_SetStaticObjectField(JNIEnv * env, jclass p0, jfieldID p1, jobject p2) {    ((JniEnvProxy::Env0*)env)->next->SetStaticObjectField(p0, p1, p2);}
static void wrapper_SetStaticBooleanField(JNIEnv * env, jclass p0, jfieldID p1, jboolean p2) {    ((JniEnvProxy::Env0*)env)->next->SetStaticBooleanField(p0, p1, p2);}
static void wrapper_SetStaticByteField(JNIEnv * env, jclass p0, jfieldID p1, jbyte p2) {    ((JniEnvProxy::Env0*)env)->next->SetStaticByteField(p0, p1, p2);}
static void wrapper_SetStaticCharField(JNIEnv * env, jclass p0, jfieldID p1, jchar p2) {    ((JniEnvProxy::Env0*)env)->next->SetStaticCharField(p0, p1, p2);}
static void wrapper_SetStaticShortField(JNIEnv * env, jclass p0, jfieldID p1, jshort p2) {    ((JniEnvProxy::Env0*)env)->next->SetStaticShortField(p0, p1, p2);}
static void wrapper_SetStaticIntField(JNIEnv * env, jclass p0, jfieldID p1, jint p2) {    ((JniEnvProxy::Env0*)env)->next->SetStaticIntField(p0, p1, p2);}
static void wrapper_SetStaticLongField(JNIEnv * env, jclass p0, jfieldID p1, jlong p2) {    ((JniEnvProxy::Env0*)env)->next->SetStaticLongField(p0, p1, p2);}
static void wrapper_SetStaticFloatField(JNIEnv * env, jclass p0, jfieldID p1, jfloat p2) {    ((JniEnvProxy::Env0*)env)->next->SetStaticFloatField(p0, p1, p2);}
static void wrapper_SetStaticDoubleField(JNIEnv * env, jclass p0, jfieldID p1, jdouble p2) {    ((JniEnvProxy::Env0*)env)->next->SetStaticDoubleField(p0, p1, p2);}
static jstring wrapper_NewString(JNIEnv * env, const jchar * p0, jsize p1) {    return ((JniEnvProxy::Env0*)env)->next->NewString(p0, p1);}
static jsize wrapper_GetStringLength(JNIEnv * env, jstring p0) {    return ((JniEnvProxy::Env0*)env)->next->GetStringLength(p0);}
static const jchar * wrapper_GetStringChars(JNIEnv * env, jstring p0, jboolean * p1) {    return ((JniEnvProxy::Env0*)env)->next->GetStringChars(p0, p1);}
static void wrapper_ReleaseStringChars(JNIEnv * env, jstring p0, const jchar * p1) {    ((JniEnvProxy::Env0*)env)->next->ReleaseStringChars(p0, p1);}
static jstring wrapper_NewStringUTF(JNIEnv * env, const char * p0) {    return ((JniEnvProxy::Env0*)env)->next->NewStringUTF(p0);}
static jsize wrapper_GetStringUTFLength(JNIEnv * env, jstring p0) {    return ((JniEnvProxy::Env0*)env)->next->GetStringUTFLength(p0);}
static const char * wrapper_GetStringUTFChars(JNIEnv * env, jstring p0, jboolean * p1) {    return ((JniEnvProxy::Env0*)env)->next->GetStringUTFChars(p0, p1);}
static void wrapper_ReleaseStringUTFChars(JNIEnv * env, jstring p0, const char * p1) {    ((JniEnvProxy::Env0*)env)->next->ReleaseStringUTFChars(p0, p1);}
static jsize wrapper_GetArrayLength(JNIEnv * env, jarray p0) {    return ((JniEnvProxy::Env0*)env)->next->GetArrayLength(p0);}
static jobjectArray wrapper_NewObjectArray(JNIEnv * env, jsize p0, jclass p1, jobject p2) {    return ((JniEnvProxy::Env0*)env)->next->NewObjectArray(p0, p1, p2);}
static jobject wrapper_GetObjectArrayElement(JNIEnv * env, jobjectArray p0, jsize p1) {    return ((JniEnvProxy::Env0*)env)->next->GetObjectArrayElement(p0, p1);}
static void wrapper_SetObjectArrayElement(JNIEnv * env, jobjectArray p0, jsize p1, jobject p2) {    ((JniEnvProxy::Env0*)env)->next->SetObjectArrayElement(p0, p1, p2);}
static jbooleanArray wrapper_NewBooleanArray(JNIEnv * env, jsize p0) {    return ((JniEnvProxy::Env0*)env)->next->NewBooleanArray(p0);}
static jbyteArray wrapper_NewByteArray(JNIEnv * env, jsize p0) {    return ((JniEnvProxy::Env0*)env)->next->NewByteArray(p0);}
static jcharArray wrapper_NewCharArray(JNIEnv * env, jsize p0) {    return ((JniEnvProxy::Env0*)env)->next->NewCharArray(p0);}
static jshortArray wrapper_NewShortArray(JNIEnv * env, jsize p0) {    return ((JniEnvProxy::Env0*)env)->next->NewShortArray(p0);}
static jintArray wrapper_NewIntArray(JNIEnv * env, jsize p0) {    return ((JniEnvProxy::Env0*)env)->next->NewIntArray(p0);}
static jlongArray wrapper_NewLongArray(JNIEnv * env, jsize p0) {    return ((JniEnvProxy::Env0*)env)->next->NewLongArray(p0);}
static jfloatArray wrapper_NewFloatArray(JNIEnv * env, jsize p0) {    return ((JniEnvProxy::Env0*)env)->next->NewFloatArray(p0);}
static jdoubleArray wrapper_NewDoubleArray(JNIEnv * env, jsize p0) {    return ((JniEnvProxy::Env0*)env)->next->NewDoubleArray(p0);}
static jboolean * wrapper_GetBooleanArrayElements(JNIEnv * env, jbooleanArray p0, jboolean * p1) {    return ((JniEnvProxy::Env0*)env)->next->GetBooleanArrayElements(p0, p1);}
static jbyte * wrapper_GetByteArrayElements(JNIEnv * env, jbyteArray p0, jboolean * p1) {    return ((JniEnvProxy::Env0*)env)->next->GetByteArrayElements(p0, p1);}
static jchar * wrapper_GetCharArrayElements(JNIEnv * env, jcharArray p0, jboolean * p1) {    return ((JniEnvProxy::Env0*)env)->next->GetCharArrayElements(p0, p1);}
static jshort * wrapper_GetShortArrayElements(JNIEnv * env, jshortArray p0, jboolean * p1) {    return ((JniEnvProxy::Env0*)env)->next->GetShortArrayElements(p0, p1);}
static jint * wrapper_GetIntArrayElements(JNIEnv * env, jintArray p0, jboolean * p1) {    return ((JniEnvProxy::Env0*)env)->next->GetIntArrayElements(p0, p1);}
static jlong * wrapper_GetLongArrayElements(JNIEnv * env, jlongArray p0, jboolean * p1) {    return ((JniEnvProxy::Env0*)env)->next->GetLongArrayElements(p0, p1);}
static jfloat * wrapper_GetFloatArrayElements(JNIEnv * env, jfloatArray p0, jboolean * p1) {    return ((JniEnvProxy::Env0*)env)->next->GetFloatArrayElements(p0, p1);}
static jdouble * wrapper_GetDoubleArrayElements(JNIEnv * env, jdoubleArray p0, jboolean * p1) {    return ((JniEnvProxy::Env0*)env)->next->GetDoubleArrayElements(p0, p1);}
static void wrapper_ReleaseBooleanArrayElements(JNIEnv * env, jbooleanArray p0, jboolean * p1, jint p2) {    ((JniEnvProxy::Env0*)env)->next->ReleaseBooleanArrayElements(p0, p1, p2);}
static void wrapper_ReleaseByteArrayElements(JNIEnv * env, jbyteArray p0, jbyte * p1, jint p2) {    ((JniEnvProxy::Env0*)env)->next->ReleaseByteArrayElements(p0, p1, p2);}
static void wrapper_ReleaseCharArrayElements(JNIEnv * env, jcharArray p0, jchar * p1, jint p2) {    ((JniEnvProxy::Env0*)env)->next->ReleaseCharArrayElements(p0, p1, p2);}
static void wrapper_ReleaseShortArrayElements(JNIEnv * env, jshortArray p0, jshort * p1, jint p2) {    ((JniEnvProxy::Env0*)env)->next->ReleaseShortArrayElements(p0, p1, p2);}
static void wrapper_ReleaseIntArrayElements(JNIEnv * env, jintArray p0, jint * p1, jint p2) {    ((JniEnvProxy::Env0*)env)->next->ReleaseIntArrayElements(p0, p1, p2);}
static void wrapper_ReleaseLongArrayElements(JNIEnv * env, jlongArray p0, jlong * p1, jint p2) {    ((JniEnvProxy::Env0*)env)->next->ReleaseLongArrayElements(p0, p1, p2);}
static void wrapper_ReleaseFloatArrayElements(JNIEnv * env, jfloatArray p0, jfloat * p1, jint p2) {    ((JniEnvProxy::Env0*)env)->next->ReleaseFloatArrayElements(p0, p1, p2);}
static void wrapper_ReleaseDoubleArrayElements(JNIEnv * env, jdoubleArray p0, jdouble * p1, jint p2) {    ((JniEnvProxy::Env0*)env)->next->ReleaseDoubleArrayElements(p0, p1, p2);}
static void wrapper_GetBooleanArrayRegion(JNIEnv * env, jbooleanArray p0, jsize p1, jsize p2, jboolean * p3) {    ((JniEnvProxy::Env0*)env)->next->GetBooleanArrayRegion(p0, p1, p2, p3);}
static void wrapper_GetByteArrayRegion(JNIEnv * env, jbyteArray p0, jsize p1, jsize p2, jbyte * p3) {    ((JniEnvProxy::Env0*)env)->next->GetByteArrayRegion(p0, p1, p2, p3);}
static void wrapper_GetCharArrayRegion(JNIEnv * env, jcharArray p0, jsize p1, jsize p2, jchar * p3) {    ((JniEnvProxy::Env0*)env)->next->GetCharArrayRegion(p0, p1, p2, p3);}
static void wrapper_GetShortArrayRegion(JNIEnv * env, jshortArray p0, jsize p1, jsize p2, jshort * p3) {    ((JniEnvProxy::Env0*)env)->next->GetShortArrayRegion(p0, p1, p2, p3);}
static void wrapper_GetIntArrayRegion(JNIEnv * env, jintArray p0, jsize p1, jsize p2, jint * p3) {    ((JniEnvProxy::Env0*)env)->next->GetIntArrayRegion(p0, p1, p2, p3);}
static void wrapper_GetLongArrayRegion(JNIEnv * env, jlongArray p0, jsize p1, jsize p2, jlong * p3) {    ((JniEnvProxy::Env0*)env)->next->GetLongArrayRegion(p0, p1, p2, p3);}
static void wrapper_GetFloatArrayRegion(JNIEnv * env, jfloatArray p0, jsize p1, jsize p2, jfloat * p3) {    ((JniEnvProxy::Env0*)env)->next->GetFloatArrayRegion(p0, p1, p2, p3);}
static void wrapper_GetDoubleArrayRegion(JNIEnv * env, jdoubleArray p0, jsize p1, jsize p2, jdouble * p3) {    ((JniEnvProxy::Env0*)env)->next->GetDoubleArrayRegion(p0, p1, p2, p3);}
static void wrapper_SetBooleanArrayRegion(JNIEnv * env, jbooleanArray p0, jsize p1, jsize p2, const jboolean * p3) {    ((JniEnvProxy::Env0*)env)->next->SetBooleanArrayRegion(p0, p1, p2, p3);}
static void wrapper_SetByteArrayRegion(JNIEnv * env, jbyteArray p0, jsize p1, jsize p2, const jbyte * p3) {    ((JniEnvProxy::Env0*)env)->next->SetByteArrayRegion(p0, p1, p2, p3);}
static void wrapper_SetCharArrayRegion(JNIEnv * env, jcharArray p0, jsize p1, jsize p2, const jchar * p3) {    ((JniEnvProxy::Env0*)env)->next->SetCharArrayRegion(p0, p1, p2, p3);}
static void wrapper_SetShortArrayRegion(JNIEnv * env, jshortArray p0, jsize p1, jsize p2, const jshort * p3) {    ((JniEnvProxy::Env0*)env)->next->SetShortArrayRegion(p0, p1, p2, p3);}
static void wrapper_SetIntArrayRegion(JNIEnv * env, jintArray p0, jsize p1, jsize p2, const jint * p3) {    ((JniEnvProxy::Env0*)env)->next->SetIntArrayRegion(p0, p1, p2, p3);}
static void wrapper_SetLongArrayRegion(JNIEnv * env, jlongArray p0, jsize p1, jsize p2, const jlong * p3) {    ((JniEnvProxy::Env0*)env)->next->SetLongArrayRegion(p0, p1, p2, p3);}
static void wrapper_SetFloatArrayRegion(JNIEnv * env, jfloatArray p0, jsize p1, jsize p2, const jfloat * p3) {    ((JniEnvProxy::Env0*)env)->next->SetFloatArrayRegion(p0, p1, p2, p3);}
static void wrapper_SetDoubleArrayRegion(JNIEnv * env, jdoubleArray p0, jsize p1, jsize p2, const jdouble * p3) {    ((JniEnvProxy::Env0*)env)->next->SetDoubleArrayRegion(p0, p1, p2, p3);}
static jint wrapper_RegisterNatives(JNIEnv * env, jclass p0, const JNINativeMethod * p1, jint p2) {    return ((JniEnvProxy::Env0*)env)->next->RegisterNatives(p0, p1, p2);}
static jint wrapper_UnregisterNatives(JNIEnv * env, jclass p0) {    return ((JniEnvProxy::Env0*)env)->next->UnregisterNatives(p0);}
static jint wrapper_MonitorEnter(JNIEnv * env, jobject p0) {    return ((JniEnvProxy::Env0*)env)->next->MonitorEnter(p0);}
static jint wrapper_MonitorExit(JNIEnv * env, jobject p0) {    return ((JniEnvProxy::Env0*)env)->next->MonitorExit(p0);}
#if 0
static jint wrapper_GetJavaVM(JNIEnv * env, JavaVM ** p0) {    return ((JniEnvProxy::Env0*)env)->next->GetJavaVM(p0);}
#else
static jint wrapper_GetJavaVM(JNIEnv * env, JavaVM ** p0) {    *p0= ((JniEnvProxy::Env0*)env)->owner->GetProxyedVM(); return 0; }
#endif
static void wrapper_GetStringRegion(JNIEnv * env, jstring p0, jsize p1, jsize p2, jchar * p3) {    ((JniEnvProxy::Env0*)env)->next->GetStringRegion(p0, p1, p2, p3);}
static void wrapper_GetStringUTFRegion(JNIEnv * env, jstring p0, jsize p1, jsize p2, char * p3) {    ((JniEnvProxy::Env0*)env)->next->GetStringUTFRegion(p0, p1, p2, p3);}
static void * wrapper_GetPrimitiveArrayCritical(JNIEnv * env, jarray p0, jboolean * p1) {    return ((JniEnvProxy::Env0*)env)->next->GetPrimitiveArrayCritical(p0, p1);}
static void wrapper_ReleasePrimitiveArrayCritical(JNIEnv * env, jarray p0, void * p1, jint p2) {    ((JniEnvProxy::Env0*)env)->next->ReleasePrimitiveArrayCritical(p0, p1, p2);}
static const jchar * wrapper_GetStringCritical(JNIEnv * env, jstring p0, jboolean * p1) {    return ((JniEnvProxy::Env0*)env)->next->GetStringCritical(p0, p1);}
static void wrapper_ReleaseStringCritical(JNIEnv * env, jstring p0, const jchar * p1) {    ((JniEnvProxy::Env0*)env)->next->ReleaseStringCritical(p0, p1);}
static jweak wrapper_NewWeakGlobalRef(JNIEnv * env, jobject p0) {    return ((JniEnvProxy::Env0*)env)->next->NewWeakGlobalRef(p0);}
static void wrapper_DeleteWeakGlobalRef(JNIEnv * env, jweak p0) {    ((JniEnvProxy::Env0*)env)->next->DeleteWeakGlobalRef(p0);}
static jboolean wrapper_ExceptionCheck(JNIEnv * env) {    return ((JniEnvProxy::Env0*)env)->next->ExceptionCheck();}
static jobject wrapper_NewDirectByteBuffer(JNIEnv * env, void * p0, jlong p1) {    return ((JniEnvProxy::Env0*)env)->next->NewDirectByteBuffer(p0, p1);}
static void * wrapper_GetDirectBufferAddress(JNIEnv * env, jobject p0) {    return ((JniEnvProxy::Env0*)env)->next->GetDirectBufferAddress(p0);}
static jlong wrapper_GetDirectBufferCapacity(JNIEnv * env, jobject p0) {    return ((JniEnvProxy::Env0*)env)->next->GetDirectBufferCapacity(p0);}
static jobjectRefType wrapper_GetObjectRefType(JNIEnv * env, jobject p0) {    return ((JniEnvProxy::Env0*)env)->next->GetObjectRefType(p0);}
// ================
const void* gWrapperJniNativeInterface[] = {
   NULL,
   NULL,
   NULL,
   NULL,
(void*)   wrapper_GetVersion,
(void*)   wrapper_DefineClass,
(void*)   wrapper_FindClass,
(void*)   wrapper_FromReflectedMethod,
(void*)   wrapper_FromReflectedField,
(void*)   wrapper_ToReflectedMethod,
(void*)   wrapper_GetSuperclass,
(void*)   wrapper_IsAssignableFrom,
(void*)   wrapper_ToReflectedField,
(void*)   wrapper_Throw,
(void*)   wrapper_ThrowNew,
(void*)   wrapper_ExceptionOccurred,
(void*)   wrapper_ExceptionDescribe,
(void*)   wrapper_ExceptionClear,
(void*)   wrapper_FatalError,
(void*)   wrapper_PushLocalFrame,
(void*)   wrapper_PopLocalFrame,
(void*)   wrapper_NewGlobalRef,
(void*)   wrapper_DeleteGlobalRef,
(void*)   wrapper_DeleteLocalRef,
(void*)   wrapper_IsSameObject,
(void*)   wrapper_NewLocalRef,
(void*)   wrapper_EnsureLocalCapacity,
(void*)   wrapper_AllocObject,
(void*)   wrapper_NewObject,
(void*)   wrapper_NewObjectV,
(void*)   wrapper_NewObjectA,
(void*)   wrapper_GetObjectClass,
(void*)   wrapper_IsInstanceOf,
(void*)   wrapper_GetMethodID,
(void*)   wrapper_CallObjectMethod,
(void*)   wrapper_CallObjectMethodV,
(void*)   wrapper_CallObjectMethodA,
(void*)   wrapper_CallBooleanMethod,
(void*)   wrapper_CallBooleanMethodV,
(void*)   wrapper_CallBooleanMethodA,
(void*)   wrapper_CallByteMethod,
(void*)   wrapper_CallByteMethodV,
(void*)   wrapper_CallByteMethodA,
(void*)   wrapper_CallCharMethod,
(void*)   wrapper_CallCharMethodV,
(void*)   wrapper_CallCharMethodA,
(void*)   wrapper_CallShortMethod,
(void*)   wrapper_CallShortMethodV,
(void*)   wrapper_CallShortMethodA,
(void*)   wrapper_CallIntMethod,
(void*)   wrapper_CallIntMethodV,
(void*)   wrapper_CallIntMethodA,
(void*)   wrapper_CallLongMethod,
(void*)   wrapper_CallLongMethodV,
(void*)   wrapper_CallLongMethodA,
(void*)   wrapper_CallFloatMethod,
(void*)   wrapper_CallFloatMethodV,
(void*)   wrapper_CallFloatMethodA,
(void*)   wrapper_CallDoubleMethod,
(void*)   wrapper_CallDoubleMethodV,
(void*)   wrapper_CallDoubleMethodA,
(void*)   wrapper_CallVoidMethod,
(void*)   wrapper_CallVoidMethodV,
(void*)   wrapper_CallVoidMethodA,
(void*)   wrapper_CallNonvirtualObjectMethod,
(void*)   wrapper_CallNonvirtualObjectMethodV,
(void*)   wrapper_CallNonvirtualObjectMethodA,
(void*)   wrapper_CallNonvirtualBooleanMethod,
(void*)   wrapper_CallNonvirtualBooleanMethodV,
(void*)   wrapper_CallNonvirtualBooleanMethodA,
(void*)   wrapper_CallNonvirtualByteMethod,
(void*)   wrapper_CallNonvirtualByteMethodV,
(void*)   wrapper_CallNonvirtualByteMethodA,
(void*)   wrapper_CallNonvirtualCharMethod,
(void*)   wrapper_CallNonvirtualCharMethodV,
(void*)   wrapper_CallNonvirtualCharMethodA,
(void*)   wrapper_CallNonvirtualShortMethod,
(void*)   wrapper_CallNonvirtualShortMethodV,
(void*)   wrapper_CallNonvirtualShortMethodA,
(void*)   wrapper_CallNonvirtualIntMethod,
(void*)   wrapper_CallNonvirtualIntMethodV,
(void*)   wrapper_CallNonvirtualIntMethodA,
(void*)   wrapper_CallNonvirtualLongMethod,
(void*)   wrapper_CallNonvirtualLongMethodV,
(void*)   wrapper_CallNonvirtualLongMethodA,
(void*)   wrapper_CallNonvirtualFloatMethod,
(void*)   wrapper_CallNonvirtualFloatMethodV,
(void*)   wrapper_CallNonvirtualFloatMethodA,
(void*)   wrapper_CallNonvirtualDoubleMethod,
(void*)   wrapper_CallNonvirtualDoubleMethodV,
(void*)   wrapper_CallNonvirtualDoubleMethodA,
(void*)   wrapper_CallNonvirtualVoidMethod,
(void*)   wrapper_CallNonvirtualVoidMethodV,
(void*)   wrapper_CallNonvirtualVoidMethodA,
(void*)   wrapper_GetFieldID,
(void*)   wrapper_GetObjectField,
(void*)   wrapper_GetBooleanField,
(void*)   wrapper_GetByteField,
(void*)   wrapper_GetCharField,
(void*)   wrapper_GetShortField,
(void*)   wrapper_GetIntField,
(void*)   wrapper_GetLongField,
(void*)   wrapper_GetFloatField,
(void*)   wrapper_GetDoubleField,
(void*)   wrapper_SetObjectField,
(void*)   wrapper_SetBooleanField,
(void*)   wrapper_SetByteField,
(void*)   wrapper_SetCharField,
(void*)   wrapper_SetShortField,
(void*)   wrapper_SetIntField,
(void*)   wrapper_SetLongField,
(void*)   wrapper_SetFloatField,
(void*)   wrapper_SetDoubleField,
(void*)   wrapper_GetStaticMethodID,
(void*)   wrapper_CallStaticObjectMethod,
(void*)   wrapper_CallStaticObjectMethodV,
(void*)   wrapper_CallStaticObjectMethodA,
(void*)   wrapper_CallStaticBooleanMethod,
(void*)   wrapper_CallStaticBooleanMethodV,
(void*)   wrapper_CallStaticBooleanMethodA,
(void*)   wrapper_CallStaticByteMethod,
(void*)   wrapper_CallStaticByteMethodV,
(void*)   wrapper_CallStaticByteMethodA,
(void*)   wrapper_CallStaticCharMethod,
(void*)   wrapper_CallStaticCharMethodV,
(void*)   wrapper_CallStaticCharMethodA,
(void*)   wrapper_CallStaticShortMethod,
(void*)   wrapper_CallStaticShortMethodV,
(void*)   wrapper_CallStaticShortMethodA,
(void*)   wrapper_CallStaticIntMethod,
(void*)   wrapper_CallStaticIntMethodV,
(void*)   wrapper_CallStaticIntMethodA,
(void*)   wrapper_CallStaticLongMethod,
(void*)   wrapper_CallStaticLongMethodV,
(void*)   wrapper_CallStaticLongMethodA,
(void*)   wrapper_CallStaticFloatMethod,
(void*)   wrapper_CallStaticFloatMethodV,
(void*)   wrapper_CallStaticFloatMethodA,
(void*)   wrapper_CallStaticDoubleMethod,
(void*)   wrapper_CallStaticDoubleMethodV,
(void*)   wrapper_CallStaticDoubleMethodA,
(void*)   wrapper_CallStaticVoidMethod,
(void*)   wrapper_CallStaticVoidMethodV,
(void*)   wrapper_CallStaticVoidMethodA,
(void*)   wrapper_GetStaticFieldID,
(void*)   wrapper_GetStaticObjectField,
(void*)   wrapper_GetStaticBooleanField,
(void*)   wrapper_GetStaticByteField,
(void*)   wrapper_GetStaticCharField,
(void*)   wrapper_GetStaticShortField,
(void*)   wrapper_GetStaticIntField,
(void*)   wrapper_GetStaticLongField,
(void*)   wrapper_GetStaticFloatField,
(void*)   wrapper_GetStaticDoubleField,
(void*)   wrapper_SetStaticObjectField,
(void*)   wrapper_SetStaticBooleanField,
(void*)   wrapper_SetStaticByteField,
(void*)   wrapper_SetStaticCharField,
(void*)   wrapper_SetStaticShortField,
(void*)   wrapper_SetStaticIntField,
(void*)   wrapper_SetStaticLongField,
(void*)   wrapper_SetStaticFloatField,
(void*)   wrapper_SetStaticDoubleField,
(void*)   wrapper_NewString,
(void*)   wrapper_GetStringLength,
(void*)   wrapper_GetStringChars,
(void*)   wrapper_ReleaseStringChars,
(void*)   wrapper_NewStringUTF,
(void*)   wrapper_GetStringUTFLength,
(void*)   wrapper_GetStringUTFChars,
(void*)   wrapper_ReleaseStringUTFChars,
(void*)   wrapper_GetArrayLength,
(void*)   wrapper_NewObjectArray,
(void*)   wrapper_GetObjectArrayElement,
(void*)   wrapper_SetObjectArrayElement,
(void*)   wrapper_NewBooleanArray,
(void*)   wrapper_NewByteArray,
(void*)   wrapper_NewCharArray,
(void*)   wrapper_NewShortArray,
(void*)   wrapper_NewIntArray,
(void*)   wrapper_NewLongArray,
(void*)   wrapper_NewFloatArray,
(void*)   wrapper_NewDoubleArray,
(void*)   wrapper_GetBooleanArrayElements,
(void*)   wrapper_GetByteArrayElements,
(void*)   wrapper_GetCharArrayElements,
(void*)   wrapper_GetShortArrayElements,
(void*)   wrapper_GetIntArrayElements,
(void*)   wrapper_GetLongArrayElements,
(void*)   wrapper_GetFloatArrayElements,
(void*)   wrapper_GetDoubleArrayElements,
(void*)   wrapper_ReleaseBooleanArrayElements,
(void*)   wrapper_ReleaseByteArrayElements,
(void*)   wrapper_ReleaseCharArrayElements,
(void*)   wrapper_ReleaseShortArrayElements,
(void*)   wrapper_ReleaseIntArrayElements,
(void*)   wrapper_ReleaseLongArrayElements,
(void*)   wrapper_ReleaseFloatArrayElements,
(void*)   wrapper_ReleaseDoubleArrayElements,
(void*)   wrapper_GetBooleanArrayRegion,
(void*)   wrapper_GetByteArrayRegion,
(void*)   wrapper_GetCharArrayRegion,
(void*)   wrapper_GetShortArrayRegion,
(void*)   wrapper_GetIntArrayRegion,
(void*)   wrapper_GetLongArrayRegion,
(void*)   wrapper_GetFloatArrayRegion,
(void*)   wrapper_GetDoubleArrayRegion,
(void*)   wrapper_SetBooleanArrayRegion,
(void*)   wrapper_SetByteArrayRegion,
(void*)   wrapper_SetCharArrayRegion,
(void*)   wrapper_SetShortArrayRegion,
(void*)   wrapper_SetIntArrayRegion,
(void*)   wrapper_SetLongArrayRegion,
(void*)   wrapper_SetFloatArrayRegion,
(void*)   wrapper_SetDoubleArrayRegion,
(void*)   wrapper_RegisterNatives,
(void*)   wrapper_UnregisterNatives,
(void*)   wrapper_MonitorEnter,
(void*)   wrapper_MonitorExit,
(void*)   wrapper_GetJavaVM,
(void*)   wrapper_GetStringRegion,
(void*)   wrapper_GetStringUTFRegion,
(void*)   wrapper_GetPrimitiveArrayCritical,
(void*)   wrapper_ReleasePrimitiveArrayCritical,
(void*)   wrapper_GetStringCritical,
(void*)   wrapper_ReleaseStringCritical,
(void*)   wrapper_NewWeakGlobalRef,
(void*)   wrapper_DeleteWeakGlobalRef,
(void*)   wrapper_ExceptionCheck,
(void*)   wrapper_NewDirectByteBuffer,
(void*)   wrapper_GetDirectBufferAddress,
(void*)   wrapper_GetDirectBufferCapacity,
(void*)   wrapper_GetObjectRefType,
};
