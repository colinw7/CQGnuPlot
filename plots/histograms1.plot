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
plot 'data/immigration.dat' using 2:xtic(1) title columnheader(2), \
for [i=3:22] '' using i title columnheader(i)
#
pause -1 "<cr> to plot the same data as a histogram"
