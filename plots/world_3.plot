unset key
set border
set yzeroaxis
set xtics
set ytics
set angles degrees
set ticslevel 0
set view 70,40,0.8,1.2
set mapping spherical
set parametric
set samples 32
set isosamples 9
set urange [-90:90]
set vrange [0:360]

# HBB 20000715: new demo:
# same plot, but with hidden3d active, plus axes through the
# poles:
set title "3D solid version with hidden line removal"
set hidden3d offset 0
set arrow from 0,0,-1.2 to 0,0,1.2 lc rgb "cyan" lw 2
set arrow from -1.2, 0, 0 to 1.2, 0, 0 nohead lc rgb "cyan" lw 1
set arrow from 0, -1.2, 0 to 0, 1.2, 0 nohead lc rgb "cyan" lw 1
splot cos(u)*cos(v),-cos(u)*sin(v),sin(u) with lines lc rgb "cyan" ,\
'data/world.dat' u 1:2:(1.001) with lines lc rgb "blue" , 'data/world.cor' with points lt 1 pt 2
pause -1 "Hit return to continue"

unset arrow