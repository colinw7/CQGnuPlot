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

set title "Heat map from csv data with column and row labels"

# Some output modes (SVG, HTML5) allow smoothing of adjacent pixels,
# which may not be desired. This is not normally an issue with PNG or PDF.
# The `pixels` option forces pixel-by-pixel drawing with no smoothing.

unset colorbox
set format xy ""
set multiplot layout 1,2 title "Compare 'image' and 'image pixels' modes" \
    margin 0.05, 0.95, 0.10, 0.85 spacing 0.05
set title "plot with image"
plot $map1 matrix using 1:2:3 with image
set title "plot with image pixels"
plot $map1 matrix using 1:2:3 with image pixels
unset multiplot
