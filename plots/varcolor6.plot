set boxwidth 0.2 abs
set bars front

by3(x) = (((int(x)%3)+1)/6.)
by4(x) = (((int(x)%4)+1)/7.)

rgbfudge(x) = x*51*32768 + (11-x)*51*128 + int(abs(5.5-x)*510/9.)

set yrange [-4:10]
set xrange [0:11]
unset key #below 

#plot 'candlesticks.dat' using 1:5:2:6:(.2):(rgbfudge($1)) with boxerror lc rgb var fs solid 0.5 noborder
#plot 'candlesticks.dat' using 1:(1):(rgbfudge($1)) with points pt 11 lc rgb var
#plot 'candlesticks.dat' using 1:(8):(by3($0)):(by4($0)):(rgbfudge($1)) with xyerrorbars lc rgb var
#plot 'candlesticks.dat' using ($1+.5):($2/2):(rgbfudge($1)) with impulses lc rgb var lw 3
#plot 'candlesticks.dat' using 1:(-3):(0.5):(1):(rgbfudge($1)) with vectors lc rgb var
plot 'candlesticks.dat' using 1:(-1):1:(rgbfudge($1)) with labels tc rgb var point
