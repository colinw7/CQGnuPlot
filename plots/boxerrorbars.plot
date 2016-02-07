#plot 'error.data' using 4:xtic(1) title col, '' using 2:xtic(1) title col;

set xrange [-0.5:12.75]
set xtic rotate by -45
set boxwidth 0.25

#plot 'data/error.data' using ($0-.05):4:5:xtic(1) with boxerrorbars title col, '' using ($0+0.25):2:3 with boxerrorbars title col

#plot 'data/error.data' using (column(0)):4:3:xtic(1) w errorbars title col, '' using (column(0)):2:5:xtic(1) w errorbars title col

plot 'data/error.data' using (column(0)):4:3:xtic(1) w errorbars title col
