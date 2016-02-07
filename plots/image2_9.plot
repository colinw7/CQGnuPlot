print ""
print "These concepts of extending lower dimensional data also apply"
print "to temporal-like signals.  For example, a uniformly sampled"
print "sinusoid, sin(1.75*pi*x), in a binary file having no data for"
print "the independent variable can be displayed along any direction"
print "for both \'plot\'..."
print ""
set size 1.0, 1.0
set origin 0.0, 0.0
set multiplot
set size 0.5,0.48
set origin 0.0,0.47
unset key
set xrange [0:2]
set yrange [-1:1]
set label 1 "Temporal data having one generated coordinate" at 2.25,1.5 center
set title 'Along the x-axis' offset 0,-0.5
plot 'data/sine.bin' binary endian=little array=201 dx=0.01 origin=(0,0) format='%f' using 1 with lines
unset label 1
set size 0.5,0.48
set origin 0.5,0.47
set xrange [-1:1]
set yrange [0:2]
set title 'Along the y-axis'
plot 'data/sine.bin' binary endian=little array=201 dx=0.01 origin=(0,0) rotate=0.5pi format='%f' with lines
set size 0.5,0.48
set origin 0.25,0.0
set xrange [-2.2:0.7]
set yrange [-2.2:0.7]
set title 'Along a 225 degree projection'
plot 'data/sine.bin' binary endian=little array=201 dx=0.01 rotate=225d origin=(0,0) format='%f' using 1 with lines
unset multiplot
pause -1 "Hit return to continue"
