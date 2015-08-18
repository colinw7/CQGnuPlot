## airfoil.dem
  + No issues

## animate2.dem
  + No issues

## animate.dem
  + No issues

## approximate.dem
  + no issues

## arrowstyle.dem
  + should have no key on first few plots

## autoscale.dem
  + handle range limits

## barchart_art.dem
  + No issues

## binary.dem
  + Bad fill color (top/bottom sides)

## bivariat.dem
  + expr crash on user func (x>=-d*.1) ? 0 : (int1b(x+d,d)+(f(x+d)+4*f(x+d*.5)+f(x))*d/6.)
  + infinite loop

## bolditalic.dem
  + No issues

## borders.dem
  + key not needed

## boxplot.dem
  + candlesticks wrong :

## callargs.dem
  + No issues

## candlesticks.dem
  + No issues

## cerf.dem
  + No issues (cerf not supported)

## charset.dem
  + No issues

## circles.dem
  + bad pie chart labels and colors

## cities.dem
  + No issues

## colorscheme.dem
  + No issues

## colorwheel.dem
  + No issues

## complex_trig.dem
  + z axis should not be drawn

## contours.dem
  + last multiplot should overlay

## controls.dem
  + No issues

## dashcolor.dem
  + No issues

## dashtypes.dem
  + wrong color line 

## datastrings.dem
  + No issues

## dgrid3d.dem
  + splines, qnorm, gauss not working

## discrete.dem
  + No issues

## electron.dem
  + No issues

## ellipse.dem
  + ellipse points in wrong position

## ellipses_style.dem
  + No issues

## enhancedtext.dem
  + y axis not correct
  + power 3/2 not spaced properly
  + v overprint in wrong spot

## enhanced_utf8.dem
  + y axis not correct
  + power 3/2 not spaced properly
  + v overprint in wrong spot

## epslatex.dem
  + terminal not supported, device push/pop
  + Invalid arg 'standalone'

## fillbetween.dem
  + Invalid arg '"%lf'
  + Invalid arg 'filledcu'
  + runs ok, need to check output

## fillcvrs.dem
  + runs ok, need to check output

## fillstyle.dem
  + runs ok, need to check output

## finance.dem
  + Invalid format string
  + runs ok, need to check output

## fit.dem
  + Invalid expression ' "  fit function:", GPFUN_l'
  + bad 3d
  + crash in CExprPrintF

## fitmulti.dem
  + bad results

## fontfile.dem
  + postscript terminal not supported

## fontfile_latex.dem
  + postscript terminal not supported

## gantt.dem
  + Invalid command name 'A', ... (inline data not supported)
  + Eval failed: strptime("%m","2")

## heatmaps.dem
  + inline data not supported

## hidden2.dem
  + hidden not working

## hidden.dem
  + no issues

## histograms2.dem
  + bad range
  + Failed to eval lt for x=-10

## histograms.dem
  + Invalid Function 'columnheader'
  + bad range
  + Failed to eval lt for x=-10
  + Invalid arg 'rotatescale'
  + Invalid histogram style 'gap'

## hypertext.dem
  + some bad chars from file
  + Invalid arg 'hypertext', ...

## image2.dem
  + Unhandled binary plot
  + multiple bad plots

## image.dem
  + gaps between image pixels - need to draw to separate image bitmap and scale ?
  + 3d plot initial range is bad

## imageNaN.dem
  + Invalid option name 'tic'
  + Invalid arg '0'
  + Invalid command name 'Junk'

## iterate.dem
  + bad key

## kdensity2d.dem
  + bad display
  + Invalid Character "$random#$#random"

## key.dem
  + No issues

## layout.dem
  + bad bottom chart
  + Invalid arg 'margins'

## lines_arrows.dem
  + No issues

## linkedaxes.dem
  + Invalid option name 'ticslevel'
  + Invalid arg 'volatile', ...

## macros.dem
  + Missing plot style '@style1 title 'plot1', '1.dat' using 1:($2+1) with @style2 title 'plot2''

## margins.dem
  + Syntax Error >>*<<
  + maybe bad display

