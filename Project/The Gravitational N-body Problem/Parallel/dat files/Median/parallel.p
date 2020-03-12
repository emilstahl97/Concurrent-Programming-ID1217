set terminal png
set output "parallel.png"

set title "Execution time to compute particle simulation in parallel with n^2"

set key right center

set xlabel "Number of threads"
set ylabel "Time (ms)"

set xrange [1:4]
set yrange [0:100]

set key invert

plot 'parallel-120.dat' with linespoints title "120", 'parallel-180.dat' with linespoints title "180", 'parallel-240.dat' with linespoints title "240"
