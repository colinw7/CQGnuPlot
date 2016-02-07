set dummy u,v
set samples 51
set isosample 21

set title "Sinc function"
set zrange [-1:1]
set label 1 "This is equal to 1" at 0,3.2,1 left
set arrow 1 from 0,3,1 to 0,0,1
sinc(u,v) = sin(sqrt(u**2+v**2)) / sqrt(u**2+v**2)
splot [-5:5.01] [-5:5.01] sinc(u,v)
pause -1 "Hit return to continue (14)"
