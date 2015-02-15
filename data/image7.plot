set size 1.0, 1.0
set origin 0.0, 0.0
set multiplot
set size 0.5,0.48
set origin 0.0,0.47
unset key
set xrange [-0.5:127.5]
set yrange [-0.5:127.5]
set tics out
set label 1 "Selection of the input channels via \`using\`" at 140,160 center
set title '"I do impersonations..."' offset 0,-0.5
plot 'blutux.rgb' binary array=(128,128) flip=y format='%uchar' using 1:2:3 with rgbimage
unset label 1
set size 0.5,0.48
set origin 0.5,0.47
set title '"A cardinal."'
plot 'blutux.rgb' binary array=(128,128) flip=y format='%uchar%*uchar%*uchar' using 1:(0):(0) with rgbimage
set size 0.5,0.48
set origin 0.0,0.0
set title '"A parrot."'
plot 'blutux.rgb' binary array=(128,128) flipy format='%*uchar%uchar%*uchar' using (0):1:(0) with rgbimage
set size 0.5,0.48
set origin 0.5,0.0
set title '"A bluebird."'
plot 'blutux.rgb' binary array=(128,128) flipy format='%*uchar%*uchar%uchar' using (0):(0):1 with rgbimage
unset multiplot
