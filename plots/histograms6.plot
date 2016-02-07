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
plot 'data/immigration.dat' using 6 ti col, '' using 12 ti col, \
     '' using 13 ti col, '' using 14:key(1) ti col
#
pause -1 "Next we do several sets of parallel histograms"
