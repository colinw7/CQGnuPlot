set title "Turn on grid"
set yrange [75:105]
set ytics (105, 100, 95, 90, 85, 80)
set xrange [50:253]
set lmargin 9
set rmargin 2
set grid
plot 'finance.dat' using 0:5 notitle with lines
