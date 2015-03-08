set terminal qt enhanced

set label 1 "a^x" at -9,0
set label 2 "a_x" at -7,0
set label 3 "a@^b_{cd}" at -5,0
set label 4 "d&{space}b" at -3,0
set label 5 "~a{.8-}" at -1,0
set label 6 "{/Times abc}" at 1,0
set label 7 "{/Times*2 abc}" at 3,0
set label 8 "{/Times:Italic abc}" at 5,0
set label 9 "{/Arial:Bold=20 abc}" at 7,0

set arrow 10 from -10,0 to 10,0

plot cos(x)
