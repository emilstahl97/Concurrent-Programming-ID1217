echo "1 thread" >> 10k.txt.txt 
echo "running first job"
./test 10000 1000 1 >> 10k.txt.txt
sleep 1
./test 10000 1000 1 >> 10k.txt.txt
sleep 1
./test 10000 1000 1 >> 10k.txt.txt
sleep 1
./test 10000 1000 1 >> 10k.txt.txt
sleep 1
./test 10000 1000 1 >> 10k.txt.txt

echo "running second job"
echo " " >> 10k.txt.txt
echo "2 threads" >> 10k.txt.txt 
sleep 1
./test 10000 1000 2 >> 10k.txt.txt
sleep 1
./test 10000 1000 2 >> 10k.txt.txt
sleep 1
./test 10000 1000 2 >> 10k.txt.txt
sleep 1
./test 10000 1000 2 >> 10k.txt.txt
sleep 1
./test 10000 1000 2 >> 10k.txt.txt
sleep 1

echo "running third job"
echo " " >> 10k.txt.txt
echo "4 threads" >> 10k.txt.txt 
sleep 1
./test 10000 1000 4 >> 10k.txt.txt
sleep 1
./test 10000 1000 4 >> 10k.txt.txt
sleep 1
./test 10000 1000 4 >> 10k.txt.txt
sleep 1
./test 10000 1000 4 >> 10k.txt.txt
sleep 1
./test 10000 1000 4 >> 10k.txt.txt
sleep 1

echo "running fourth job"
echo " " >> 10k.txt.txt
echo "8 threads" >> 10k.txt.txt 
sleep 1
./test 10000 1000 8 >> 10k.txt.txt
sleep 1
./test 10000 1000 8 >> 10k.txt.txt
sleep 1
./test 10000 1000 8 >> 10k.txt.txt
sleep 1
./test 10000 1000 8 >> 10k.txt.txt
sleep 1
./test 10000 1000 8 >> 10k.txt.txt
sleep 1

echo "running fifth job"
echo " " >> 10k.txt.txt
echo "16 threads" >> 10k.txt.txt 
sleep 1
./test 10000 1000 16 >> 10k.txt.txt
sleep 1
./test 10000 1000 16 >> 10k.txt.txt
sleep 1
./test 10000 1000 16 >> 10k.txt.txt
sleep 1
./test 10000 1000 16 >> 10k.txt.txt
sleep 1
./test 10000 1000 16 >> 10k.txt.txt
sleep 1

exit 1