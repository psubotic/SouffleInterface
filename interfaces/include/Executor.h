/*
 * Souffle - A Datalog Compiler
 * Copyright (c) 2013, 2015, Oracle and/or its affiliates. All rights reserved
 * Licensed under the Universal Permissive License v 1.0 as shown at:
 * - https://opensource.org/licenses/UPL
 * - <souffle root>/licenses/SOUFFLE-UPL.txt
 */

/************************************************************************
 *
 * @file Executor.h
 *
 * Executor for Souffle
 *
 ***********************************************************************/
#pragma once

#include <stdlib.h>
#include <errno.h>
#include <libgen.h>
#include <limits.h>
#include <string.h>
#include <stdarg.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/stat.h>
#include <getopt.h>
#include <config.h>

#include <chrono>
#include <list>
#include <iostream>
#include <fstream>

#include <vector>
#include <map>
#include <string>
#include <iostream>
#include <assert.h>

#include "Util.h"
#include "RamExecutor.h"
#include "RamData.h"
#include "RamStatement.h"

#include "SymbolTable.h"
#include "SouffleInterface.h"
#include "InterfaceResult.h"

// Shared Library loading
#include <dlfcn.h>


namespace souffle {

class Executor {
public:
  Executor(SymbolTable symb, std::unique_ptr<RamStatement> ram) : 
    table(symb), rp(std::move(ram)) 
  {}

  InterfaceResult* executeInterpreter(RamData* data) {
    RamInterpreter r;
    return new InterfaceResult(r.execute(table, *rp, data));
  }

  InterfaceResult* executeCompiler(RamData* data, std::string& filename) {
    //lambda to check if file exists
    auto existsfile = [](const std::string& name) -> bool {
      if (FILE *file = fopen(name.c_str(), "r")) {
        fclose(file);
        return true;
      } else {
        return false;
      }   
    };

    std::string libname = "lib"+ filename + ".so";

    if(!existsfile(libname.c_str())){
      std::cout << "Compiling to library\n";
      std::chrono::steady_clock::time_point compilebegin = std::chrono::steady_clock::now();
      RamCompiler r(filename);
      r.compileToLibrary(table, *rp, filename); // compile it to a library
      std::chrono::steady_clock::time_point compileend = std::chrono::steady_clock::now();
      std::cout << "Compile Duration = " << std::chrono::duration_cast<std::chrono::microseconds>(compileend - compilebegin).count() <<std::endl;

    }

    //std::cout << "lib created\n";

    void *lib_handle;
    SouffleProgram* (*fn)(const char* p);
    std::string error;
    std::string openfile = "lib"+filename+".so";

    lib_handle = dlopen(openfile.c_str(), RTLD_LAZY);
    if (lib_handle == NULL){
      std::cout << "cannot find library\n";
      return NULL;
    }

    //std::cout << "lib opened\n";

    void* temp = dlsym(lib_handle, "getInstance");
    if(temp == NULL)
    {
      std::cout << "error: Cannot find symbol from lib\n";
      return NULL;
    }

    memcpy(&fn, &temp, sizeof fn);
    //std::cout << "instance found\n";

    SouffleProgram* p = (*fn)(filename.c_str());

    //std::cout << "got SouffleProgram\n";
    std::map<std::string, std::vector<std::vector<std::string> > > dmap = data->getDataMap();

    for(auto& m : dmap) { 
      Relation* rel = p->getRelation(m.first);
      if (rel == nullptr) {
        //std::cout << "Warning, relation " << m.first << " not found!!!!\n";
        continue;
      }

      //std::cout << "found rel " << m.first << " of arity " << rel->getArity() << "\n";

      for(auto& row : m.second){
        tuple t(rel);
        for(auto& r : row){
          //std::cout << "adding elem " << r << " to row\n";
          t << r;
        }
        //std::cout << "row inserted\n";
        rel->insert(t);
      } 
    }



    std::chrono::steady_clock::time_point runbegin = std::chrono::steady_clock::now();
    p->run();
    std::chrono::steady_clock::time_point runend= std::chrono::steady_clock::now();
    std::cout << "Run Duration = " << std::chrono::duration_cast<std::chrono::microseconds>(runend - runbegin).count() <<std::endl;

    dlclose(lib_handle);



    return new InterfaceResult(p);
  }

private:
  SymbolTable table;
  std::unique_ptr<RamStatement> rp;
};

}


