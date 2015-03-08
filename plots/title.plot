unset key

set bmargin at screen .2; 
set rmargin at screen .8; set lmargin at screen .2;

set offset .08, .08, .08, .08
set border lw 0.5

set title "Rug Plot" font ",24" offset 0,.5

set xtics out scale 3
set ytics out scale 3

plot cos(x)
