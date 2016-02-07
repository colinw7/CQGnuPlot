print ""
print "Not only can the 2d binary data mode be used for image data."
print "Here is an example that repeats the \`using.dem\` demo with the"
print "same data, but stored in binary format of differing sizes.  It"
print "uses different format specifiers within the \'format\' string."
print "There are machine dependent and machine independent specifiers,"
print "display by the command \'show datafile binary datasizes\':"
print ""
show datafile binary datasizes
print ""
set title "Convex     November 1-7 1989    Circadian"
set key left box
set xrange[-1:24]
plot 'data/using.bin' binary format='%*int32%int8%*int16%int8%*int16%*int16' using 1:2 title "Logged in" with impulses,\
     'data/using.bin' binary format='%*int32%int8%*int16%int8%*int16%*int16' using 1:2 title "Logged in" with points
pause -1 "Hit return to continue"
