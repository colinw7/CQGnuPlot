set object circle at 0,0 size 1 fillcolor rgb "green"
set object ellipse at 0,0 size 1,0.5 fs solid fillcolor rgb "blue"
set object polygon from 0,0.5 to 0.5,0 to 1,0.5 to 0.5,1 lw 2
set object rectangle from -2,-2 to -1,-1

plot [-2:2][-2:2] x**2
