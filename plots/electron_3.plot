unset label
unset rmargin

# show off double axes

# amplitude frequency response
A(jw) = ({0,1}*jw/({0,1}*jw+p1)) * (1/(1+{0,1}*jw/p2))
p1 = 10
p2 = 10000
set dummy jw
set grid x y2
set logscale xy
set log x2
unset log y2
set key default
set key bottom center box
set title "Amplitude and Phase Frequency Response"
set xlabel "jw (radians)"
set xrange [1.1 : 90000.0]
set ylabel "magnitude of A(jw)"
set y2label "Phase of A(jw) (degrees)"
set ytics nomirror tc lt 1
set y2tics nomirror tc lt 3
set xtics mirror
set tics out
set autoscale  y
set autoscale y2
plot abs(A(jw)) lt 1, 180/pi*arg(A(jw)) axes x1y2 lt 3

pause -1 "Hit return to continue"
