#include<jni.h>
#include "handle.h"

#include "com_soufflelang_souffle_Executor.h"

#include "Executor.h"
#include "InterfaceResult.h"

using namespace souffle;

jobject Java_com_soufflelang_souffle_Executor_executeInterpreter(JNIEnv* env, jobject obj1, jobject input) {
    Executor* souffle = getHandle<Executor>(env, obj1);
    RamData* data = getHandle<RamData>(env, input);
    InterfaceResult* res = souffle->executeInterpreter(data);
    jclass c = env->FindClass("com/soufflelang/souffle/Result");
    if (c == 0) printf("Find class failed.\n");
    jmethodID cnstrctr = env->GetMethodID(c, "<init>", "(J)V");
    if (cnstrctr == 0) printf("Find method failed.\n");
    return env->NewObject(c, cnstrctr, res);
}

jobject Java_com_soufflelang_souffle_Executor_executeCompiler(JNIEnv* env, jobject obj1, jobject input, jstring jname) {
    const char *nname = env->GetStringUTFChars(jname, 0);
    std::string name = std::string(nname);
    Executor* souffle = getHandle<Executor>(env, obj1);
    RamData* data = getHandle<RamData>(env, input);
    assert(name != "");
    InterfaceResult* res = souffle->executeCompiler(data, name);
    jclass c = env->FindClass("com/soufflelang/souffle/Result");
    if (c == 0) printf("Find class failed.\n");
    jmethodID cnstrctr = env->GetMethodID(c, "<init>", "(J)V");
    if (cnstrctr == 0) printf("Find method failed.\n");
    return env->NewObject(c, cnstrctr, res);
}
