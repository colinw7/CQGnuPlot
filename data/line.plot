set style line 1 lt 3 lw 2
set style line 2 lt 3 lw 4

plot "line.data" u 1:2 with l ls 1, "" u 1:3 with l ls 2
