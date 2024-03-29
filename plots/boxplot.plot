#
# Boxplot demo
#
reset

print "*** Boxplot demo ***"

set style fill solid 0.25 border -1
set style boxplot outliers pointtype 7
set style data boxplot
set boxwidth  0.5
set pointsize 0.5

unset key
set border 2
set xtics ("A" 1, "B" 2) scale 0.0
set xtics nomirror
set ytics nomirror
set yrange [0:100]

plot 'data/silver.dat' using (1):2, '' using (2):(5*$3)

pause -1 'Hit <cr> to continue: Compare sub-datasets'

# Comparing sub-datasets

set xtics auto
set yrange [*:*]
set title "Distribution of energy usage of the continents, grouped by type of energy source\n"
set ylabel "Billion Tons of Oil Equivalent"

plot 'data/energy_circles.dat' using (1):($8/1.e6):(0):4

pause -1 'Hit <cr> to continue: Sort factors alphabetically'

# Sort factors alphabetically

set style boxplot sorted

replot

pause -1 'Hit <cr> to continue: The same, with iteration and manual filtering'

# The same as above, with manual filtering
# Note that you have to specify the factors and you have to set the xtics as well.
# However, you have greater control over the appearance of the plot
# e.g. the order of the boxplots, their colors, the tic labels
# The previous form is intended for interactive usage while the latter form is better suited
# to creating publication-ready graphs.

factors = "Coal Gas Hydroelectric Nuclear Oil Renewable"
n_f = words(factors)
set xtic ("" 1)
set for [i=1:n_f] xtics add (word(factors,i) i) 

t(x) = x/1.e6
filter(col, factor_col, level) = (strcol(factor_col) eq word(factors, level)) ? t(column(col)) : 1/0

plot for [i=1:n_f] 'data/energy_circles.dat' using (i):(filter(8, 4, i)) 

pause -1 'Hit <cr> to continue: boxplot demo finished'
