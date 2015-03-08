unset key
set xrange [-10:137]
set yrange [-10:157]
set tics out
set label 1 "\"This picture was taken by my friend Ansel Adams.\"" at 63,140 center
set title "The palette can be changed from color to gray scale"
set palette gray
unset colorbox
plot 'blutux.rgb' binary array=(128,128) flipy format='%uchar%uchar%uchar' using ($1+$2+$3)/3 with image
