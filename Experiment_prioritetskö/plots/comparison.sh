#!/bin/bash
#
# Runs the priority queue implementations with a certain
# file with seeds and with a given sequence of queue sizes
#

function print_usage {
    echo "Usage: ${0} <executable> [<first> <increment> <last>)]"
    exit 1
}

if [ $# -gt 0 ]; then
    executable="$1"
    if [ $# -eq 1 ]; then
        first=1000
        increment=1000
        last=10000
    elif [ $# -eq 4 ]; then
        first=$2
        increment=$3
        last=$4
    else
        print_usage
    fi
else
    print_usage
fi

sequence=`seq --separator=' ' $first $increment $last`

echo "# Benchmarks for $executable with n = [$sequence]"
echo "# n     time (s)"
var=1
for n in $sequence; do
	seed=$(sed -n "$var"p "/home/millasvs/Documents/II1304/Experiment_prioritetskö/seeds.txt")
    printf "%q\t" $n

    for j in `seq 1 5`; do
        sim_time[$j]=$($executable $n $seed)
    done

    median=$(echo ${sim_time[1]} ${sim_time[2]} ${sim_time[3]} ${sim_time[4]} \
        ${sim_time[5]} | tr " " "\n" | sort | head -n 3 | tail -n 1)

    printf "%q\n" $median
    let "var++"

done
