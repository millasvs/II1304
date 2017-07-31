set terminal png
set output "comp_splay_ll.png"

set title "Comparison splay tree and doubly linked list"
set key left

set xlabel "size of priority queue"
set ylabel "time for enqueue + dequeue in s"

set xrange [1000:50000]

# define a new line style with terminal-independent color cyan,
# linewidth 3, and associated point type 6 (a circle with a dot in it).
set style line 1 lt rgb "cyan" lw 1 pt 1
set style line 2 lt rgb "cyan" lw 1 pt 4
set style line 3 lt rgb "cyan" lw 1 pt 6

set style line 4 lt rgb "magenta" lw 1 pt 1
set style line 5 lt rgb "magenta" lw 1 pt 4
set style line 6 lt rgb "magenta" lw 1 pt 6

plot "linked_list_avg.dat" u 1:2 w linespoints ls 1 title "linked list avg", \
"linked_list_best.dat" u 1:2 w linespoints ls 2 title "linked list best", \
"exponential.dat" u 1:2 w linespoints ls 3 title "linked list worst", \
"splay.dat" u 1:2 w linespoints ls 4 title "splay avg", \
"splay_best.dat" u 1:2 w linespoints ls 5 title "splay best", \
"splay_worst.dat" u 1:2 w linespoints ls 6 title "splay worst"

