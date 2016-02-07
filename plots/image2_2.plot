print ""
print "Again, a different format specification for \`using\` can be"
print "used to select different \"columns\" within the file."
print ""
set xrange [1:8]
set title "Convex     November 1-7 1989"
set key below
set label "(Weekend)" at 5,25 center
plot 'data/using.bin' binary format='%*int32%*int8%int16%int8%*int16%*int16' using ($1/100):2 title "Logged in" with impulses,\
     'data/using.bin' binary format='%*int32%*int8%int16%*int8%int16%*int16' using ($1/100):($2/100) t "Load average" with points,\
     'data/using.bin' binary format='%*int32%*int8%int16%*int8%*int16%int16' using ($1/100):($2/100) t "%CPU used" with lines
unset label
pause -1 "Hit return to continue"
