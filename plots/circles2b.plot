# by Peter Juhasz - 2010

reset

set style fill solid 1.0 border -1
set size ratio -1

# Plot pie charts on top of the map of the world.
# The pie segments are read from a data file.
# Each segment has to be specified with a start and end angle.

set title "Sources of energy production, plotted for each continent"

unset key
unset xtics
unset ytics
set border

set yrange [-70:*]
set palette model HSV func gray*0.75, 0.5, 0.99
unset colorbox
r = 0.01
types = 6
keyx = -137.
keyy = -15.
keyr = 25.

set obj 20 rect from graph 0,0 to graph 1,1 behind fc rgb "cyan" fs solid 0.2
set angle degree

plot 'data/energy_circles.dat' using 2:1:(sqrt($9)*r):6:($6+$7):5 \
     with circles lc pal fs solid 1.0 border rgb "gray"

pause -1 "Hit return to continue"
