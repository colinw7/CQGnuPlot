set style histogram clustered
set style data histograms

set style fill solid

set multiplot layout 2,1

set key off

plot "data/barchart.tsv"  u 2:xtic(1)

set key on

plot "data/barchart.data" u 2:xtic(1) t "Jane", "" u 3 t "John", "" u 4 t "Axel", "" u 5 t "Mary", "" u 6 t "Samantha"

unset multiplot
