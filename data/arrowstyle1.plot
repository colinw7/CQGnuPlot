set title "Plot 'file' with vectors <arrowstyle>"
set key box opaque
set xrange [*:*]
set yrange [*:10]
set for [i=1:8] style arrow i lc i
plot '1.dat' using 1:2:(+1):(+1) with vectors lt 4 filled title 'filled', \
     '2.dat' using 1:2:(+1):(+1) with vectors lt 1 heads title 'double-headed', \
     '2.dat' using ($1):(2-$2/3):(+1):(+2.5):(int($0)%8 + 1) with vectors as var ti 'arrowstyle variable'
