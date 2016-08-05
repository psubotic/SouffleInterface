#include "Interfaces.h"

using namespace souffle;

void InternalInterface::init(AstBuilder* driver) {
    AstTranslationUnit* translationUnit = driver->getTranslationUnit();
    if(translationUnit == NULL) { 
      std::cout << "no translatiopn unit \n";
      return;
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
        return;
    }

    //translationUnit->getSymbolTable().print(std::cout);

    exec = new Executor(translationUnit->getSymbolTable(), std::move(ramProgram));
}


InterfaceResult* InternalInterface::executeInterpreter(RamData* data) {
  return exec->executeInterpreter(data);
}

InterfaceResult* InternalInterface::executeCompiler(RamData* data, std::string& filename) {
  return exec->executeCompiler(data, filename);
}


