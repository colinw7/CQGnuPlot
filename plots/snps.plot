set timefmt "%Y/%m/%d"

set xdata time

plot "snps.data" using 1:2:4:3:5 with financebars

pause -1 "Hit return to continue"

plot "snps.data" using 1:2 with lines

pause -1 "Hit return to continue"
