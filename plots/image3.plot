set key
set xrange [-10:137]
set yrange [-10:157]
set label 1 "\"I am the penguin, GOO GOO GOO JOOB.\"" at 63,140 center
set title "Palette mode 'image' used to produce psychedelic bird"
unset colorbox
plot 'data/blutux.rgb' binary array=(128,128) flipy format='%uchar%uchar%uchar' using ($1+$2+$3) with image
