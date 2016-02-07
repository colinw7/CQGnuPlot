#
# gcd(x,y) finds the greatest common divisor of x and y,
#          using Euclid's algorithm
# as this is defined only for integers, first round to the nearest integer
min(x,y) = (x < y) ? x : y
max(x,y) = (x > y) ? x : y

gcd(x,y) = gcd1(rnd(max(x,y)),rnd(min(x,y)))
gcd1(x,y) = (y == 0) ? x : gcd1(y, x - x/y * y)
rnd(x) = int(x+0.5)

set samples 59
set xrange [1:59]
set auto
set key default

set title "Greatest Common Divisor (for integers only)"

plot gcd(x, 60) with impulses
