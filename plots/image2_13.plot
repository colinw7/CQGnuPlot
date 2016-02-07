print ""
print "Binary data stored in matrix format (i.e., gnuplot binary)"
print "may also be translated with similar syntax.  However, the"
print "binary keywords \`format\`, \`array\` and \`record\` do not apply"
print "because gnuplot binary has the requirements of float data"
print "and grid information as part of the file.  Here is an"
print "example of a single matrix binary file input four times,"
print "each translated to a different location with different"
print "orientation."
print ""
set style data lines
set hidden3d
set ticslevel 0
set key box
set title "Matrix binary data (gnuplot binary) translated"
set vi 70,10
set xrange [-3:15]
set yrange [-3:15]
set zrange [-0.2:1.2]
splot "data/binary3" binary center=(1.5,1.5,0), \
  "data/binary3" binary center=(10.5,1.5,0) rotate=0.5pi u 1:2:3, \
  "data/binary3" binary center=(10.5,10.5,0) rotate=1.0pi u 1:2:3, \
  "data/binary3" binary center=(1.5,10.5,0) rotate=1.5pi u 1:2:3
pause -1 "Hit return to continue"
