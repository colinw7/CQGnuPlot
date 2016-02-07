set style fill solid 0.25 border -1
set style boxplot outliers pointtype 7
set style data boxplot
set boxwidth  0.5
set pointsize 0.5

unset key
set border 2
set ytics nomirror

set xtics auto
set yrange [*:*]
set title "Distribution of energy usage of the continents, grouped by type of energy source\n"
set ylabel "Billion Tons of Oil Equivalent"

set style boxplot sorted

factors = "Coal Gas Hydroelectric Nuclear Oil Renewable"
n_f = words(factors)
set xtic ("" 1)
set for [i=1:n_f] xtics add (word(factors,i) i)

t(x) = x/1.e6
filter(col, factor_col, level) = (strcol(factor_col) eq word(factors, level)) ? t(column(col)) : 1/0

plot for [i=1:n_f] 'data/energy_circles.dat' using (i):(filter(8, 4, i))
