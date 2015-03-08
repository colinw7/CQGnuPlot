set terminal svg background rgb "#8888AA"
set terminal qt

set object 1 rectangle from screen 0,0 to screen 1,1 fillcolor bgnd behind
set object 2 rectangle from screen 0.25,0.25 to screen 0.75,0.75 fillcolor rgb "green" behind
plot cos(x)
