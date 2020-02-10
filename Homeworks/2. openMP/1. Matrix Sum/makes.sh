echo "1 thread" >> 1k.txt 

./maxminB 1000 1000 1 >> 1k.txt
sleep 1
./maxminB 1000 1000 1 >> 1k.txt
sleep 1
./maxminB 1000 1000 1 >> 1k.txt
sleep 1
./maxminB 1000 1000 1 >> 1k.txt
sleep 1
./maxminB 1000 1000 1 >> 1k.txt

echo " " >> 1k.txt
echo "2 threads" >> 1k.txt 
sleep 1
./maxminB 1000 1000 2 >> 1k.txt
sleep 1
./maxminB 1000 1000 2 >> 1k.txt
sleep 1
./maxminB 1000 1000 2 >> 1k.txt
sleep 1
./maxminB 1000 1000 2 >> 1k.txt
sleep 1
./maxminB 1000 1000 2 >> 1k.txt
sleep 1
echo " " >> 1k.txt
echo "4 threads" >> 1k.txt 
sleep 1
./maxminB 1000 1000 4 >> 1k.txt
sleep 1
./maxminB 1000 1000 4 >> 1k.txt
sleep 1
./maxminB 1000 1000 4 >> 1k.txt
sleep 1
./maxminB 1000 1000 4 >> 1k.txt
sleep 1
./maxminB 1000 1000 4 >> 1k.txt
sleep 1
echo " " >> 1k.txt
echo "8 threads" >> 1k.txt 
sleep 1
./maxminB 1000 1000 8 >> 1k.txt
sleep 1
./maxminB 1000 1000 8 >> 1k.txt
sleep 1
./maxminB 1000 1000 8 >> 1k.txt
sleep 1
./maxminB 1000 1000 8 >> 1k.txt
sleep 1
./maxminB 1000 1000 8 >> 1k.txt
sleep 1
echo " " >> 1k.txt
echo "16 threads" >> 1k.txt 
sleep 1
./maxminB 1000 1000 16 >> 1k.txt
sleep 1
./maxminB 1000 1000 16 >> 1k.txt
sleep 1
./maxminB 1000 1000 16 >> 1k.txt
sleep 1
./maxminB 1000 1000 16 >> 1k.txt
sleep 1
./maxminB 1000 1000 16 >> 1k.txt
sleep 1

exit 1