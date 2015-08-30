set title "Mandelbrot function"
unset parametric
set mapping cartesian
set view 60,30,1,1
set auto
set isosamples 60
set hidden3d
compl(a,b)=a*{1,0}+b*{0,1}
mand(z,a,n) = n<=0 || abs(z)>100 ? 1:mand(z*z+a,a,n-1)+1
splot [-2:1][-1.5:1.5] mand({0,0},compl(x,y),30)
pause -1 "Hit return to continue"
