# Gnuplot script for plotting data in file "sequential.dat"

set terminal png
set output "sequential.png"

set title "Execution time to compute particle simulation"

set key right center

set ylabel "Time (ms)"
set xlabel "Number of threads"

set xrange [0:16]
set yrange [0:3]

plot 'sequential-120.dat' with linespoints title "120", plot 'sequential-180.dat' with linespoints title "180", plot 'sequential-240.dat' with linespoints title "240",
