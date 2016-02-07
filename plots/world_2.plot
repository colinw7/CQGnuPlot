#
# plot a '3D version using spherical coordinate system' of the world.
reset
unset key
set border
set yzeroaxis
set xtics
set ytics
set angles degrees
set title "3D version using spherical coordinate system"
set ticslevel 0
set view 70,40,0.8,1.2
set mapping spherical
set parametric
set samples 32
set isosamples 9
set urange [-90:90]
set vrange [0:360]
splot cos(u)*cos(v),cos(u)*sin(v),sin(u) with lines lc rgb "cyan" ,\
'data/world.dat' with lines lc rgb "blue" , 'data/world.cor' with points lt 1 pt 2
pause -1 "Hit return to continue"
