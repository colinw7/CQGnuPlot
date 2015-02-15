nsamp = 50

do for [i=1:nsamp] {
  print sprintf("%8.5g %8.5g", invnorm(rand(0)), invnorm(rand(0)))
}
