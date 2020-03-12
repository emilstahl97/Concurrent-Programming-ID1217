javac ParallelNbody.java
echo "Running execution 1"
java ParallelNbody 120 56500 2 >> parallel-120.dat
sleep 1
echo "Running execution 2"
java ParallelNbody 120 56500 2 >> parallel-120.dat
sleep 1
echo "Running execution 3"
java ParallelNbody 120 56500 2 >> parallel-120.dat
sleep 1
echo "Running execution 4"
java ParallelNbody 120 56500 2 >> parallel-120.dat
sleep 1
echo "Running execution 5"
java ParallelNbody 120 56500 2 >> parallel-120.dat
sleep 1
