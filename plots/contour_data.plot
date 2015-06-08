set contour
set cntrparam levels 10
set auto
set zrange [-1.0:1.0]
set style data lines
set title "contour of data grid plotting"
set parametric
splot "data/glass.dat"
