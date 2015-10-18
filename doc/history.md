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

### 6/30/2015
 + add zero axis support
 + add named parameter range and sample
 + add norm function

### 7/3/2015
 + improve line type/line style handle (0, -1, -2, -3 indices)
 + add pentagon symbol
 + improve zero axis rendering with line type and tics above/below

### 7/6/2015
 + improve plot parsing for filled curves
 + support polar clip for filled curves 
 + improve polar plots (lines, impulses)
 + return complex values for real powers and out of range asin, acos, atan

### 7/11/2015
 + improve log axis

### 7/19/2015
 + improve polar plot (log on/off)
 + make time stamp configurable object

### 7/26/2015
 + Improve axis tics, labels for secondary axis
 + Support filled and border for labels style
 + Improve key columnhead and columns() using function
 + Support Symbol width
 + better position of plot title (above axis labels)

### 8/9/2015
 + 3D arrow
 + geographic format string
 + improve table output (discontinuity, contour and surface)
 + fix z axis position and border
 + margins in character units

### 8/17/2015
 + Improve rotated axis tic labels
 + Auto calc margins based on axis, colorbox and title
 + Use axis tic size on color box
 + Support margins for key placement
 + Improve pm3d handling (set pm3d view)
 + Pm3D virtual class
 + Filled curves pattern fill
 + add factorial to expression eval

### 8/30/2015
 + support per bar color for barchart plot
 + improve tooltip on barchart
 + fix margin for chord diagram

### 9/6/2015
 + improve adjacency caching and tip
 + improve piechart customization (inner radius, label radius) and tip
 + add inline styles in comments
 + add interactive support for "-" plot file
 + improve tootip widget and experimental cursor support

### 9/13/2015
 + add newhistogram support
 + add fill color option for key
 + add multiple object property edit for bars, pies
 + add more control for histogram properties (gap)

### 9/20/2015
 + improve axis label alignment when rotated

### 9/27/2015
 + add info tip support for all objects and annotations
 + better highlight/selected drawing
 + add object cache for arrow objects and use in vector plot for selectable info tip arrows
 + add log device to be used for regressions

### 9/4/2015
 + Improve text drawing for label style
 + Add error lines for bar object
 + Improve bubble plot highlight and tip
 + Add support for Utf space characters (WIP)
 + Add functions support for palette
 + Add Fill, Stroke Classes to abstract fill and stroke data
 + Add cbrange support to more plots
 + Fix tree map range
 + Add support for property tree search and expand

### 9/11/2015
 + Add fill and stroke object with editable properties
 + Add path object
 + Add object for error bar ends

### 9/18/2015
 + Add error bar and finance bar objects for errorbar and financebar plots
 + Add smooth for plot points
 + Add mark object for point properties
 + Fix symbol draw a value for errorbar plots
 + Add selection/highlight for chord diagram
