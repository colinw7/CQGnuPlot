set style histogram clustered horizontal
set style data histograms

set multiplot

plot "data/barchart.tsv" u 0:2:ytic(1)

unset multiplot
