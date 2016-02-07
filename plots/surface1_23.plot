set title "splot with \"set pm3d\" (implemented with some terminals)"
set pm3d
set hidden3d
splot 'data/glass.dat' every 2::0::12 with lines
pause -1 "Hit return to continue (23)"
