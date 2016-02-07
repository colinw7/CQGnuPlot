set title "Larry Ewing's GIMP penguin on vacation basking in\nthe balmy waters off the coast of Murmansk"
set xrange [-10:137]
set yrange [-10:157]
set label "\"I flew here... by plane.  Why?  For the halibut.\"" at 64,135 center
plot 'data/blutux.rgb' binary array=(128,128) flipy dx=0.5 dy=0.5 format='%uchar' with rgbimage
