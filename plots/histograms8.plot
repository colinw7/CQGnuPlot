#
# Example of using histogram modes
#
reset
set title "US immigration from Europe by decade"
set datafile missing "-"
set xtics nomirror rotate by -45
set key noenhanced
#
# First plot using linespoints
set style data linespoints
#
set title "US immigration from Northern Europe\nPlot selected data columns as histogram of clustered boxes"
set auto x
set yrange [0:300000]
set style data histogram
set style histogram cluster gap 1
set style fill solid border -1
set boxwidth 0.9
set xtic rotate by -45 scale 0
#set bmargin 10 
#
set title "US immigration from Northern Europe\n(same plot with larger gap between clusters)"
set style histogram gap 5
#
# Stacked histograms
#
set title "US immigration from Europe by decade\nPlot as stacked histogram"
set key invert reverse Left outside
set key autotitle columnheader
set yrange [0:7e6]
set auto x
unset xtics
set xtics nomirror rotate by -45 scale 0
set style data histogram
set style histogram rowstacked
set style fill solid border -1
set boxwidth 0.75
#
# Stacked histograms by percent
#
set title "US immigration from Europe by decade\nFraction of total plotted as stacked histogram"
set key invert reverse Left outside
set yrange [0:100]
set ylabel "% of total"
unset ytics
set grid y
set border 3
set style data histograms
set style histogram rowstacked
set style fill solid border -1
set boxwidth 0.75
#
# Columnstacks
#	xtic labels should be picked up from column heads ('title column')
#	key titles should be picked up from row heads ('key(1)')
#
set title "Immigration from Northern Europe\n(columstacked histogram)"
set style histogram columnstacked
set key noinvert box
set yrange [0:*]
set ylabel "Immigration by decade"
set xlabel "Country of Origin"
set tics scale 0.0
set ytics
unset xtics
set xtics norotate nomirror
#
# 'newhistogram' keyword to plot
#
set title "Immigration from different regions\n(give each histogram a separate title)"
set key under nobox
set style histogram clustered gap 1 title offset 2,0.25
set style fill solid noborder
set boxwidth 0.95
unset xtics
set xtics nomirror rotate by -45 scale 0 font ",8"
set xlabel "(note: histogram titles have specified offset relative to X-axis label)" offset 0,-2
set ytics font ",8"
set grid y
set auto y

set style histogram rows
set boxwidth 0.8
set yrange [0:900000]
set xlabel "(Same plot using rowstacked rather than clustered histogram)"

plot \
newhistogram "Northern Europe", \
'data/immigration.dat' using "Sweden":xtic(1) t col, '' u "Denmark" t col, '' u "Norway" t col, \
newhistogram "Southern Europe", \
'' u "Greece":xtic(1) t col, '' u "Romania" t col, '' u "Yugoslavia" t col, \
newhistogram "British Isles", \
'' u "Ireland":xtic(1) t col, '' u "United_Kingdom" t col
#
pause -1 "<cr> to finish histogram demo"

