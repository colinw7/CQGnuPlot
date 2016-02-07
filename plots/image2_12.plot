print ""
print "Generating uniformly spaced coordinates is valid for polar"
print "plots as well.  This is useful for data acquired by machines"
print "sampling in a circular fashion.  Here the sinusoidal data"
print "of the previous 2D plot put on a polar plot.  Note the"
print "pseudonyms \'dt\' meaning sample period along the angular,"
print "or theta, direction.  In Gnuplot, cylindrical coordinate"
print "notation is (t,r,z).  [Different from common math convention"
print "(r,t,z).]"
print ""
set size ratio 1
set title "Uniform sampling in the polar coordinate system"
set polar
set grid polar
set xrange [-1.3:1.3]
set yrange [-1.3:1.3]
# The sinusoid has period T=8/7.  Also, dx=0.01.  So solving 8/7 dt = 2/3 pi dx, we get dt = 0.018326.
plot 'data/sine.bin' binary endian=little array=201 dt=0.018326 origin=(0,0) format='%f' using 1 with lines
pause -1 "Hit return to continue"
