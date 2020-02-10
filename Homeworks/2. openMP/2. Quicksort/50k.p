# Gnuplot script for plotting data in file "10k-median.dat"
set terminal png
set output "50k.png"

set title "Execution time to sort list with different threads"

set key right center

set ylabel "Time (ms)"
set xlabel "Number of threads"

set xrange [0:16]
set yrange [0:100]

plot '50k-median.dat' with linespoints title "50.000" 
