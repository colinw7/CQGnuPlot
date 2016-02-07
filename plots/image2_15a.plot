print ""
print "Decimation works on general binary data files as well.  Here is the"
print "image file with increasing decimation."
print ""
set title "Decimation works on general binary data files as well.\nLet Tux have his fun..."
set xrange [28:98]
set yrange [55:125]
set label 1 "\"Can I do that?\"" at 64,116 center
plot 'data/blutux.rgb' binary array=(128,128) format='%uchar' every 1:1:43:15:83:65 with rgbimage
pause -1 "Hit return to continue"
set title "... Sure, go ahead."
set xrange [-10:130]
set yrange [-8:157]
set label 1 "\"Picasso's \'Penguin on Beach\'\"" at 64,139 center
plot 'data/blutux.rgb' binary array=(128,128) format='%uchar' every 8:8 with rgbimage
pause -1 "Hit return to continue"
