unset log z
set xtics autofreq
set ytics autofreq
set xrange [-1:1]
set yrange [-1:1]
set samples 51
set isosample 21
set dummy u,v
set title "3D gnuplot demo"

splot [x=-3:3] [y=-3:3] sin(x) * cos(y)
pause -1 "Hit return to continue (12)
