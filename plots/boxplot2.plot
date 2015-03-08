#
# Boxplot demo
#
set style fill solid 0.25 border -1
set style boxplot outliers pointtype 7
set style data boxplot
set boxwidth  0.5
set pointsize 0.5

unset key
set border 2
set xtics auto
set xtics nomirror
set ytics nomirror
set yrange [*:*]

set title "Distribution of energy usage of the continents, grouped by type of energy source\n"
set ylabel "Billion Tons of Oil Equivalent"

plot 'energy_circles.dat' using (1):($8/1.e6):(0):4
