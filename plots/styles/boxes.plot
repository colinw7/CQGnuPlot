set boxwidth 0.9 relative
set style fill solid 1.0

plot "data/boxes.data" using 1:2:(0.75) with boxes, "" using 1:3:4 with boxes
