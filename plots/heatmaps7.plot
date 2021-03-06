#
# Demonstrate use of 4th data column to color a 3D surface.
# Also demonstrate use of the pseudodata special file '++'.
# This plot is nice for exploring the effect of the 'l' and 'L' hotkeys.
#
set view 49, 28, 1, 1.48
set xrange [ 5 : 35 ] noreverse nowriteback
set yrange [ 5 : 35 ] noreverse nowriteback
# set zrange [ 1.0 : 3.0 ] noreverse nowriteback
set ticslevel 0
set format cb "%4.1f"
set colorbox user size .03, .6 noborder
set cbtics scale 0

set samples 25, 25
set isosamples 50, 50

set title "4D data (3D Heat Map)"\
          ."\nIndependent value color-mapped onto 3D surface"  offset 0,1
set xlabel "x" offset 3, 0, 0 
set ylabel "y" offset -5, 0, 0
set zlabel "z" offset 2, 0, 0 
set pm3d implicit at s

Z(x,y) = 100. * (sinc(x,y) + 1.5)
sinc(x,y) = sin(sqrt((x-20.)**2+(y-20.)**2))/sqrt((x-20.)**2+(y-20.)**2)
color(x,y) = 10. * (1.1 + sin((x-20.)/5.)*cos((y-20.)/10.))

set title "4D data (3D Heat Map)"\
          ."\nZ is contoured. Independent value is color-mapped"  offset 0,1

set view map
set ylabel norotate offset -1,0

set contour base
splot '++' using 1:2:(Z($1,$2)):(color($1,$2)) with pm3d title "4 data columns x/y/z/color"
