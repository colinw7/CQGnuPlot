Rosenbrock(x,y) = (1-x)**2 + 100*(y - x**2)**2

set title "Sometimes it helps to use multiplot"
set view map
set xr [-0.5:1.5]
set yr [0:1.5]
unset xlabel
unset ylabel
set tics scale 0.0
set lmargin at screen 0.1
set rmargin at screen 0.9
set bmargin at screen 0.1
set tmargin at screen 0.9

set multiplot

set cntrparam levels discrete 0.1, 1.0, 10.0, 100.0
set isosamples 500,100
splot Rosenbrock(x,y) with lines lc rgb "#007700"

set isosamples 50,50
set cntrlabel start 25 interval -1 font ",7"
splot Rosenbrock(x,y) with labels boxed

unset multiplot
