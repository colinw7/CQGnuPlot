set view 90.0
set isosamples 200,2
unset ytics
set xtics offset 0,-2
set border 4095
set xyplane 0
set palette defined ( 0 'red', 0.25 'red', 0.5 'green', 1.5 'green', 1.75 'red', 2 'red' )

splot sin(x)**3+0.3*sin(5*x) w l palette
