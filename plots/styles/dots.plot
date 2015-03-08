unset key
set xrange [0: 1]
set yrange [0: 1]
set title "Lattice test for random numbers"
set xlabel "rand(n) ->"
set ylabel "rand(n + 1) ->"
set format x "%3.2f"
set format y "%3.2f"
set tics
set sample 1000
set style function dots
set parametric
plot rand(0), rand(0)
