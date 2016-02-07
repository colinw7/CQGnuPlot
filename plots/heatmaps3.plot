#
# Various ways to create a 2D heat map from ascii data
#

unset key
set tic scale 0

# Color runs from white to green
set palette rgbformula -7,2,-7
set cbrange [0:5]
set cblabel "Score"
unset cbtics

set xrange [-0.5:4.5]
set yrange [-0.5:4.5]

$map1 << EOD
5 4 3 1 0
2 2 0 0 1
0 0 0 1 0
0 0 0 2 3
0 1 2 4 3
EOD

set view map

set title "Heat map with non-zero pixel values written as labels"
plot $map1 matrix using 1:2:3 with image, \
     $map1 matrix using 1:2:($3 == 0 ? "" : sprintf("%g",$3) ) with labels
