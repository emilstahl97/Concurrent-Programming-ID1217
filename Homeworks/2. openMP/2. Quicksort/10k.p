# Gnuplot script for plotting data in file "10k-median.dat"
set terminal png
set output "10k.png"

set title "Execution time to sort list with different threads"

set key right center

set ylabel "Time (ms)"
set xlabel "Number of threads"

set xrange [0:16]
set yrange [0:20]

plot '10k-median.dat' with linespoints title "10.000" 
