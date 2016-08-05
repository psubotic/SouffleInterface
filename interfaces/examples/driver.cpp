#include "Interfaces.h"
#include <array>
#include <vector>
#include <string>
#include "AstBuilder.h"

using namespace souffle; 

int main(int argc, char **argv) {
 
    Flags flags;
    flags.includeOpt="-I.";
    flags.factFileDir = ".";
    flags.outputDir=".";
    flags.filenames = "./insert_print.dl";
    assert(existFile(flags.filenames));
    flags.programName = which(argv[0]);

    // Ast Driver
    AstBuilder driver;
    //Type
    AstTypeIdentifier tid("Node");
    AstType* tnode = new AstPrimitiveType(tid, false);
    driver.addType(tnode);

    // Decl 
    AstRelationIdentifier r1id("edge");
    AstRelation* r1 = new AstRelation();
    r1->setName(r1id);
    r1->addAttribute(std::unique_ptr<AstAttribute>(new AstAttribute("node1", tid)));
    r1->addAttribute(std::unique_ptr<AstAttribute>(new AstAttribute("node2", tid)));
    r1->setQualifier(1);

    driver.addRelation(r1);

    AstRelationIdentifier r2id("path");
    AstRelation* r2 = new AstRelation();
    r2->setName(r2id);
    r2->addAttribute(std::unique_ptr<AstAttribute>(new AstAttribute("node1", tid)));
    r2->addAttribute(std::unique_ptr<AstAttribute>(new AstAttribute("node2", tid)));
    r2->setQualifier(2);

    driver.addRelation(r2);

    // Clause 1
    std::unique_ptr<AstAtom> a1(new AstAtom("path"));

    a1->addArgument(std::unique_ptr<AstVariable>(new AstVariable("X")));
    a1->addArgument(std::unique_ptr<AstVariable>(new AstVariable("Y")));

    std::unique_ptr<AstAtom> b1 = std::unique_ptr<AstAtom>(new AstAtom("path"));
    b1->addArgument(std::unique_ptr<AstVariable>(new AstVariable("X")));
    b1->addArgument(std::unique_ptr<AstVariable>(new AstVariable("Z")));

    std::unique_ptr<AstAtom> b2 = std::unique_ptr<AstAtom>(new AstAtom("edge"));
    b2->addArgument(std::unique_ptr<AstVariable>(new AstVariable("Z")));
    b2->addArgument(std::unique_ptr<AstVariable>(new AstVariable("Y")));

    AstClause* c1 = new AstClause();
    c1->setHead(std::move(a1));
    c1->addToBody(std::move(b1));
    c1->addToBody(std::move(b2));

    driver.addClause(c1);

    // Clause 2
    std::unique_ptr<AstAtom> aa1(new AstAtom("path"));
    aa1->addArgument(std::unique_ptr<AstVariable>(new AstVariable("X")));
    aa1->addArgument(std::unique_ptr<AstVariable>(new AstVariable("Y")));

    std::unique_ptr<AstAtom> bb1(new AstAtom("edge"));
    bb1->addArgument(std::unique_ptr<AstVariable>(new AstVariable("X")));
    bb1->addArgument(std::unique_ptr<AstVariable>(new AstVariable("Y")));

    AstClause* c2 = new AstClause();
    c2->setHead(std::move(aa1));
    c2->addToBody(std::move(bb1));

    driver.addClause(c2);
    
    InterpreterInterface* souffle = new InterpreterInterface(flags);

    souffle->init(driver);
    RamEnvironment env = souffle->execute();
    std::map<std::string, RamRelation> &res = env.getRelations();
    for( auto it = res.begin(); it != res.end(); ++it ) {
      if (it->second.getID().isOutput()){
        std::stringstream ss;
        it->second.store(ss, env.getSymbolTable(), SymbolMask(it->second.getArity()));
        std::cout << it->first << " \n" << ss.str() << "\n \n";

      }
    }
    return 0;
}


