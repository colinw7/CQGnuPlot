print ""
print "The key words \'rotate\' and \'center\' still apply in \'splot\' with"
print "rules similar to their use in \'plot\'.  However, the center must be"
print "specified as a three element tuple."
print ""
unset colorbox
set title "Orientation operations from \'plot\' also apply to to \'splot\'"
splot 'data/blutux.rgb' binary array=(128,128) flipy rotate=90d center = (63.5,63.5,50) format='%uchar%uchar%uchar' using ($1+$2+$3) with image
pause -1 "Hit return to continue"
