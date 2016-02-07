print ""
print "Some binary data files have headers, which may be skipped via"
print "the \'skip\' key word.  Here is the \'scatter.dem\' example"
print "again, but this time the first and third traces are skipped."
print "The first trace is 30 samples of three floats so takes up 360"
print "bytes of space.  Similarly, the third trace takes up 348 bytes."
print ""
set title "The key word \'skip\' used to ignore some data"
set xrange [20:100]
set yrange [0:800]
set zrange [0.2:1.8]
splot 'data/scatter2.bin' binary endian=little record=30:26 skip=360:348 origin=(50,0,0):(100,0,0) format='%f%f' using (0):2:3
pause -1 "Hit return to continue"
