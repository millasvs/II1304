set terminal png
set output "dist.png"

set title "Evaluation of priority queue - doubly linked list"
set key left

set xlabel "size of priority queue"
set ylabel "time for enqueue + dequeue in s"

set xrange [1000:10000]


plot "bimodial.dat" u 1:2 w linespoints title "bimodial", \
"triangular.dat" u 1:2 w linespoints title "triangular", \
"uniform.dat" u 1:2 w linespoints title "uniform", \
"biased.dat" u 1:2 w linespoints title "biased", \
"exponential.dat" u 1:2 w linespoints title "exponential"
