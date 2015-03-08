set style fill pattern
set style histogram rowstacked
set style data histograms

plot "data/histogram.data" u 2 t "Red", "" u 3 t "Green", "" u 4 t "Blue"