## mgr.dem
  + bad display
  + Invalid plot style 'xerr', 'yerr'

## molecule.dem
  + Bad display ?

## mouselab_1.dem
  + Eval failed:  LID + 1
  + sprintf(">>> READY FOR LABEL %d <<<",LID-100)

## mouselab_2.dem
  + Eval failed: (31 < MOUSE_KEY && MOUSE_KEY < 127)

## mouselabels.dem
  + Eval failed:  MOUSE_CHAR

## mousevariables.dem
  + Mouse click no handled

## multiaxis.dem
  + bad plot ?

## multimsh.dem
  + bad plot ?

## multipalette.dem
  + Bad display

## multiplt.dem
  + Invalid arg '-'
  + bad axes

## named_var.dem
  + bad plot

## nokey.dem
  + Invalid separator type
  + Invalid arg 'at' 'end'

## orbits.dem
  + Invalid arg '(cy($2,$3,$4,$5,$6))'

## parallel.dem
  + second plot bad range

## param.dem
  + bad plots
  + Invalid arg 'cycle'

## piecewise.dem
  + Failed to eval sample for x=-2, ...

## pm3dcolors.dem
  + Invalid arg '.'
  + Invalid arg 'rgb'
  + bad 3d plots

## pm3d.dem
  + Invalid option name 'ticslevel'
  + Invalid command name 'OK'

## pm3dgamma.dem
  + bad plots

## pointsize.dem
  + Invalid arg ',-1.2'

## polar.dem
  + check output
  + Invalid arg 'r=2.5'
  + Invalid arg 'notitle'

## poldat.dem
  + axis labels no spaced correctly
  + timestamp position on left plot edge
  + polar empty center circle

## prob2.dem
  + Eval failed:  1.1 * binom(floor((n+1)*p), n, p)
  + Eval failed: normal(mu, mu, sigma)

## prob.dem
  + Eval failed:  (p < 1.0 || q < 1.0) ? 2.0 : 1.4 * beta((p - 1.0)/(p + q - 2.0), p, q)

## rainbow.dem
  + Invalid line modifier 'rgb'
  + Invalid line modifier 'pal'

## random.dem
  + bad 3d plot
  + Invalid option name 'clabel'
  + Invalid option name 'ticslevel'

## rectangle.dem
  + no issues

## rgba_lines.dem
  + Invalid arg 'lc'
  + Invalid arg 'radius'

## rgbalpha.dem
  + Invalid key arg "sample"
  + Invalid key arg "1"

## rgb_variable.dem
  + Invalid arg 'lc'
  + Bad display

## rugplot.dem
  + Eval failed: (!exist("$random")

## running_avg.dem
  + Invalid arg '.'

## scatter.dem
  + some missing data on plots
  + crash on contour plot (array access)

## simple.dem
  + no issues

## singulr.dem
  + No issues

## smooth.dem
  + Invalid smooth type 'cumulative'

## spline.dem
  + no issues

## stats.dem
  + No support
  + Failed to eval A_mean for x=-10

## steps.dem
  + bad plots
  + Invalid Function 'norm'
  + Invalid Function 'inverf'

## stringvar.dem
  + bad expression foo[5:] (no support of empty or '*' in array indices)

## surface1.dem
  + Missing command name '$grid << EOD'

## surface2.dem
  + expression crash " n<=0 || abs(z)>100 ? 1:mand(z*z+a,a,n-1)+1"

## textcolor.dem
  + bad colors

## textrotate.dem
  + bad text rotate

## tics.dem
  + bad tics ?

## timedat.dem
  + bad display

## transparent.dem
  + not filled to axis

## transparent_solids.dem
  + no transparent

## using.dem
  + key margin formatting

## utf8.dem
  + looks ok

## varcolor.dem
  + box border needed
  + symbol center (triangle) not exact
  + border and fill color same for error bars ?

## vector.dem
  + key order
  + set view map disable Z axis ?
  + arrow size
  + no plot when table

## world2.dem
  + artifacts on colorbar
  + point/text not consistent (enhanced and non-enhaned)
  + no hidden removal

## world.dem
  + default margins wrong (2d and 3d)
