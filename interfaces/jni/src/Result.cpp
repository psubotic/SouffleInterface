#include "com_soufflelang_souffle_Result.h"
#include "handle.h"
#include "InterfaceResult.h"
#include <iostream>
#include <jni.h>
#include "conversion.h"

using namespace souffle;

void Java_com_soufflelang_souffle_Result_release(JNIEnv* env, jobject obj) {
    InterfaceResult* data = getHandle<InterfaceResult>(env, obj);
    std::cout << "10\n";
    delete data;
}

jobject Java_com_soufflelang_souffle_Result_getRelationRows(JNIEnv* env, jobject obj, jstring str) {
    std::cout << "1\n";
    std::string name = std::string(env->GetStringUTFChars(str, 0));
    std::cout << "2\n";
    InterfaceResult* intres = getHandle<InterfaceResult>(env, obj);
    std::cout << "3\n";
    std::vector<std::vector<std::string>> vec = intres->getRelationRows(name);
    std::cout << "4\n";

    jclass java_util_ArrayList2 = static_cast<jclass>(env->NewGlobalRef(env->FindClass("java/util/ArrayList")));
    std::cout << "5\n";
    jmethodID java_util_ArrayList_2 = env->GetMethodID(java_util_ArrayList2,"<init>", "(I)V");
    std::cout << "6\n";
    jmethodID java_util_ArrayList_add2 = env->GetMethodID(java_util_ArrayList2, "add", "(Ljava/lang/Object;)Z");
    std::cout << "7\n";
    jobject result2 = env->NewObject(java_util_ArrayList2, java_util_ArrayList_2, (jint) vec.size());
    std::cout << "8\n";

    for (std::vector<std::string>v: vec) {
      env->CallVoidMethod(result2, java_util_ArrayList_add2, vec2arr(env, v));
    }
    std::cout << "9\n";
    return result2;
}
