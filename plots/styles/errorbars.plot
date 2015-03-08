set xrange [-0.5:12.75]
set xtic rotate by -45
set boxwidth 0.25

plot 'data/error.data' using (column(0)):4:3:xtic(1) w errorbars title col
