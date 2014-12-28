set timefmt "%Y-%m-%d"
set format x "%d%b%y"
set xdata time

set ytics nomirror # Switch mirroring of primary system OFF
set y2tics         # Switch secondary system ON

set ylabel  "Mean Icecream Consumption per Head [Grams]"
set y2label "Mean Temperature [Celsius]"

set key top left reverse Left

plot ["1951-03-25":] "multi_axes.data" u 1:2 t "Icecream" axes x1y1 w linesp, "" u 1:3 axes x1y2 t "Temperature" w linesp
