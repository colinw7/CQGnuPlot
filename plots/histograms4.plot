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
set title "US immigration from Northern Europe\n(same plot with larger gap between clusters)"
set style histogram gap 5
#
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
plot 'data/immigration.dat' using 2:xtic(1), for [i=3:22] '' using i
#
pause -1 "<cr> to rescale each stack to % of total"
