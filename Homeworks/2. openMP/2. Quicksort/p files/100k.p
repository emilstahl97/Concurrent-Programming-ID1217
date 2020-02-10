# Gnuplot script for plotting data in file "10k-median.dat"
set terminal png
set output "100k.png"

set title "Execution time to sort list with different threads"

set key right center

set ylabel "Time (ms)"
set xlabel "Number of threads"

set xrange [0:16]
set yrange [0:160]

plot '100k-median.dat' with linespoints title "100.000" 
