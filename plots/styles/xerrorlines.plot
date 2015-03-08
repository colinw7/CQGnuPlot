set samples 100

unset grid

set xlabel "Resistance [Ohm]"
set ylabel "Power [W]"

set title "UM1-Cell Power"

set xrange [0.02:0.12]
set yrange [-1:20]

plot "data/battery.dat" using 2:0:4 t "Power" with xerrorlines
