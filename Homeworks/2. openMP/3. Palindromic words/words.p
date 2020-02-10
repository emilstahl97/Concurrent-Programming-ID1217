# Gnuplot script for plotting data in file "words-median.dat"
set terminal png
set output "words.png"

set title "Execution time to find palindromic words in file with 25k words"

set key right center

set ylabel "Time (ms)"
set xlabel "Number of threads"

set xrange [0:16]
set yrange [0:20]

plot 'words-median.dat' with linespoints title "#words = 25.143" 
