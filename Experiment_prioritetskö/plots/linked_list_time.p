set terminal png
set output "comparison.png"

set title "Evaluation of priority queue - doubly linked list"
set key left

set xlabel "size of priority queue"
set ylabel "time for enqueue + dequeue in s"

set xrange [1000:50000]

plot "linked_list_avg.dat" u 1:2 w linespoints title "avg", \
"linked_list_best.dat" u 1:2 w linespoints title "best", \
"linked_list_worst.dat" u 1:2 w linespoints title "worst"
