import scala.collection.JavaConversions._
import scala.collection.mutable.ArrayBuffer
import com.soufflelang.souffle._

object Main {
  System.loadLibrary("souffle");

  def main(args: Array[String]): Unit = {
    // Create Solver
    val souffle = new Solver();

    // Create Program
    val p = new Program();
    p.getExample(p);

    val data = new Data();
    data.addRelationTuple("edge", ArrayBuffer("A", "B"))
    data.addRelationTuple("edge", ArrayBuffer("B", "C"))
    data.addRelationTuple("edge", ArrayBuffer("C", "D"))
    data.addRelationTuple("edge", ArrayBuffer("D", "E"))
    data.addRelationTuple("edge", ArrayBuffer("E", "F"))
    data.addRelationTuple("edge", ArrayBuffer("F", "Z"))

    println(p.print());

    // Parse 
    souffle.parse(p);

    // Execute
    val r = souffle.executeCompiler(data, "scalaEx");

    data.release();

    // Read Data
    val vals = r.getRelationRows("path").toSeq;

    println("==============PATH===================");
    for(rec <- vals) {
      for(v <- rec) {
        print(v + "\t")
      }
      println("\n")
    }

    r.release();
    souffle.release();
  }

}
