#include<jni.h>
#include "com_soufflelang_souffle_Program.h"
#include "handle.h"
 
#include "AstBuilder.h"

using namespace souffle;

void Java_com_soufflelang_souffle_Program_init(JNIEnv *env, jobject obj) {
    AstBuilder* builder = new AstBuilder();
    setHandle(env, obj, builder);
}


void Java_com_soufflelang_souffle_Program_addRelation(JNIEnv *env, jobject obj1, jobject obj2) {
    AstBuilder *prog = getHandle<AstBuilder>(env, obj1);
    AstRelation *rel = getHandle<AstRelation>(env, obj2);
    prog->addRelation(rel);
}

void Java_com_soufflelang_souffle_Program_addType(JNIEnv *env, jobject obj1, jobject obj2) {
    AstBuilder *prog = getHandle<AstBuilder>(env, obj1);
    AstType *ty = getHandle<AstType>(env, obj2);
    prog->addType(ty);
}

void Java_com_soufflelang_souffle_Program_addClause(JNIEnv *env, jobject obj1, jobject obj2) {
    AstBuilder *prog = getHandle<AstBuilder>(env, obj1);
    AstClause *cl = getHandle<AstClause>(env, obj2);
    prog->addClause(cl);
}

jstring Java_com_soufflelang_souffle_Program_print(JNIEnv *env, jobject obj1) {
    AstBuilder *prog = getHandle<AstBuilder>(env, obj1);
    std::string res = prog->print();
    return env->NewStringUTF(res.c_str());  
}
