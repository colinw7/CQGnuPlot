reset
# set label 1 "Demo of plotting financial data" at screen 0.5, screen 0.95 center
set title "Demo of plotting financial data"
set yrange [75:105]
set ytics (105, 100, 95, 90, 85, 80)
set xrange [50:253]
set lmargin 9
set rmargin 2
plot 'finance.dat' using 0:5 notitle with lines
