#pragma once

#include "AstRelation.h"
#include "SouffleInterface.h"
#include "RamRelation.h"

namespace souffle {

class InterfaceResult {
public:
  InterfaceResult(SouffleProgram* prog)
    :p(prog), e(NULL), ty(RESULT_COMPILER)
  {}

  InterfaceResult(RamEnvironment* env)
    :p(NULL), e(env), ty(RESULT_INTERPRETER)
  {}

  ~InterfaceResult(){
     if (p != NULL){
       delete p;
     }
     if (e != NULL){
       delete e;
     }
   }

protected:

  enum resType{
    RESULT_INTERPRETER,
    RESULT_COMPILER
  };

public:

  std::vector<std::vector<std::string>> getRelationRows(std::string name) {
    std::vector<std::vector<std::string>> vec;

    if (ty == RESULT_INTERPRETER) {
      const RamRelation& res = e->getRelation(name);
      res.store(vec, e->getSymbolTable(), res.getID().getSymbolMask());
      return vec;
    }
    else { // Compiler
      std::cout << "R0\n";
      Relation* res = p->getRelation(name);
      std::cout << "R1\n";
      if(res == NULL){
        std::cout << "relaiton " << name << " not found!!!";
        return vec;
      }
      std::cout << "R2\n";
      for(Relation::iterator it = res->begin(); it != res->end(); ++it) {
        std::vector<std::string> vecinner;
        tuple tu = (*it);
        for(size_t i = 0; i < res->getArity(); ++i){
          std::string val1;
          tu >> val1;
          vecinner.push_back(val1);
        }
        vec.push_back(vecinner);
      }
      std::cout << "R3\n";
      return vec;
    }
  }

private:
  SouffleProgram* p;
  RamEnvironment* e;
  resType ty;
};

}
