#include "com_soufflelang_souffle_Result.h"
#include "handle.h"
#include "InterfaceResult.h"
#include <iostream>
#include <jni.h>
#include "conversion.h"

using namespace souffle;

void Java_com_soufflelang_souffle_Result_release(JNIEnv* env, jobject obj) {
    InterfaceResult* data = getHandle<InterfaceResult>(env, obj);
    delete data;
}

jobject Java_com_soufflelang_souffle_Result_getRelationNames(JNIEnv* env, jobject obj) {
    InterfaceResult* intres = getHandle<InterfaceResult>(env, obj);
    std::vector<std::string> vec = intres->getRelationNames();

    jclass java_util_ArrayList2 = 
      static_cast<jclass>(env->NewGlobalRef(env->FindClass("java/util/ArrayList")));

    jmethodID java_util_ArrayList_2 = env->GetMethodID(java_util_ArrayList2,"<init>", "(I)V");

    jmethodID java_util_ArrayList_add2 = 
      env->GetMethodID(java_util_ArrayList2, "add", "(Ljava/lang/Object;)Z");

    jobject result = 
      env->NewObject(java_util_ArrayList2, java_util_ArrayList_2, (jint)vec.size());

    for (auto &v: vec) {
      env->CallVoidMethod(result, java_util_ArrayList_add2, env->NewStringUTF(v.c_str()));
    }

    return result;
}

jobject Java_com_soufflelang_souffle_Result_getPrimData(JNIEnv* env, jobject obj, jstring str) {
    std::string name = std::string(env->GetStringUTFChars(str, 0));
    InterfaceResult* intres = getHandle<InterfaceResult>(env, obj);
    PrimData* pdata = intres->getPrimRelation(name);

    // Create PrimData java Object and add pdata to its oconstructor
    jclass c = env->FindClass("com/soufflelang/souffle/PrimData");
    if (c == 0) printf("Find class failed.\n");
    jmethodID cnstrctr = env->GetMethodID(c, "<init>", "(J)V");
    if (cnstrctr == 0) printf("Find method failed.\n");
    return env->NewObject(c, cnstrctr, pdata);
}

jobject Java_com_soufflelang_souffle_Result_getRelationRows(JNIEnv* env, jobject obj, jstring str) {
    std::string name = std::string(env->GetStringUTFChars(str, 0));
    InterfaceResult* intres = getHandle<InterfaceResult>(env, obj);
    PrimData* pdata = intres->getPrimRelation(name);

    jclass java_util_ArrayList2 = 
      static_cast<jclass>(env->NewGlobalRef(env->FindClass("java/util/ArrayList")));

    jmethodID java_util_ArrayList_2 = env->GetMethodID(java_util_ArrayList2,"<init>", "(I)V");

    jmethodID java_util_ArrayList_add2 = 
      env->GetMethodID(java_util_ArrayList2, "add", "(Ljava/lang/Object;)Z");

    if (pdata == NULL) {
       std::cout << "jni: cannot get relation " << name << "\n";
       jobject result = env->NewObject(java_util_ArrayList2, java_util_ArrayList_2, (jint)0);
       return result;
    }

    jobject result2 = 
      env->NewObject(java_util_ArrayList2, java_util_ArrayList_2, (jint)pdata->data.size());

    for (std::vector<std::string>v: pdata->data) {
      env->CallVoidMethod(result2, java_util_ArrayList_add2, vec2arr(env, v));
    }

    return result2;
}
