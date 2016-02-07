print ""
print "...and \'splot\'.  Here is the \'scatter.dem\' example again,"
print "but this simulates the case of the redundant x coordinates not"
print "being in the binary file.  The first \"column\" of the binary"
print "file is ignored and reconstructed by orienting the various"
print "data records."
print ""
set title "2d binary data example where x coordinate is ignored then generated"
set xrange [20:100]
set yrange [0:800]
set zrange [0.2:1.8]
splot 'data/scatter2.bin' binary endian=little record=30:30:29:26 origin=(25,0,0):(50,0,0):(75,0,0):(100,0,0) format='%f%f' using (0):2:3
pause -1 "Hit return to continue"
