set title "Binary data specified at the command line, intended for use through pipe"
set xrange [-1.5:3.5]
set yrange [-1.5:3.5]
set cbrange [0:1]
plot '-' binary endian=little array=(2,2) dx=2 format="%float" using 1:2:3 with rgbimage,\
     '-' binary endian=little record=4 format="%char" using 1:2 with points pt 7 ps 2 lt -1
           ?       ?           ?               ?    
pause -1 "Press return to continue"
