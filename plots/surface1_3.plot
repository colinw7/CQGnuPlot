unset arrow
unset label
set grid
splot x**2+y**2, x**2-y**2
pause -1 "Hit return to continue (2)"

rep x*y
pause -1 "Hit return to continue (3)"

rep (x**3+y**3)/10
pause -1 "Hit return to continue (4)"

set ticslevel 0.0
set title "3D gnuplot demo ( ticslevel = 0.0 )"
rep
pause -1 "Hit return to continue (5)"

set ticslevel 2.0
set title "3D gnuplot demo ( ticslevel = 2.0 )"
rep
pause -1 "Hit return to continue (6)"

set ticslevel 0.5
set title "3D gnuplot demo ( ticslevel = 0.5 )"
rep
pause -1 "Hit return to continue (7)"

