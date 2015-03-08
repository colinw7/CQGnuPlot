set samples 100

unset grid

set xlabel "Resistance [Ohm]"
set ylabel "Power [W]"

set title "UM1-Cell Power"

set xrange [-1:20]
set yrange [0.02:0.12]

plot "data/battery.dat" using 0:2:4 t "Power" with yerrorbars
