sinc(u,v) = sin(sqrt(u**2+v**2)) / sqrt(u**2+v**2)

set title "\"fence plot\" using single parametric surface with undefined points"
# Another method suggested by Hans-Bernhard Broeker
# <broeker@physik.rwth-aachen.de>: Use parametric variable u to control
# x, y, and z.  u=3*fencenumber+remainder, where the remainder selects
# one of three types of points:
#         remainder=0: x=xmin+fencenumber*dx, y=v, z=zbase
#         remainder=1: x=xmin+fencenumber*dx, y=v, z=f(x,y)
#         remainder=2: x=xmin+fencenumber*dx, y=v, z=1/0 (undefined)
set parametric
unset label
unset arrow
xmin=-4.99; xmax=5; n=10; dx=(xmax-xmin)/(n-1.)
set samples 30,33
set isosamples 3*n,33
zbase=-1
set arrow from 5,-5,-1.2 to 5,5,-1.2 lt -1
set label 1 "increasing v" at 6,0,-1
set arrow from -5,-5,-1.2 to 5,-5,-1.2 lt -1
set label 2 "increasing u" at 0,-5,-1.5
set arrow from 5,6,-1 to 5,5,-1 lt -1
set label 3 "floor(u)%3=0" at 5,6.5,-1
set arrow from 5,6,sinc(5,5) to 5,5,sinc(5,5) lt -1
set label 4 "floor(u)%3=1" at 5,6.5,sinc(5,5)
splot [u=.5:3*n-.5][v=-4.99:4.99] \
         xmin+floor(u/3)*dx, v, ((floor(u)%3)==0) ? zbase : \
                         (((floor(u)%3)==1) ? sinc(xmin+u/3.*dx,v) : 1/0) lt 3 notitle
pause -1 "Hit return to continue (17)"
