set title "Finance bars"
set yrange [75:105]
set ytics (105, 100, 95, 90, 85, 80)
set xrange [50:253]
set lmargin 9
set rmargin 2
set grid
set logscale y
plot 'data/finance.dat' using 0:2:3:4:5 notitle with financebars lt 8
