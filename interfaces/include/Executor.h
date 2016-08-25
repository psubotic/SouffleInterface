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
    RamEnvironment* e = r.execute(table, *rp, data);
    return new InterfaceResult(e);
  }

  InterfaceResult* executeLoadCompile(RamData* data, std::string& filename) {
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

    if(!existsfile(libname.c_str())) {
      std::cout << "interface: Error! Cannot find library!\n";
      return false;
    }

    void *lib_handle;
    SouffleProgram* (*fn)(const char* p);
    std::string error;
    std::string openfile = "lib"+filename+".so";

    lib_handle = dlopen(openfile.c_str(), RTLD_LAZY);
    if (lib_handle == NULL){
      std::cout << "interface: Error! Cannot find library!\n";
      return NULL;
    }

    void* temp = dlsym(lib_handle, "getInstance");
    if(temp == NULL) {
      std::cout << "interface: Error! Cannot find symbol from lib\n";
      return NULL;
    }

    memcpy(&fn, &temp, sizeof fn);

    SouffleProgram* p = (*fn)(filename.c_str());
    if(p == NULL) {
      std::cout << "interface: Program not found" << "\n";
    }
    std::map<std::string, PrimData*> dmap = data->getDataMap();

    for(auto& m : dmap) { 
      Relation* rel = p->getRelation(m.first);
      if (rel == nullptr) {
        std::cout << "Interface: rel is null, cannot find: " << m.first << "\n";
        continue;
      }

      if (m.second->data.size() == 0) {
        std::cout << "Data empty" << "\n";
        continue;
      }
      for(auto& row : m.second->data) {
        tuple t(rel);
        int i = 0;
        for(auto& r : row){
          if (*rel->getAttrType(i) == 'i')
            t << std::atoi(r.c_str());
          else
            t << r;

          ++i;
        }
        rel->insert(t);
      } 
    }
    std::chrono::steady_clock::time_point runbegin = std::chrono::steady_clock::now();
    assert(p != NULL);
    p->run();
    std::chrono::steady_clock::time_point runend= std::chrono::steady_clock::now();
    std::cout << "Run Duration = " << std::chrono::duration_cast<std::chrono::microseconds>(runend - runbegin).count() <<std::endl;
    dlclose(lib_handle);
    return new InterfaceResult(p);
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
    if(!existsfile(libname.c_str())) {
      std::cout << "interface: Compiling to library\n";
      std::chrono::steady_clock::time_point compilebegin = std::chrono::steady_clock::now();
      RamCompiler r(filename);
      r.compileToLibrary(table, *rp, filename); // compile it to a library
      std::chrono::steady_clock::time_point compileend = std::chrono::steady_clock::now();
      std::cout << "Compilation Duration = " << std::chrono::duration_cast<std::chrono::microseconds>(compileend - compilebegin).count() <<std::endl;
    }

    void *lib_handle;
    SouffleProgram* (*fn)(const char* p);
    std::string error;
    std::string openfile = "lib"+filename+".so";

    lib_handle = dlopen(openfile.c_str(), RTLD_LAZY);
    if (lib_handle == NULL){
      std::cout << "interface: Error! Cannot find library\n";
      return NULL;
    }

    void* temp = dlsym(lib_handle, "getInstance");
    if(temp == NULL) {
      std::cout << "interface: Error! Cannot find symbol from lib\n";
      return NULL;
    }

    memcpy(&fn, &temp, sizeof fn);

    SouffleProgram* p = (*fn)(filename.c_str());
    if(p == NULL) {
      std::cout << "interface: Program not found" << "\n";
    }
    std::map<std::string, PrimData*> dmap = data->getDataMap();

    for(auto& m : dmap) { 
      Relation* rel = p->getRelation(m.first);
      if (rel == nullptr) {
        std::cout << "Interface: rel is null, cannot find: " << m.first << "\n";
        continue;
      }

      if (m.second->data.size() == 0) {
        std::cout << "Data empty" << "\n";
        continue;
      }
      for(auto& row : m.second->data) {
        tuple t(rel);
        int i = 0;
        for(auto& r : row){
          if (*rel->getAttrType(i) == 'i')
            t << std::atoi(r.c_str());
          else
            t << r;

          ++i;
        }
        rel->insert(t);
      } 
    }
    std::chrono::steady_clock::time_point runbegin = std::chrono::steady_clock::now();
    assert(p != NULL);
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


