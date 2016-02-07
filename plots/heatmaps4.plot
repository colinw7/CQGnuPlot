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

set view map

set title "Heat map from csv data with column and row labels"

$map3 << EOD
,Apple,Bacon,Cream,Donut,Eclair
Row 1, 5, 4, 3, 1, 0
Row 2, 2, 2, 0, 0, 1
Row 3, 0, 0, 0, 1, 0
Row 4, 0, 0, 0, 2, 3
Row 5, 0, 1, 2, 4, 3
EOD

set datafile separator comma
plot '$map3' matrix rowheaders columnheaders using 1:2:3 with image
set datafile separator
