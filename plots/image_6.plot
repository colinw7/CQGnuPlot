set palette gray

set key
set xrange [-10:138]
set yrange [-10:138]
set tics out
set title "Polygons used to draw pixels for rotated images\nNotice the slower refresh rate than for the next plot"
unset colorbox
plot 'data/blutux.rgb' binary array=(128,128) dx=0.70711 dy=0.70711 flipy rotation=45d center=(63.5,63.5) format='%uchar' using ($1+$2+$3)/3 with image
pause -1 "Hit return to continue"
reset
