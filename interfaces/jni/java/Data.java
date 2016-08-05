package com.soufflelang.souffle;
import com.soufflelang.souffle.*;

import java.util.List;

public class Data {
  public Data() {
    init();
  }

  private native void init();
  public native void release();

  public native void addRelationTuple(String name, List<String> data);

  private long nativeHandle;

}
