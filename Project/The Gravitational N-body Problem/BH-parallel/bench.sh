javac BarnesHut.java
echo "Running parallel with 180 bodies"

echo "1 thread" >> parallel-180.dat

echo "1 thread"
echo "Running execution 1"
java BarnesHut 180 56500 1 >> parallel-180.dat
sleep 1
echo "Running execution 2"
java BarnesHut 180 56500 1 >> parallel-180.dat
sleep 1
echo "Running execution 3"
java BarnesHut 180 56500 1 >> parallel-180.dat
sleep 1
echo "Running execution 4"
java BarnesHut 180 56500 1 >> parallel-180.dat
sleep 1
echo "Running execution 5"
java BarnesHut 180 56500 1 >> parallel-180.dat
sleep 1

echo ""
echo "2 threads"
echo "2 thread" >> parallel-180.dat
echo "" >> parallel-180.dat


echo "Running execution 1"
java BarnesHut 180 56500 2 >> parallel-180.dat
sleep 1
echo "Running execution 2"
java BarnesHut 180 56500 2 >> parallel-180.dat
sleep 1
echo "Running execution 3"
java BarnesHut 180 56500 2 >> parallel-180.dat
sleep 1
echo "Running execution 4"
java BarnesHut 180 56500 2 >> parallel-180.dat
sleep 1
echo "Running execution 5"
java BarnesHut 180 56500 2 >> parallel-180.dat
sleep 1

echo ""
echo "3 threads"
echo ""

echo "3 thread" >> parallel-180.dat

echo "Running execution 1"
java BarnesHut 180 56500 3 >> parallel-180.dat
sleep 1
echo "Running execution 2"
java BarnesHut 180 56500 3 >> parallel-180.dat
sleep 1
echo "Running execution 3"
java BarnesHut 180 56500 3 >> parallel-180.dat
sleep 1
echo "Running execution 4"
java BarnesHut 180 56500 3 >> parallel-180.dat
sleep 1
echo "Running execution 5"
java BarnesHut 180 56500 3 >> parallel-180.dat
sleep 1

echo ""
echo "4 threads"
echo ""

echo "4 thread" >> parallel-180.dat

echo "Running execution 1"
java BarnesHut 180 56500 4 >> parallel-180.dat
sleep 1
echo "Running execution 2"
java BarnesHut 180 56500 4 >> parallel-180.dat
sleep 1
echo "Running execution 3"
java BarnesHut 180 56500 4 >> parallel-180.dat
sleep 1
echo "Running execution 4"
java BarnesHut 180 56500 4 >> parallel-180.dat
sleep 1
echo "Running execution 5"
java BarnesHut 180 56500 4 >> parallel-180.dat
sleep 1

echo "done"