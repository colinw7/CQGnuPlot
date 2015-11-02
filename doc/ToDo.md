## ToDo

  + Better axes ticks for log (match base powers)
  + allow select/modifiy all objects (plots, axis, ...)
  + edit interface (move) for annotations
  + save cmd
  + user shared_ptr/unique_ptr instead of CRefPtr
  + more color schemes (selectable)
  + don't recalc bar/bubble/pie on plot
  + consistent handling of position front,back,behind
  + startup file
  + improve variable color, ... handling for each plot type
  + rationalize use of line style/line type (one copies current values, one use global values)
  + plot options (after with) are type specific
  + cleanup axis usage (single class instead of two)
  + axis data
  + use color box in test palette command
  + lookup box by id to avoid overwritting object values
  + reset on loadCmd

  + zoom on mouse
  + zoom on wheel
  + 3d zoom/probe

  + more axis property settings
  + more bar settings (fill, border, ...)
  + better histogram display
  + fix histogram init range
  + multiple ranges on plot

  + rationalize range code
    + plot has x/y data range
    + group has adjusted data range (and non-adjusted ? to allow axis update)

  + change multiple values (all bars)

  + log scale broken
  + filled curves demo bad display

  + new histogram multiple histogram groups

  + renderer pixel data with no transform
  + images rendered to image area instead of individual pixels

  + multiple variable values

  + fit command
  + import

  + improve property editors (popup menu)
  + multiple value changes
  + filter properties

  + parse real with suffix 1.3d, dont allow parse real in quotes ?

  + better input of points in polygons style

  + support parameter setting from file data 'param("color",$2)'
    and use in plot for point customization

  + support mouse probe

  + chord diagram
  + . check value order and coloring
  + . interactive selection

  + custom key for style

  + single line/path for contours
  + vertical line probe on line plots

### Plots
 + force directed
 + stacked area
 + choropleth (use polygons)
 + hierarchical treemap, sunburst, bubble
