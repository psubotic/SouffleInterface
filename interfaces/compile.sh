SOUFFLE_DIR=../souffle
INCLUDE_PATH="-I./jni/include/ -I./jni/include/generated/ -I./include/ -I/usr/lib/jvm/java-8-oracle/include/linux/ -I/usr/lib/jvm/java-8-oracle/include/ -I$SOUFFLE_DIR/src/ -I$SOUFFLE_DIR/ -I$SOUFFLE_DIR/include/"

javac -d . ./jni/java/Arg.java 
javac -d . ./jni/java/Const.java 
javac -d . ./jni/java/SConst.java 
javac -d . ./jni/java/Var.java 
javac -d . ./jni/java/BinOp.java
javac -d . ./jni/java/Literal.java 
javac -d . ./jni/java/Atom.java 
javac -d . ./jni/java/Negation.java 
javac -d . ./jni/java/Constraint.java 
javac -d . ./jni/java/Type.java
javac -d . ./jni/java/PrimType.java 
javac -d . ./jni/java/Result.java 
javac -d . ./jni/java/Clause.java 
javac -d . ./jni/java/Relation.java 
javac -d . ./jni/java/Program.java 
javac -d . ./jni/java/Data.java 
javac -d . ./jni/java/Solver.java 

javah -jni -d ./jni/include/generated/ com.soufflelang.souffle.Result
javah -jni -d ./jni/include/generated/ com.soufflelang.souffle.Data
javah -jni -d ./jni/include/generated/ com.soufflelang.souffle.Arg
javah -jni -d ./jni/include/generated/ com.soufflelang.souffle.SConst
javah -jni -d ./jni/include/generated/ com.soufflelang.souffle.Const
javah -jni -d ./jni/include/generated/ com.soufflelang.souffle.Var
javah -jni -d ./jni/include/generated/ com.soufflelang.souffle.BinOp
javah -jni -d ./jni/include/generated/ com.soufflelang.souffle.Atom 
javah -jni -d ./jni/include/generated/ com.soufflelang.souffle.Literal
javah -jni -d ./jni/include/generated/ com.soufflelang.souffle.Negation
javah -jni -d ./jni/include/generated/ com.soufflelang.souffle.Constraint
javah -jni -d ./jni/include/generated/ com.soufflelang.souffle.Clause 
javah -jni -d ./jni/include/generated/ com.soufflelang.souffle.Program
javah -jni -d ./jni/include/generated/ com.soufflelang.souffle.Relation 
javah -jni -d ./jni/include/generated/ com.soufflelang.souffle.PrimType
javah -jni -d ./jni/include/generated/ com.soufflelang.souffle.Solver

g++ -fPIC -g -Wall -std=c++11 $INCLUDE_PATH -c ./jni/src/Negation.cpp -o Negation.o
g++ -fPIC -g -Wall -std=c++11 $INCLUDE_PATH -c ./jni/src/Constraint.cpp -o Constraint.o
g++ -fPIC -g -Wall -std=c++11 $INCLUDE_PATH -c ./jni/src/Data.cpp -o Data.o
g++ -fPIC -g -Wall -std=c++11 $INCLUDE_PATH -c ./jni/src/Arg.cpp -o Arg.o
g++ -fPIC -g -Wall -std=c++11 $INCLUDE_PATH -c ./jni/src/Result.cpp -o Result.o
g++ -fPIC -g -Wall -std=c++11 $INCLUDE_PATH -c ./jni/src/Var.cpp -o Var.o
g++ -fPIC -g -Wall -std=c++11 $INCLUDE_PATH -c ./jni/src/BinOp.cpp -o BinOp.o
g++ -fPIC -g -Wall -std=c++11 $INCLUDE_PATH -c ./jni/src/Const.cpp -o Const.o
g++ -fPIC -g -Wall -std=c++11 $INCLUDE_PATH -c ./jni/src/SConst.cpp -o SConst.o
g++ -fPIC -g -Wall -std=c++11 $INCLUDE_PATH -c ./jni/src/Atom.cpp -o Atom.o
g++ -fPIC -g -Wall -std=c++11 $INCLUDE_PATH -c ./jni/src/Clause.cpp -o Clause.o
g++ -fPIC -g -Wall -std=c++11 $INCLUDE_PATH -c ./jni/src/Program.cpp -o Program.o
g++ -fPIC -g -Wall -std=c++11 $INCLUDE_PATH -c ./jni/src/Relation.cpp -o Relation.o
g++ -fPIC -g -Wall -std=c++11 $INCLUDE_PATH -c ./jni/src/PrimType.cpp -o PrimType.o
g++ -fPIC -g -Wall -std=c++11 $INCLUDE_PATH -c ./jni/src/Solver.cpp -o Solver.o

g++ -fPIC -g -Wall -std=c++11 $INCLUDE_PATH -c ./src/AstBuilder.cpp -o AstBuilder.o
g++ -fPIC -g -Wall -std=c++11 $INCLUDE_PATH -c ./src/Interfaces.cpp -o Interfaces.o

g++ -shared -o ./lib/libsouffle.so Program.o Atom.o Clause.o Relation.o Const.o SConst.o Var.o PrimType.o Solver.o Constraint.o Negation.o Interfaces.o AstBuilder.o Data.o Result.o BinOp.o ./lib/libsouffle.a

jar cf com.soufflelang.souffle.jar -C ./com/soufflelang/souffle/*.class
jar uf com.soufflelang.souffle.jar ./com/soufflelang/souffle/Arg.class
jar uf com.soufflelang.souffle.jar ./com/soufflelang/souffle/Atom.class
jar uf com.soufflelang.souffle.jar ./com/soufflelang/souffle/Result.class
jar uf com.soufflelang.souffle.jar ./com/soufflelang/souffle/Data.class
