print ""
print "Tux says \"bye-bye\"."
print ""
set xrange [-0.5:137.5]
set yrange [-0.5:137.5]
unset border
unset xtics
unset ytics
unset ztics
unset key
unset colorbox
set tmargin 0
set bmargin 0
set rmargin 0
set lmargin 0
set view 60, 45, 1, 1
set label 1 "Tux in a reflective mood" at 140,0,40 center
splot 'data/blutux.rgb' binary array=(128,128) flip=y rot=90d origin=(0,0,0) perp = (1,0,0) format='%uchar' with rgbimage, \
      'data/blutux.rgb' binary array=(128,128) flipy rot=90d origin=(+137,+137,0) perp=(0,1,0) format='%uchar' with rgbimage
pause -1 "Hit return to continue"
