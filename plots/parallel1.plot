set title "Parallel Axis Plot"

#set border 0
unset key
set xrange [] noextend
unset ytics

# Use x-axis tic positions to label the axes
set xtics 1 format "axis %g" scale 0,0

# Turn on axis tics for the parallel axes
set for [i=1:5] paxis i tics

# Use the range commands to create an "interesting" plot.
# For suitable input data this is the sort of result you
# might get without axis range manipulation.

set paxis 2 range  [0:30]
set paxis 4 range  [-10:50]
set paxis 5 range  [50:*] reverse
set paxis 5 tics left offset 4

plot 'data/silver.dat' using 2:3:1:($3/2):2:(int($0/25)) with parallel lt 1 lc variable
