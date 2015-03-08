set terminal wxt enhanced

set key top left box

set arrow 1 from -4,.35 to -3.4,0.2
set label 1 "Cusp at x=-{/Symbol p} ?" at -6,0.4

set arrow 2 from 5,-.2 to 5,.95 nohead linetype 4
set label 2 "Singularity at x=5" at 5,1 center

set arrow 3 from -1,0.2 to 1,0.2 heads size 0.1,90
set label 3 "Full Width at\nHalf Maximum" at 0,0.15 center

plot [-7:7][-.2:1.1] 1/(1+x**2) t "Lorentzian", exp(-x**2) t "Gaussian"
