set xlabel "Cluster Size [thousands]"
set ylabel "Running Time [sec]"
set title "DLA Growth Program: Run Time vs Cluster Size (Double Logarithmic)"

set key top left
set logscale

#plot [0.5:40][.5:30000] "dlacluster.txt" u 2:6 title "data" w p, 1.2*x**2.7 notitle
plot [0.5:40][.5:30000] 1.2*x**2.7 title "data" w p
