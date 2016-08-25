package com.soufflelang.souffle;

public class Executor {

  public Executor(long env) { 
    nativeHandle = env;
  }

  private native void init();
  public native void release();
  public native Result executeCompiler(Data d, String name);
  public native Result executeInterpreter(Data d);

  private long nativeHandle;
}
