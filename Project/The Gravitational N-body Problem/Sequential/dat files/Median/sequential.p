set term png

 set output "sequential.png"

 set boxwidth 0.5

 set style fill solid

set title "Execution time to compute n-body simulation sequentially with O(n^2)

set xlabel "Number of bodies"
set ylabel "Time(s)"
 set yrange [0:100]

 plot "sequential.dat" using 1:3:xtic(2) with boxes title ""