unset key
set xrange [-138:10]
set yrange [-10:138]
set tics out
set title "Terminal image routine used to draw plot rotated about origin\nNotice the faster refresh rate than for the previous plot"
unset colorbox
plot 'blutux.rgb' binary array=(128,128) dx=1 flip=y rotation=0.5pi origin=(0,0) format='%uchar' using ($1+$2+$3)/3 with image
