print ""
print "It is possible to enter binary data at the command line.  Of"
print "course, the limitation to this approach is that keyboards will"
print "allow entering only a limited subset of the possible character"
print "values necessary to represent general binary data.  For this"
print "reason, the primary application for binary data at the command"
print "line is using Gnuplot through a pipe.  For example, if a pipe"
print "is established with a C program, the function \'fputs()\' can"
print "send ASCII strings containing the Gnuplot commands while the"
print "function \'fwrite()\' can send binary data."
print ""
print "Furthermore, there can be no special ending character such as"
print "in the case of ASCII data entry where \'e\' represents the end"
print "of data for the special file \'-\'.  It is important to note"
print "that when \'binary\' is specified, Gnuplot will continue"
print "reading until reaching the number of elements specified via"
print "the \'array\' or \'record\' command."
print ""
print "Here is an example of binary data at the command line where"
print "keyboard input has been side stepped by copying 48 bytes from"
print "a pre-existing binary file into this demo file."
print ""
set title "Binary data specified at the command line, intended for use through pipe"
set xrange [-1.5:3.5]
set yrange [-1.5:3.5]
set cbrange [0:1]
plot '-' binary endian=little array=(2,2) dx=2 format="%float" using 1:2:3 with rgbimage,\
     '-' binary endian=little record=4 format="%char" using 1:2 with points pt 7 ps 2 lt -1
           ?       ?           ?               ?    
pause -1 "Hit return to continue"
