unset key
set border
set yzeroaxis
set xtics
set ytics
set angles degrees

#
# plot a '3D version using cylindrical coordinate system' of the world.
set title "3D version using cylindrical coordinate system"
set ticslevel 0.0
set view 70,40,0.8,1.2
set mapping cylindrical
set parametric
set samples 32
set isosamples 13
set urange [-180:180]
set vrange [-90:90]
splot cos(u),sin(u),v with lines lc rgb "cyan" ,\
'data/world.dat' with lines lc rgb "blue" lw 2, 'data/world.cor' with points lt 1 pt 2
pause -1 "Hit return to continue"
reset
