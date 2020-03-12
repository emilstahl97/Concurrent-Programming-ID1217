echo "Runnig seq BH"
javac BarnesHut.java

echo "120" >> sequential-120.dat
echo "" >> sequential-120.dat

java BarnesHut 120 56500 >> sequential-120.dat
sleep 1
java BarnesHut 120 56500 >> sequential-120.dat
sleep 1
java BarnesHut 120 56500 >> sequential-120.dat
sleep 1
java BarnesHut 120 56500 >> sequential-120.dat
sleep 1
java BarnesHut 120 56500 >> sequential-120.dat
sleep 1

echo "180" >> sequential-180.dat
echo "" >> sequential-180.dat

java BarnesHut 180 56500 >> sequential-180.dat
sleep 1
java BarnesHut 180 56500 >> sequential-180.dat
sleep 1
java BarnesHut 180 56500 >> sequential-180.dat
sleep 1
java BarnesHut 180 56500 >> sequential-180.dat
sleep 1
java BarnesHut 180 56500 >> sequential-180.dat
sleep 1

echo "240" >> sequential-240.dat
echo "" >> sequential-240.dat

java BarnesHut 240 56500 >> sequential-240.dat
sleep 1
java BarnesHut 240 56500 >> sequential-240.dat
sleep 1
java BarnesHut 240 56500 >> sequential-240.dat
sleep 1
java BarnesHut 240 56500 >> sequential-240.dat
sleep 1
java BarnesHut 240 56500 >> sequential-240.dat
sleep 1

echo "done"