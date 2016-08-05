package com.soufflelang.souffle;

import java.util.Set;
import java.util.List;
import java.util.Arrays;
import java.util.ArrayList;

public class Result {

  public Result(long env) { 
    nativeHandle = env;
  }

  private native void init();

  public native void print();
  public native void release();
  public native List<List<String>> getRelationRows(String rname);

  private long nativeHandle;
}
