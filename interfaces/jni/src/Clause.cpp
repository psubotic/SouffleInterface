#include<jni.h>
#include "com_soufflelang_souffle_Clause.h"

#include "handle.h"
#include "AstClause.h"

using namespace souffle;

void Java_com_soufflelang_souffle_Clause_init(JNIEnv *env, jobject obj) {
    AstClause* clause = new AstClause();
    setHandle(env, obj, clause);
}

void JNICALL Java_com_soufflelang_souffle_Clause_setHead(JNIEnv* env, jobject obj1, jobject obj2) {
    AstClause *cl = getHandle<AstClause>(env, obj1);
    AstAtom *a = getHandle<AstAtom>(env, obj2);
    cl->setHead(std::unique_ptr<AstAtom>(a));
}

void JNICALL Java_com_soufflelang_souffle_Clause_addToBody(JNIEnv* env, jobject obj1, jobject obj2) {
    AstClause* cl = getHandle<AstClause>(env, obj1);
    AstLiteral* l = getHandle<AstLiteral>(env, obj2);
    cl->addToBody(std::unique_ptr<AstLiteral>(l));
}
