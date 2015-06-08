set size ratio -1
set title "Lena's key points"

unset xtics
unset ytics

unset key
unset border

set yrange [] reverse

#plot 'data/lena.rgb' binary array=(128,128) dx=4 dy=4 format='%uchar' with rgbimage
#plot 'data/lena-keypoints.bin' binary format='%double' with circles lc rgb "yellow"

plot 'data/lena.rgb' binary array=(128,128) dx=4 dy=4 format='%uchar' with rgbimage, \
     'data/lena-keypoints.bin' binary format='%double' with circles lc rgb "yellow"

pause -1 "Hit return to continue"
