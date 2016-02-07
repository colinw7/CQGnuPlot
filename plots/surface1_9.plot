set samples 21
set isosample 11

unset xtics
unset ytics
set xrange [-1:1]
set yrange [-1:1]
set title "Surfaces with no grid or tics"
splot x*y with lines, x**2*y**3 with dots, x**3*y*2 with points
pause -1 "Hit return to continue (9)"
