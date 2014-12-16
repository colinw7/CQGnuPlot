set pm3d

splot [-2:2][-2:2] exp(-(x**2+y**2))*cos(x/4)*sin(y)*cos(2*(x**2+y**2))
