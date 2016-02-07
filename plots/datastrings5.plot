set title "Read tic labels from a datafile column\nAn approximation of Hans Olav Eggestad's categoric plot patch\nusing 'using ($0):2:xticlabels(1)' and 'set style fill solid border -1'"
set boxwidth 0.3
set style fill solid 1.000000 border -1
set bmargin 3
set pointsize 2
set yrange [0:180]
set xrange [-0.5:11.5]
set ylabel "mm"
set tics scale 0.0
set grid y
set xtics
#
set key autotitle columnhead
#
set title "Same plot using x2ticlabels also"
set x2tics
set title "Plot from table format (titles taken from column headers)"
unset x2tics
#
set title "Plot actual y-value as a label"
plot 'data/table.dat' using 0:2:xtic(1) title columnhead with boxes, \
     ''          using 0:2:2 with labels center offset 0,1 notitle
pause -1 "Press return to continue"
