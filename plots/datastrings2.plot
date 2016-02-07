reset
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
plot 'data/ctg-y2.dat' using ($0-0.2):2 index 0 with boxes title "precipitation 1992-2000", \
     ''           using ($0+0.2):2 index 1 with boxes title "              2000-2001", \
     ''           using ($0):2 index 2 with linespoints lw 3 title "runoff 1992-2000", \
     ''           using ($0):2:xticlabels(1) index 3 with linespoints lw 3 title "       2000-2001"
pause -1 "Press return to continue"
