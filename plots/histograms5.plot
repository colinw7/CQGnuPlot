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
plot 'data/immigration.dat' using (100.*$2/$24):xtic(1) t column(2), \
     for [i=3:23] '' using (100.*column(i)/column(24)) title column(i) 
#
pause -1 "Now try histograms stacked by columns"
