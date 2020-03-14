set terminal png
set output "BH-parallel.png"

set title "Execution time to compute n-body simulation in parallel with O(n log n) Barnes-Hut"

set key right center

set xlabel "Number of threads"
set ylabel "Time (ms)"

set xrange [1:4]
set yrange [0:6]

set key invert

plot 'parallel-120.dat' with linespoints title "120", 'parallel-180.dat' with linespoints title "180", 'parallel-240.dat' with linespoints title "240"
