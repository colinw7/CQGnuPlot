set multiplot

set origin 0,0
set size 1,1
set margin 4,2,2,4

plot [-130:-65][22:50] "data/us-states.data" with polygons using 2:1 lc rgb "#8bb397" fs solid border lc rgb "black"

set origin 0,0
set size 1,1

set datafile csv
set clip

plot [-130:-65][22:50] "data/airports.csv" with delaunay using 7:6 pt 6 pointsize 0.5

unset multiplot
