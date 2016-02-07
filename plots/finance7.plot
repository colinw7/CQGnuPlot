set title "Add some boiler plate"
set label "Courtesy of Bollinger Capital" at graph 0.01, 0.07
set label "  www.BollingerBands.com" at graph 0.01, 0.03
set yrange [75:105]
set ytics (105, 100, 95, 90, 85, 80)
set xrange [50:253]
set lmargin 9
set rmargin 2
set grid
set logscale y
plot 'data/finance.dat' using 0:2:3:4:5 notitle with financebars lt 8, \
'data/finance.dat' using 0:9 notitle with lines lt 3, \
'data/finance.dat' using 0:10 notitle with lines lt 1, \
'data/finance.dat' using 0:11 notitle with lines lt 2, \
'data/finance.dat' using 0:8 axes x1y2 notitle with lines lt 4
