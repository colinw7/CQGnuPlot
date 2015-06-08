# by Ralf Juengling - 2008

reset
set title "Trace of unconstrained optimization with trust-region method"
unset key
set size ratio -1
set xrange [-2.5:1.5]
set yrange [-1:2.5]
plot 'data/optimize.dat' with circles lc rgb "blue" fs transparent solid 0.15 noborder,\
     'data/optimize.dat' u 1:2 with linespoints lw 2 lc rgb "black"

pause -1 "Hit return to continue"
