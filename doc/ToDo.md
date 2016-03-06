## ToDo

  + animation (fade in/out)
  + per style data/properties
  + less reload on data change

  + smooth bezier, sbezier, csplines, acsplines
  + support probe in more styles
  + better clipping support (objects)
  + support zoom/pan using scale and offset properties to modify
    window->pixel calc (do not change range)
  + CGnuPlotFile store reals as well as strings
  + Use model data to drive plot
  + data table view
  + Polygons modify all with property
  + Generate multiple (>2) columns
  + Info on table (sets, subsets, values)
  + bind shortcuts

  + isosamples/samples logic (generate using isosamples and supersample at samples ?)

  + Better axes ticks for log (match base powers)
  + allow select/modifiy all objects (plots, axis, ...)
  + edit interface (move) for annotations
  + save cmd
  + user shared_ptr/unique_ptr instead of CRefPtr
  + more color schemes (selectable)
  + do not recalc bar/bubble/pie on plot
  + consistent handling of position front,back,behind
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

  + custom key for style

  + single line/path for contours
  + vertical line probe on line plots
