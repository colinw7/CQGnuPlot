set key autotitle columnheader

t2=98.2
t3=41.3
t4=19.3
t5=8.5
t6=5.3

plot "radar.data" using ($2/t2):($3/t3):($4/t4):($5/t5):($6/t6):xticlabels(1) with radar
