set palette gray

unset key
set pm3d map
set xrange [10:117]
set yrange [10:117]
set tics out
set colorbox
set cbrange [0:255]
set title "As with 3d color surfaces, a color box may be added to the plot"
splot 'data/blutux.rgb' binary array=(128,128) flipy format='%uchar%uchar%uchar' using ($1+$2+$3)/3 with image
pause -1 "Hit return to continue"
