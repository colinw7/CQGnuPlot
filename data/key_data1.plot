set style data linesp
set key autotitle columnhead
plot "key_data.txt" u 1:2 t "Wheat Data", "" u 1:3 t "Barley Data", "" u 1:4 t "Rye Data"
