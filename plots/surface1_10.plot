set samples 21
set isosample 11

set xtics ("low" -3, "mid" 0, "high" 3)
set ytics -2,0.5,2
set xrange [-3:3]
set yrange [-3:3]
set log z
set title "Surfaces with z log scale"
splot x**2*y**2 + 2, x**2*y**4 + 2, x**4*y**2 + 2
pause -1 "Hit return to continue (10)"
