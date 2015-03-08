set title "Gnuplot Correspondences"
unset key
unset border
unset yzeroaxis
unset xtics
unset ytics
#
# plot world map and correspondent locations as a +
plot 'world.dat' with lines lt 3 , 'world.cor' with points lt 1 pt 2
set title ""
set key on
set border
set yzeroaxis
set xtics
set ytics
