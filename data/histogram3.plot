set style line 1 lc rgb 'grey30'
set style line 2 lc rgb 'grey50'
set style line 3 lc rgb 'grey70'

set style increment user

set style fill solid 1 border -1

set style histogram rowstacked
set style data histograms

plot "histogram.data" u 2 t "Red", "" u 3 t "Green", "" u 4 t "Blue"
