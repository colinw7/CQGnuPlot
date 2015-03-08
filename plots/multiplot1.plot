t(x,n) = cos(n*acos(x))

set xrange [-1:1]
set yrange [-1:1]

set multiplot title "Chebyshev Polynomials" layout 3,2

plot t(x,1)
plot t(x,2)
plot t(x,3)
plot t(x,4)
plot t(x,5)
plot t(x,6)

unset multiplot
