set xrange [-1000:1000]
set yrange [-178:86]
set tics scale 0

set style line 1 lt 1 lw 2
set style line 2 lt 3 lw 2

set style arrow 1 head filled size screen 0.025,30,45 ls 1
set style arrow 2 head nofilled size screen 0.03,15 ls 2
set style arrow 3 head filled size screen 0.03,15,45 ls 1
set style arrow 4 head filled size screen 0.03,15 ls 2
set style arrow 5 heads filled size screen 0.03,15,135 ls 1
set style arrow 6 head empty size screen 0.03,15,135 ls 2
set style arrow 7 nohead ls 1
set style arrow 8 heads size screen 0.008,90 ls 2

set title 'Top: plot with vectors arrowstyle 1, Bottom: explicit arrows'
plot 'data/arrowstyle.dat' using 1:2:(0):3 notitle with vectors arrowstyle 1
