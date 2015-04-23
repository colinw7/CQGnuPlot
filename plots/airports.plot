set multiplot

set origin 0,0
set size 1,1

plot [-130:-65][22:50] "data/us-states.data" with polygons using 2:1

set origin 0,0
set size 1,1

set datafile csv
set clip

plot [-130:-65][22:50] "data/airports.csv" with delaunay using 7:6

unset multiplot
