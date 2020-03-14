set term png

set output "BH-sequential.png"

set boxwidth 0.5

set style fill solid

set title "Execution time to compute n-body simulation sequentially with O(n logn) Barnes-Hut"

set xlabel "Number of bodies"
set ylabel "Time(s)"
set yrange [0:15]

plot "BH-sequential.dat" using 1:3:xtic(2) with boxes title ""