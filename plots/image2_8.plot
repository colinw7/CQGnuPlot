print ""
print "To have full degrees of freedom in orienting the image, an additional"
print "key word, \'perpendicular\', can translate the x/y-plane of the 2d"
print "data so that it lies orthogonal to a vector given as a three element"
print "tuple.  The default vector is, of course, (0,0,1).  The vector need"
print "not be of unit length, as this example illustrates.  Viewing this"
print "plot with the mouse active can help visualize the image's orientation"
print "by panning the axes."
print ""
set title "The key word \'perpendicular\' applies only to \'splot\'"
set cbrange [0:255]
set style line 1 lt -1 lw 1
set style line 2 lt -1 lw 1
set style arrow 1 nohead ls 1
set style arrow 2 head filled size screen 0.025,30,45 ls 2
set arrow from -10,0,0 to 10,0,0 as 1
set arrow from 0,-10,0 to 0,10,0 as 1
set arrow from 0,0,-10 to 0,0,10 as 1
set arrow from 0,0,0 to 30,30,30 as 2
splot 'data/blutux.rgb' binary array=(128,128) flipy rot=1.0pi center = (63.5,63.5,50) perp=(1,1,1) format='%uchar%uchar%uchar' using ($1+$2+$3)/3 with image
pause -1 "Hit return to continue"
