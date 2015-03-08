set style fill solid

set yrange [10:40]

plot "data/step.data" using 1:2 with fillsteps
