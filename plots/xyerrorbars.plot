set samples 100
unset grid
set xlabel "Resistance [Ohm]"
set ylabel "Power [W]"
set title "UM1-Cell Power"
n(x)=1.53**2*x/(5.67+x)**2
plot [0:50] "data/battery.dat" t "Power" with xyerrorbars, n(x) t "Theory" w lines
