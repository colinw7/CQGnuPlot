set samples 100
unset grid
set xlabel "Resistance [Ohm]"
set ylabel "Power [W]"
set title "UM1-Cell Power"
plot [0:50] "data/battery.dat" t "Power" with xyerrorlines
