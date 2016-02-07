set yrange [-4:10]
set xrange [0:11]

set boxwidth 0.2 abs
set bars front

by3(x) = (((int(x)%3)+1)/6.)
by4(x) = (((int(x)%4)+1)/7.)

rgbfudge(x) = x*51*32768 + (11-x)*51*128 + int(abs(5.5-x)*510/9.)

set yrange [-4:10]
set xrange [0:11]

unset key #below 

set title "variable color points, circles, candlesticks, boxes, and boxxyerror"

#plot 'data/candlesticks.dat' using 1:(1):1 with points pt 11 lc variable
#plot 'data/candlesticks.dat' using 1:2:(.1):1 with circles lc variable
plot 'data/candlesticks.dat' using 1:3:2:6:5:1 with candlesticks lc variable
#plot 'data/candlesticks.dat' using ($1+.3):3:2:6:5:1 with financebars lc variable
#plot 'data/candlesticks.dat' using 1:(8):(by3($0)):(by4($0)):1 with boxxy lc var fs solid
#plot 'data/candlesticks.dat' using 1:(-$2/2):1 with boxes lc var
