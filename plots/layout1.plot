set multiplot layout 3, 1 title "Multiplot layout 3, 1" font ",14"
set tmargin 2
set title "Plot 1"
unset key
plot sin(x)/x
#
set title "Plot 2"
unset key
plot 'silver.dat' using 1:2 ti 'silver.dat'
#
set style histogram columns
set style fill solid
set key autotitle column
set boxwidth 0.8
set format y "    "
set tics scale 0
set title "Plot 3"
plot 'immigration.dat' using 2 with histograms, \
     '' using 7  with histograms , \
     '' using 8  with histograms , \
     '' using 11 with histograms 
#
unset multiplot
