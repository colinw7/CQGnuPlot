print ""
print "Lower dimensional data may be extended to higher dimensional plots"
print "via a number of simple, logical rules.  The first step Gnuplot does"
print "is sets the components for higher than the natural dimension of the"
print "input data to zero.  For example, a two dimensional image can be"
print "displayed in the three dimensional plot as shown.  Without"
print "translation, the image lies in the x/y-plane."
print ""
set title "Simple extension of a two dimensional image into three dimensions"
set xrange [-10:137]
set yrange [-10:157]
set cbrange [0:255]
splot 'data/blutux.rgb' binary array=(128,128) flip=y format='%uchar%uchar%uchar' using ($1+$2+$3)/3 with image
pause -1 "Hit return to continue"
