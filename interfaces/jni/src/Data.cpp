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

void Java_com_soufflelang_souffle_Data_addRelationTuple(JNIEnv* env, jobject obj, jstring str, jobject obj2) {
    RamData* data = getHandle<RamData>(env, obj);
    std::vector<std::string> arr = arr2vec(env, obj2);
    std::string name = std::string(env->GetStringUTFChars(str, 0));
    data->addTuple(name, arr);
}
