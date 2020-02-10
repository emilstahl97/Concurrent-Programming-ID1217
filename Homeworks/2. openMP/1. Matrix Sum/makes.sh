echo "1 thread" >> 100k.txt 
echo "running first job"
./test 100000 1 >> 100k.txt
sleep 1
./test 100000 1 >> 100k.txt
sleep 1
./test 100000 1 >> 100k.txt
sleep 1
./test 100000 1 >> 100k.txt
sleep 1
./test 100000 1 >> 100k.txt

echo "running second job"
echo " " >> 100k.txt
echo "2 threads" >> 100k.txt 
sleep 1
./test 100000 2 >> 100k.txt
sleep 1
./test 100000 2 >> 100k.txt
sleep 1
./test 100000 2 >> 100k.txt
sleep 1
./test 100000 2 >> 100k.txt
sleep 1
./test 100000 2 >> 100k.txt
sleep 1

echo "running third job"
echo " " >> 100k.txt
echo "4 threads" >> 100k.txt 
sleep 1
./test 100000 4 >> 100k.txt
sleep 1
./test 100000 4 >> 100k.txt
sleep 1
./test 100000 4 >> 100k.txt
sleep 1
./test 100000 4 >> 100k.txt
sleep 1
./test 100000 4 >> 100k.txt
sleep 1

echo "running fourth job"
echo " " >> 100k.txt
echo "8 threads" >> 100k.txt 
sleep 1
./test 100000 8 >> 100k.txt
sleep 1
./test 100000 8 >> 100k.txt
sleep 1
./test 100000 8 >> 100k.txt
sleep 1
./test 100000 8 >> 100k.txt
sleep 1
./test 100000 8 >> 100k.txt
sleep 1

echo "running fifth job"
echo " " >> 100k.txt
echo "16 threads" >> 100k.txt 
sleep 1
./test 100000 16 >> 100k.txt
sleep 1
./test 100000 16 >> 100k.txt
sleep 1
./test 100000 16 >> 100k.txt
sleep 1
./test 100000 16 >> 100k.txt
sleep 1
./test 100000 16 >> 100k.txt
sleep 1

exit 1