set xdata time

set timefmt "%y-%b-%d"

plot "data/stacked_area.tsv" with stackedarea using 1:2:3:4:5:6
