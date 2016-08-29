#include "Interfaces.h"

using namespace souffle;

Executor* InternalInterface::parse(AstBuilder* driver) {
    LOG(INFO) ENTERCPP("parse");
    AstTranslationUnit* translationUnit = driver->getTranslationUnit();
    if(translationUnit == NULL) { 
      std::cout << "no translation unit \n";
      LOG(ERR) PRE << "No translation unit\n";
      assert(false && "No translation unit\n");
      return NULL;
    }

    // ------- rewriting / optimizations -------------
    std::vector<std::unique_ptr<AstTransformer>> transforms;
    transforms.push_back(std::unique_ptr<AstTransformer>(new ComponentInstantiationTransformer()));
    transforms.push_back(std::unique_ptr<AstTransformer>(new UniqueAggregationVariablesTransformer()));
    transforms.push_back(std::unique_ptr<AstTransformer>(new AstSemanticChecker()));
    transforms.push_back(std::unique_ptr<AstTransformer>(new ResolveAliasesTransformer()));
    transforms.push_back(std::unique_ptr<AstTransformer>(new RemoveRelationCopiesTransformer()));
    transforms.push_back(std::unique_ptr<AstTransformer>(new MaterializeAggregationQueriesTransformer()));
    transforms.push_back(std::unique_ptr<AstTransformer>(new RemoveEmptyRelationsTransformer()));
   
    for (const auto &transform : transforms) {
        LOG(INFO) PRE << "Applying transformation\n";
        transform->apply(*translationUnit);
    }

    /* translate AST to RAM */
    LOG(INFO) PRE << "Translating ram\n";
    std::unique_ptr<RamStatement> ramProgram = RamTranslator(false).translateProgram(*translationUnit);

    if(ramProgram == nullptr) {
        LOG(WARN) PRE << "Ram is empty!\n";
        return NULL;
    }

    LOG(INFO) LEAVECPP;
    return new Executor(translationUnit->getSymbolTable(), std::move(ramProgram));
}

