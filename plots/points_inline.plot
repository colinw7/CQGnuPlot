set title "Multi Series Points"

plot "-" using 1:2:3 with points title columnheader(1)
# --style-- linecolor rgb "blue" pointtype 7 pointsize 3 tippoints
"Data Series 1"
10 71 12
20 55 23
30 50 6
40 65 2
50 95 50
60 68 13
70 28 21
80 34 5
90 14 7


# --style-- linecolor rgb "red" pointtype 11 pointsize 4 tippoints
"Data Series 2"
10 41 12
20 25 23
30 80 6
40 35 2
50 65 50
60 38 13
70 48 21
80 14 5
90 44 7
e

pause -1 "Press return to continue"
