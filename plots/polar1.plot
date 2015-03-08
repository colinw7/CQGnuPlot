unset border
set clip
set polar
set xtics axis nomirror
set ytics axis nomirror
unset rtics
set samples 160
set zeroaxis
set trange [0:2*pi]
set title "Three circles (with aspect ratio distortion)"
plot .5,1,1.5
