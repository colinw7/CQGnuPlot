set boxwidth 0.2 abs
set bars front

by3(x) = (((int(x)%3)+1)/6.)
by4(x) = (((int(x)%4)+1)/7.)

set yrange [-4:10]
set xrange [0:11]
unset key #below 

set title "variable color boxerror, xyerrorbars, impulses, vectors, and labels"

plot 'data/candlesticks.dat' using 1:5:2:6:(.2):1 with boxerror lc var fs solid 0.5 border -1

#plot 'candlesticks.dat' \
        using 1:5:2:6:(.2):1 with boxerror lc var fs solid 0.5 border -1 , \
     '' using 1:(1):1 with points pt 11 lc variable, \
     '' using 1:(8):(by3($0)):(by4($0)):1 with xyerrorbars lc var, \
     '' using ($1+.5):($2/2):1 with impulses lc var lw 3,\
     '' using 1:(-3):(0.5):(1):1 with vectors lc var,\
     '' using 1:(-1):1:1 with labels tc variable
