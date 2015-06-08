reset
set size ratio -1
set title "Delaunay triangulation of Hemisphere points, some empty circles in red"
unset key

plot 'data/empty-circles.dat' with circles lw 2 lc rgb "red", \
     'data/delaunay-edges.dat' with lines lc rgb "forest-green", \
     'data/hemisphr.dat' u (100*$1):(100*$2) with points pt 7 lc rgb "black"

pause -1 "Hit return to continue"
