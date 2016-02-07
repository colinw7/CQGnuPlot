set style fill pattern
set style histogram clustered
plot "data/histogram.data" u 2 t "Red" w histograms, "" u 3 t "Green" w histograms, "" u 4 t "Blue" w histograms
