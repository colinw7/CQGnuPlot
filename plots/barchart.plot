set style histogram clustered
set style data histograms

set multiplot

plot "barchart.tsv"  u 0:2:xtic(1)
plot "barchart.data" u 0:1:xtic(1), "" u 0:2:xtic(1), "" u 0:3:xtic(1), "" u 0:4:xtic(1), "" u 0:5:xtic(1)

unset multiplot
