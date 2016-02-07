print ""
print "ASCII data files have a matrix variant.  Unlike matrix binary,"
print "ASCII binary may have multiple matrices per file, separted"
print "by a blank line.  The keyword \`index\` can select the desired"
print "matrix to plot."
print ""
set style data lines
set hidden3d
set ticslevel 0
set key box
set view 70,45
set multiplot
set size 0.5,0.5
set origin 0.0,0.5
unset key
set tics out
set title "'asciimat.dat' matrix index 0"
splot 'data/asciimat.dat' matrix index 0
set size 0.5,0.5
set origin 0.5,0.5
set title "'asciimat.dat' matrix index 1"
splot 'data/asciimat.dat' matrix index 1
set size 0.5,0.5
set origin 0.0,0.0
set title "'asciimat.dat' matrix index 2"
splot 'data/asciimat.dat' matrix index 2
set size 0.5,0.5
set origin 0.5,0.0
set title "'asciimat.dat' matrix index 2 using 1:(2*$2):($3*$3)"
splot 'data/asciimat.dat' matrix index 2 using 1:(2*$2):($3*$3)
unset multiplot
pause -1 "Hit return to continue"
