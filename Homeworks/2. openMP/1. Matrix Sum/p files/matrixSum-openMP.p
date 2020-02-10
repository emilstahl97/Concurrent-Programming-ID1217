# Gnuplot script for plotting data in file "1k-median.dat"
set terminal png
set output "multi.png"

set title "Execution time to compute matrix sum with different threads"

set key right center

set ylabel "Time (ms)"
set xlabel "Number of threads"

set xrange [0:16]
set yrange [0:10000]

plot '100k-median.dat' with lines title "100.000", '5k-median.dat' with linespoints title "5.000", '10k-median.dat' with linespoints title "10.000", '1k-median.dat' with linespoints title "1.000" 
