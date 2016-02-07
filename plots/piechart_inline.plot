set title "Desktop Search Engine Market Share, Dec-2012" font "Helvetica,20"

set key left reverse

plot "-" using 2:1:fillcolor($3):linecolor("white") with piechart startangle 20 labelradius 0.8
# --style--
83.24 "Google" "#4f81bc"
8.16 "Yahoo!"  "#c0504e"
4.67 "Bing"    "#9bbb58"
1.67 "Baidu"   "#23bfaa"
0.98 "Others"  "#8064a1"
e

pause -1 "Press return to continue"
