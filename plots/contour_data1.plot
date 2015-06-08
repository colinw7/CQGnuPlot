set contour
set cntrparam levels 10
set auto
set style data lines
set title "contour of data grid plotting"
set xrange [0:15]
set yrange [0:15]
set zrange [-1.2:1.2]
unset parametric
splot "data/glass.dat" using 1
