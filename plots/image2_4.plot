print ""
print "For binary data, the byte endian format of the file and of the"
print "compiler often require attention.  Therefore, the key word"
print "\'endian\' is provided for setting or interchanging byte"
print "order.  The allowable types are \'little\', \'big\', and"
print "depending upon how your version of Gnuplot was compiled,"
print "\'middle\' (or \'pdp\') for those still living in the medieval"
print "age of computers.  These types refer to the file's endian."
print "Gnuplot arranges bytes according to this endian and what it"
print "determines to be the compiler's endian."
print ""
print "There are also the special types \'default\' and \'swap\' (or"
print "\'swab\') for those who don't know the file type but realize"
print "their data looks incorrect and want to change the byte read"
print "order."
print ""
print "Here is an example showing the \`scatter.dem\` data plotted"
print "with correct and incorrect byte order.  The file is known"
print "to be little endian, so the upper left plot is correct"
print "appearance and the upper right plot is incorrect appearance."
print "The lower two plots are default and swapped bytes.  If the"
print "plots within the columns match, your compiler uses little"
print "endian.  If diagonal plots match then your compiler uses"
print "big endian.  If neither of the bottom plots matches the"
print "upper plots, Tux says you're living in the past."
print ""
set size 1.0, 1.0
set origin 0.0, 0.0
set multiplot
set format "%.2g"
unset ztics
set size 0.5,0.48
set origin 0.0,0.47
unset key
set label 1 "If plots in columns match, your compiler is little endian" at 100,1200,3.0 center
set title 'Little endian' offset 0,-0.5
splot 'data/scatter2.bin' binary record=30:30:29:26 endian=little using 1:2:3
unset label 1
set size 0.5,0.48
set origin 0.5,0.47
set title 'Big endian'
splot 'data/scatter2.bin' binary record=30:30:29:26 endian=big using 1:2:3
set size 0.5,0.48
set origin 0.0,0.0
set title 'Default'
splot 'data/scatter2.bin' binary record=30:30:29:26 endian=default using 1:2:3
set size 0.5,0.48
set origin 0.5,0.0
set title 'Swapped'
splot 'data/scatter2.bin' binary record=30:30:29:26 endian=swap using 1:2:3
unset multiplot
pause -1 "Hit return to continue"
