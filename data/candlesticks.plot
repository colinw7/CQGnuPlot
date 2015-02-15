set xrange [0:11]
set yrange [0:10]
set title "candlesticks showing both states of open/close"
set style fill empty
set boxwidth 0.2
plot 'candlesticks.dat' using 1:(int($0)%3?$3:$5):2:6:(int($0)%3?$5:$3) with candlesticks title "open < close", NaN with boxes lt 1 fs solid 1 title "close < open"
