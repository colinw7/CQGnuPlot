print ""
print "The \'flip\', \'rotate\' and \'perpendicular\' qualifiers"
print "should provide adequate freedom to orient data as desired."
print "However, there is an additional key words \'scan\' which may"
print "offer a more direct and intuitive manner of orienting data"
print "depending upon the user's application and perspective."
print ""
print "\'scan\' is a 2 or 3 letter string representing how Gnuplot"
print "should derive (x,y), (x,y,z), (t,r) or (t,r,z) from the"
print "the datafile's scan order.  The first letter pertains to the"
print "fastest rate or point-by-point increment.  The second letter"
print "pertains to the medium rate or line-by-line increment.  If"
print "there is a third letter, it pertains to the slowest rate or"
print "plane-by-plane increment.  The default or inherent scan order"
print "is \'scan=xyz\'."
print ""
print "The pseudonym \'transpose\' is equivalent to \'scan=yx\' when"
print "generating 2D coordinates and \'scan=yxz\' when generating"
print "3D coordinates."
print ""
print "There is a subtle difference between the behavior of \'scan\'"
print "when dimension info is taken from the file itself as opposed"
print "to entered at the command line.  When information is gathered"
print "from the file, internal scanning is unaltered so that issuing"
print "the \'scan\' command may cause the number of samples along"
print "the various dimensions to change.  However, when the qualifier"
print "\'array\' is entered at the command line, the array dimensions"
print "adjust so that \'array=XxYxZ\' is always the number of samples"
print "along the Cartesian x, y and z directions, respectively."
print ""
set size 1.0, 1.0
set origin 0.0, 0.0
set multiplot
set size 0.5,0.5
set origin 0.0,0.5
unset key
set xrange [-5:635]
set yrange [-5:635]
set tics out
set title 'Details read from file' offset 0,-0.5
plot 'data/demo.edf' binary filetype=auto with image
set size 0.5,0.5
set origin 0.5,0.5
set title 'Transpose of file-read axes parameters'
plot 'data/demo.edf' binary filetype=auto transpose with image
set size 0.5,0.5
set origin 0.0,0.0
set title 'Details specified at command line'
set label 1 "array=(32,64) dx=10 dy=5 scan=xy" at 315,500 center
plot 'data/demo.edf' binary array=(32,64) dx=10 dy=5 scan=xy skip=1024 format="%*float%float" using 1 with image
set size 0.5,0.5
set origin 0.5,0.0
set title 'Transpose of command line axes parameters'
set label 1 "array=(64,32) dx=5 dy=10 scan=yx" at 315,500 center
plot 'data/demo.edf' binary array=(64,32) dx=5 dy=10 scan=yx skip=1024 format="%*float%float" using 1 with image
unset label 1
unset multiplot
pause -1 "Hit return to continue"
