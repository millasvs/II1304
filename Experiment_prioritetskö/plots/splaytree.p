set terminal png
set output "splay_time_avg_best_only.png"

set title "Evaluation of priority queue - splay tree"
set key left

set xlabel "size of priority queue"
set ylabel "time for enqueue + dequeue in s"

set xrange [1000:50000]

plot "splay_2.dat" u 1:2 w linespoints title "avg",\
"splay_best.dat" u 1:2 w linespoints title "best"


