print ""
print "Images maintain orientation with respect to axis direction."
print "All plots show the same exact plot, but with various states"
print "of reversed axes.  The upper left plot has reversed x axis,"
print "the upper right plot has conventional axes, the lower left"
print "plot has both reversed x and y axes, and the lower right"
print "plot has reversed y axis."
print ""
set size 1.0, 1.0
set origin 0.0, 0.0
set multiplot
set size 0.5,0.48
unset key
set origin 0.0,0.47
set xrange [127.5:-0.5]
set x2range [127.5:-0.5]
set yrange [-0.5:127.5]
set y2range [-0.5:127.5]
set xtics out mirror
unset ytics
set y2tics out mirror
set title '"Eccentric coordinate systems"' offset 0,-0.5
set label 1 "Images reverse according to axis orientation" at -30,162 center
plot 'data/blutux.rgb' binary array=(128,128) flip=y format='%uchar' using 1:2:3 with rgbimage
unset label 1
set origin 0.5,0.47
set xrange [-0.5:127.5]
set x2range [-0.5:127.5]
unset y2tics
set ytics out mirror
set title '"Cartesian plane!"'
replot
set origin 0.0,0.04
unset xtics
set xrange [127.5:-0.5]
set x2range [127.5:-0.5]
set x2tics out mirror
unset ytics
set yrange [127.5:-0.5]
set y2range [127.5:-0.5]
set y2tics out mirror
unset title
set title ' ' offset 0,-0.5
set label 1 '"Nato, ergo sum."' at 64,140 center rotate by 180
replot
set origin 0.5,0.04
set xrange [-0.5:127.5]
set x2range [-0.5:127.5]
unset y2tics
set ytics out mirror
set title ' ' offset 0,-0.5
set label 1 '"Physics is phun."' at 64,140 center rotate by 180
replot
unset multiplot
pause -1 "Hit return to continue"
