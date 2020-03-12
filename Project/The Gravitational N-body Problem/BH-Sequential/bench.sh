echo "Runnig seq BH"
javac BarnesHut.java

echo "120" >> sequential-120.dat
echo "" >> sequential-120.dat
echo "Running 120"

echo "Running execution 1"
java BarnesHut 120 56500 100 >> sequential-120.dat
sleep 1
echo "Running execution 2"
java BarnesHut 120 56500 100 >> sequential-120.dat
sleep 1
echo "Running execution 3"
java BarnesHut 120 56500 100 >> sequential-120.dat
sleep 1
echo "Running execution 4"
java BarnesHut 120 56500 100 >> sequential-120.dat
sleep 1
echo "Running execution 5"
java BarnesHut 120 56500 100 >> sequential-120.dat
sleep 1

echo "180" >> sequential-180.dat
echo "" >> sequential-180.dat
echo "180"

echo "Running execution 1"
java BarnesHut 180 56500 100 >> sequential-180.dat
sleep 1
echo "Running execution 2"
java BarnesHut 180 56500 100 >> sequential-180.dat
sleep 1
echo "Running execution 3"
java BarnesHut 180 56500 100 >> sequential-180.dat
sleep 1
echo "Running execution 4"
java BarnesHut 180 56500 100 >> sequential-180.dat
sleep 1
echo "Running execution 5"
java BarnesHut 180 56500 100 >> sequential-180.dat
sleep 1

echo "240"

echo "240" >> sequential-240.dat
echo "" >> sequential-240.dat

echo "Running execution 1"
java BarnesHut 240 56500 100 >> sequential-240.dat
sleep 1
echo "Running execution 2"
java BarnesHut 240 56500 100 >> sequential-240.dat
sleep 1
echo "Running execution 3"
java BarnesHut 240 56500 100 >> sequential-240.dat
sleep 1
echo "Running execution 4"
java BarnesHut 240 56500 100 >> sequential-240.dat
sleep 1
echo "Running execution 5"
java BarnesHut 240 56500 100 >> sequential-240.dat
sleep 1

echo "done"