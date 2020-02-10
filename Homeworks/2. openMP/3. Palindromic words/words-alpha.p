# Gnuplot script for plotting data in file "words-alpha-median.dat"
set terminal png
set output "words-alpha.png"

set title "Execution time to find palindromic words in file with 370k words"

set key right center

set ylabel "Time (ms)"
set xlabel "Number of threads"

set xrange [0:16]
set yrange [0:400]

plot 'words-alpha-median.dat' with linespoints title "#words = 370.102" 
