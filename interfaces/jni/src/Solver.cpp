#include<jni.h>
#include "handle.h"

#include "com_soufflelang_souffle_Solver.h"

#include "Interfaces.h"
#include "InterfaceResult.h"

using namespace souffle;

void Java_com_soufflelang_souffle_Solver_init(JNIEnv *env, jobject obj) {
    Flags flags;
    flags.includeOpt="-I.";
    flags.factFileDir = "..";
    flags.outputDir="-";
    InternalInterface* souffle = new InternalInterface(flags);
    setHandle(env, obj, souffle);
}

void Java_com_soufflelang_souffle_Solver_release(JNIEnv *env, jobject obj) {
    InternalInterface* souffle = getHandle<InternalInterface>(env, obj);
    delete souffle;
}

void Java_com_soufflelang_souffle_Solver_parse(JNIEnv* env, jobject obj1, jobject obj2) {
    InternalInterface* souffle = getHandle<InternalInterface>(env, obj1);
    AstBuilder* builder = getHandle<AstBuilder>(env, obj2);
    souffle->init(builder);
}

jobject Java_com_soufflelang_souffle_Solver_executeInterpreter(JNIEnv* env, jobject obj1, jobject input) {
    InternalInterface* souffle = getHandle<InternalInterface>(env, obj1);
    RamData* data = getHandle<RamData>(env, input);
    InterfaceResult* res = souffle->executeInterpreter(data);
    jclass c = env->FindClass("com/soufflelang/souffle/Result");
    if (c == 0) printf("Find class failed.\n");
    jmethodID cnstrctr = env->GetMethodID(c, "<init>", "(J)V");
    if (cnstrctr == 0) printf("Find method failed.\n");
    return env->NewObject(c, cnstrctr, res);
}

jobject Java_com_soufflelang_souffle_Solver_executeCompiler(JNIEnv* env, jobject obj1, jobject input, jstring jname) {
    const char *nname = env->GetStringUTFChars(jname, 0);
    std::string name = std::string(nname);
    InternalInterface* souffle = getHandle<InternalInterface>(env, obj1);
    RamData* data = getHandle<RamData>(env, input);
    assert(name != "");
    InterfaceResult* res = souffle->executeCompiler(data, name);
    jclass c = env->FindClass("com/soufflelang/souffle/Result");
    if (c == 0) printf("Find class failed.\n");
    jmethodID cnstrctr = env->GetMethodID(c, "<init>", "(J)V");
    if (cnstrctr == 0) printf("Find method failed.\n");
    return env->NewObject(c, cnstrctr, res);
}
