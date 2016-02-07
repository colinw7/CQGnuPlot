#
# This definition computes the sum of the first 10, 100, 1000 fourier
# coefficients of a (particular) square wave.

set title "Finite summation of 10, 100, 1000 fourier coefficients"

set samples 500
set xrange [-10:10]
set yrange [-0.4:1.2]
set key bottom right

fourier(k, x) = sin(3./2*k)/k * 2./3*cos(k*x)
sum10(x)   = 1./2 + sum [k=1:10]   fourier(k, x)
sum100(x)  = 1./2 + sum [k=1:100]  fourier(k, x)
sum1000(x) = 1./2 + sum [k=1:1000] fourier(k, x)

plot \
    sum10(x)   title "1./2 + sum [k=1:10]   sin(3./2*k)/k * 2./3*cos(k*x)", \
    sum100(x)  title "1./2 + sum [k=1:100]  sin(3./2*k)/k * 2./3*cos(k*x)", \
    sum1000(x) title "1./2 + sum [k=1:1000] sin(3./2*k)/k * 2./3*cos(k*x)"
