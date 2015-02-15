set title "Translations of position variables via 'using'"
set xrange [-10:275]
set yrange [-10:300]
unset key
set label "\"Time for a dip...\"" at 132,285 center
plot 'blutux.rgb' binary array=(128,128) flipy center=( 64,201 ) format='%uchar' with rgbimage, \
     'blutux.rgb' binary array=(128,128) flipy rotation=-90d center=(201,201) format='%uchar' with rgbimage, \
     'blutux.rgb' binary array=(128,128) flip=y rotate=3.1415 center=(201,64) format='%uchar' with rgbimage, \
     'blutux.rgb' binary array=(128,128) flip=y rot=0.5pi center=(64,64) format='%uchar' using 1:2:3 with rgbimage
