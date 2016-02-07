#
# $Id: world.dem,v 1.10.2.1 2014/11/08 04:52:24 sfeam Exp $
#
#
set title "Gnuplot Correspondences\ngeographic coordinate system"
unset key
set xrange [-180:180]
set yrange [-90:90]
set yzeroaxis
set xtics geographic
set ytics geographic
set format x "%D %E"
set format y "%D %N"
#
# plot world map and correspondent locations as a +
plot 'data/world.dat' with lines lc rgb "blue" , 'data/world.cor' with points lt 1 pt 2
pause -1 "Hit return to continue"
