set boxwidth 0.2 abs
set bars front

by3(x) = (((int(x)%3)+1)/6.)
by4(x) = (((int(x)%4)+1)/7.)

rgbfudge(x) = x*51*32768 + (11-x)*51*128 + int(abs(5.5-x)*510/9.)

set yrange [-4:10]
set xrange [0:11]
unset key #below 

#plot 'candlesticks.dat' using 1:5:2:6:(.2):1 with boxerror lc pal z fs solid 0.5
#plot 'candlesticks.dat' using 1:(1):1 with points pt 11 lc pal z
#plot 'candlesticks.dat' using 1:(8):(by3($0)):(by4($0)):1 with xyerrorbars lc pal z
#plot 'candlesticks.dat' using ($1+.5):($2/2):1 with impulses lc pal z lw 3
#plot 'candlesticks.dat' using 1:(-3):(0.5):(1):1 with vectors lc pal z
plot 'candlesticks.dat' using 1:(-1):1:1 with labels tc pal z
