set pm3d
set view map
set size square
unset surface
set isosamples 300

#set palette model RGB defined ( 0 'black', 1 'white' )
set palette model RGB defined (0 'white', 10 'white', 225 'black', 1500 'white', 9999 'white', 10000 'black')

splot "data/fractal.data"
