set title "3D surface from a grid (matrix) of Z values"
set xrange [-0.5:4.5]
set yrange [-0.5:4.5]

set grid
set hidden3d
$grid << EOD
5 4 3 1 0
2 2 0 0 1
0 0 0 1 0
0 0 0 2 3
0 1 2 4 3
EOD
splot '$grid' matrix with lines notitle lc rgb "blue"

pause -1 "Hit return to continue"
unset hidden3d
undefine $*
