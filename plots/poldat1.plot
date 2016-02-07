set size square
set time
set clip
set xtics axis nomirror
set ytics axis nomirror
unset grid
unset polar
set title "Primitive Smith Chart"
unset key
set xlabel "Impedance or Admittance Coordinates"
set para
set rrange [-0 : 10]
set trange [-pi : pi]
set xrange [-1:1]
set yrange [-1:1]
tv(t,r) = sin(t)/(1+r)
tu(t,r) = (cos(t) +r)/(1+r)
cu(t,x) = 1 + cos(t)/x
cv(t,x) = (1+ sin(t))/x
plot cu(t,.1),cv(t,.1),cu(t,.1),-cv(t,.1),\
cu(t,1),cv(t,1),cu(t,1),-cv(t,1),\
cu(t,10),cv(t,10),cu(t,10),-cv(t,10),\
tu(t,.1),tv(t,.1),\
tu(t,.5),tv(t,.5),\
tu(t,1),tv(t,1),\
tu(t,5),tv(t,5),\
tu(t,10),tv(t,10),\
cu(t,.5),cv(t,.5),cu(t,.5),-cv(t,.5),\
tu(t,0),tv(t,0)
#cu(t,5),cv(t,5),cu(t,5),-cv(t,5)
