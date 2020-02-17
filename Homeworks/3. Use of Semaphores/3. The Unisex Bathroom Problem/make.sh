rm bathroom
gcc bathroom.c -o bathroom -lpthread
echo "Running bathroom with 10 women and 10 men that should use the bathroom 5 time each"
sleep 1
./bathroom 10 10 5

