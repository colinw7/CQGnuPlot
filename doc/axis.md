## Axes

A group of plots has a border and an axis. The axis may be placed on the
border (one or both sides) on the zero line of the plot.

There are two axes, primary and secondary for each direction The primary
is displayed on the left/bottom of the plot and the optional secondary
on the right/top of the plot. If no secondary then the primary is used for
both sides.

The axis only has as line when it is not on the border The line can be drawn
with a line type or style (color and line dash) and width.

The axis ticks can point inside or outside the border. By default the border
tics point towards the center of the plot. For the zero axis line the
tics are both above and below the line.

The size (scale) of the major/minor tics can be specified to override the
default scale (1 for major, 0.5 for minor).

Major or minor axis tics can also be specified individually giving an
optional label and position. By default on major tics have a label.

The label format can also be specified.

The axis defines the primary and secondary range of the plot. The range
can be extended to get an exact multiple of the major axis distance or
can can be clipped to the exact range.

A grid can be drawn on the axes using either one or both of the major
or minor tics.

The axes can also be definied to the logarithm of the values to any base.
