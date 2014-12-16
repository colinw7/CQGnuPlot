set datafile missing "NaN"

plot [][9:15] "missing.data" using 1:2 with linespoints, "" using 1:($2)+2 with linespoints
