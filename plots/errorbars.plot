# set terminal png transparent nocrop enhanced size 450,320 font "arial,8" 
# set output 'mgr.1.png'
set key inside right top vertical Right noreverse enhanced autotitle box lt black linewidth 1.000 dashtype solid
set samples 50, 50
set style data lines
set title "Bragg reflection -- Peak only" 
set xlabel "Angle (deg)" 
set ylabel "Amplitude" 
x = 0.0
## Last datafile plotted: "big_peak.dat"
plot "data/big_peak.dat" title "Rate" with errorbars, "" smooth csplines t "Rate"
