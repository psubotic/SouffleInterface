#include "Interfaces.h"

using namespace souffle;

Executor* InternalInterface::parse(AstBuilder* driver) {
    AstTranslationUnit* translationUnit = driver->getTranslationUnit();
    if(translationUnit == NULL) { 
      std::cout << "no translatiopn unit \n";
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
        transform->apply(*translationUnit);
    }

    /* translate AST to RAM */
    std::unique_ptr<RamStatement> ramProgram = RamTranslator(false).translateProgram(*translationUnit);

    if(ramProgram == nullptr) {
        std::cout << "ram empty \n";
        return NULL;
    }

    return new Executor(translationUnit->getSymbolTable(), std::move(ramProgram));
}

