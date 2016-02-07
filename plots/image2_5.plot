print ""
print "This close up of a 2x2 image illustrates how pixels surround the"
print "sampling grid points.  This behavior is slightly different than"
print "that for pm3d where the four grid points would be used to create"
print "a single polygon element using an average, or similar mathematical"
print "combination, of the four values at those points."
print ""
set title "Close up of pixels having grid points (0,0), (0,2), (2,0) and (2,2)"
set xrange [-1.5:3.5]
set yrange [-1.5:3.5]
set cbrange [0:1]
unset key
plot '-' with rgbimage, '-' with points pt 7 ps 2 lt -1
0 0 0.0 0.0 0.5
0 2 0.0 0.5 0.0
2 0 0.0 0.5 0.0
2 2 0.0 0.0 0.5
e
0 0
0 2
2 0
2 2
e
pause -1 "Hit return to continue"
