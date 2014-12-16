set pm3d
set view map
set size square

unset surface

set isosamples 300
#set isosamples 100

#set palette model RGB defined ( 0 'black', 1 'white' )

f(x,y,n,m) = cos(n*x)*cos(m*y)+cos(m*x)*cos(n*y)

#set palette defined ( 0 'magenta', 1 '#0066ff', 2 'cyan', 3 '#009900', 4 'green', 5 '#996633', 6 'orange', 7 'yellow', 8 'red' )
set palette defined ( 0 'magenta', 1 '#3366ff', 2 '#99ffcc', 3 '#339900', 4 '#66ff33', 5 '#996633', 6 '#ff9900', 7 '#ffff33', 8 'red' )

splot [-6:6][-6:6] f(x,y,2,5)
