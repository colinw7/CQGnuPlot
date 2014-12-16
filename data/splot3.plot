set view map
set size square
unset surface
set contour
set cntrparam levels 10
set clabel "%.1f"
set isosamples 150

splot [-2:2][-2:2] exp(-(x**2+y**2))*cos(x/4)*sin(y)*cos(2*(x**2+y**2))
