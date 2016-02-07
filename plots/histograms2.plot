#
# Example of using histogram modes
#
reset
set title "US immigration from Europe by decade"
set datafile missing "-"
set xtics nomirror rotate by -45
set key noenhanced
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
plot 'data/immigration.dat' using 6:xtic(1) ti col, '' u 12 ti col, '' u 13 ti col, '' u 14 ti col
#
pause -1 "<cr> to change the gap between clusters"
