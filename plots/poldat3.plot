set polar
set grid polar
set border 0
set logscale r 10
set view equal xy
set size square
set noxtics
set noytics
set rrange [.5 : 19]
set trange [pi/2 : 2*pi]
set rtics axis scale 0.5,0 nomirror rotate by -270  offset 0, 0.7
set rtics (2,5,10,15)
set key opaque box samplen 0.75
set title "log scale polar axis"
#
set style data impulse
plot 'data/silver.dat' using ($1/100.):3:0 lc var lw 2 title 'silver.dat'
