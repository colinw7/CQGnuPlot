set cbrange [0:400]
plot 'data/blutux.rgb' binary array=(128,128) flipy format='%uchar' using 1:2:3 with rgbimage
