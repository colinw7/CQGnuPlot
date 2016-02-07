print ""
print "As with ASCII data, decimation in various directions can"
print "be achieved via the \`every\` keyword.  (Note that no down-"
print "sampling filter is applied such that you risk aliasing data"
print "with the \`every\` keyword."
print ""
print "Here is a series of plots with increasing decimation."
print ""
set style data lines
set hidden3d
set ticslevel 0
set key box
set title "Non-decimated matrix data file"
set view 70,45
set xrange [-3:3]
set yrange [-3:3]
splot "data/binary2" binary
pause -1 "Hit return to continue"
set title "Decimate by two in first dimension"
splot "data/binary2" binary every 2
pause -1 "Hit return to continue"
set title "Decimate by three in second dimension"
splot "data/binary2" binary every :3
pause -1 "Hit return to continue"
set title "Decimate by four in both dimensions"
splot "data/binary2" binary every 4:4
pause -1 "Hit return to continue"
