package com.soufflelang.souffle;

import com.soufflelang.souffle.*;

public class Solver {
    public Solver(){
      init();
    }

    public native void init();
    public native void release();
    public native void parse(Program p);
    public native Result executeInterpreter(Data d);
    public native Result executeCompiler(Data d, String s);
   

    private long nativeHandle;
    private long returnHandle;
};
