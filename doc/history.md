### 5/25/2015
 + improve plot command split parsing
 + Improve image, rgbimage plots origin/center
 + add 3d image plot
 + Improve polar plot
 + Enhanced text on title and axis
 + SVG save from menu

### 5/26/2015
 + improve xtics parsing
 + boxplot improvements - fill density, range, axes, tics, outlier symbol

### 5/29/2015
 + fix strstrt issue
 + add octal chars in string
 + fix command semi-colon split
 + colorbox axis improvements
 + improve utf8 text handling
 + fix sinh,cosh,tanh for complex
 + fix variable color column for 3d sureface plots

### 6/7/2015
 + improve 3d axis drawing
 + improve 3d camera
 + improve contour plot (support base and/or surface, levels)
 + add contour levels to key
 + improve surface plot
 + support log z axis
 + zoom support in 3d mode
 + add svg buttons to toolbar
 + add slider edit for camera angle

### 6/8/2015
 + Display group name with x/y pos
 + only select group items if inside group region
 + improve bar chart range and tic labels
 + support multiplot rows/cols first and downward/upward

### 6/11/2015
 + improve parsing of multiple functions and variables on plot line
 + support default dash types
 + support point interval in linespoints
 + set plot title for binary data

### 6/14/2015
 + move Qt enums to separate class
 + improve handling of linetype/linestyle
 + improve dash support

### 6/20/2015
 + Use expression code to parse reals, integers and strings
 + Support black, nodraw, bgnd line types
 + support dash strings
 + pre-parse user functions

### 6/29/2015
 + Improve speed of expression evaluation by pre-compiling user functions and
   reducing nuber of memory allocations
 + mark all default functions as builtin
 + support named range variable
 + replace backtick command in double quotes
 + support PNG terminal type
