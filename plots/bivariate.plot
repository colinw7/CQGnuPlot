set xdata time

set timefmt "%Y%m%d"

plot "data/bivariate.tsv" with bivariate using 1:2:3
