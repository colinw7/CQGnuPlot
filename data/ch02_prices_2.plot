plot "ch02_prices.txt" using 1:2 with linesp, "" title "bezier" smooth bezier, "" using 1:2 title "csplines" smooth csplines
