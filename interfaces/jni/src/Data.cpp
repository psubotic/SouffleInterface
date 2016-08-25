#include "com_soufflelang_souffle_Data.h"
#include "handle.h"
#include "conversion.h"
#include "RamData.h"
#include <iostream>

using namespace souffle;

void Java_com_soufflelang_souffle_Data_init(JNIEnv* env, jobject obj) {
    RamData* data = new RamData();
    setHandle(env, obj, data);
}

void Java_com_soufflelang_souffle_Data_release(JNIEnv* env, jobject obj) {
    RamData* data = getHandle<RamData>(env, obj);
    delete data;
}

jobject Java_com_soufflelang_souffle_Data_merge(JNIEnv* env, jobject d1, jobject d2) {
 
    RamData* data1 = getHandle<RamData>(env, d1);
    RamData* data2 = getHandle<RamData>(env, d2);
    RamData* data3 = data1->merge(data2);

    jclass c = env->FindClass("com/soufflelang/souffle/Data");
    if (c == 0) printf("Find class failed.\n");
    jmethodID cnstrctr = env->GetMethodID(c, "<init>", "(J)V");
    if (cnstrctr == 0) printf("Find method failed.\n");
    return env->NewObject(c, cnstrctr, data3);
}

void Java_com_soufflelang_souffle_Data_addRelationTuple(JNIEnv* env, jobject obj, jstring str, jobject obj2) {
    RamData* data = getHandle<RamData>(env, obj);
    std::vector<std::string> arr = arr2vec(env, obj2);
    std::string name = std::string(env->GetStringUTFChars(str, 0));
    data->addTuple(name, arr);
}

void Java_com_soufflelang_souffle_Data_addRelationData(JNIEnv* env, jobject obj, jstring str, jobject obj2) {
    RamData* data = getHandle<RamData>(env, obj);
    PrimData* pdata = getHandle<PrimData>(env, obj2);
    //std::vector<std::string> arr = arr2vec(env, obj2);
    std::string name = std::string(env->GetStringUTFChars(str, 0));
    data->addTuples(name, pdata);
}
