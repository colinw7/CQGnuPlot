## airfoil.dem
  + No issues

## animate2.dem
  + No animation : recursive script - doesn't seem to work

## animate.dem
  + No animation : recursive script - doesn't seem to work

## approximate.dem
  + no issues

## arrowstyle.dem
  + should have no key on first few plots

## autoscale.dem
  + symbol should be plus
  + handle range limits

## barchart_art.dem
  + No issues

## binary.dem
  + Bad view/axis
  + Bad fill color (top/bottom sides)
  + Missing text on key

## bivariat.dem
  + expr crash on user func (x>=-d*.1) ? 0 : (int1b(x+d,d)+(f(x+d)+4*f(x+d*.5)+f(x))*d/6.)
  + infinite loop

## bolditalic.dem
  + improve polar layout (axis labels and title)

## borders.dem
  + key not needed

## boxplot.dem
  + Invalid arg 'auto'
  + eval failed - Missing close round bracket

## callargs.dem
  + No issues

## candlesticks.dem
  + No issues

## cerf.dem
  + Eval failed: (!strstrt(GPVAL_COMPILE_OPTIONS,"+LIBCERF"))

## charset.dem
  + bad enhanced text rendering

## circles.dem
  + Invalid arg 'arc'
  + Invalid arg 'func', Invalid plot var 'solid'
  + ... more invalid args
  + Unhandled binary plot

## cities.dem
  + No issues

## colorscheme.dem
  + No issues

## colorwheel.dem
  + No issues

## complex_trig.dem
  + real parse issues -pi/2
  + Invalid Function 'hsv2rgb'

## contours.dem
  + Invalid plot var 'boxed'
  + Invalid plot var 'nosurf'
  + Missing plot var '/ (x**2 + y**2 + 0.1)

## controls.dem
  + Crash 15->Enum conversion

## dashcolor.dem
  + Invalid line modifier 'dt'
  + Invalid arg 'title' ...

## dashtypes.dem
  + Invalid arg 'new', Invalid arg '+(i-1)%5, no dash type support yet

## datastrings.dem
  + Invalid Function 'xticlabel'
  + Invalid arg 'offset', 'boxed', 'notitle'

## dgrid3d.dem
  + no data on plot

## discrete.dem
  + bad data

## electron.dem
  + Invalid format string
  + bad log axis
  + Unhandled autoscale y2

## ellipse.dem
  + Missing command name '}'
  + Invalid arg 'front', Invalid Character "$random#$#random

## ellipses_style.dem
  + runs, need check output
  + Invalid arg 'units', Invalid arg 'xx', ...

## enhancedtext.dem
  + runs, need check output

## enhanced_utf8.dem
  + bad display

## epslatex.dem
  + terminal not supported, device push/pop
  + Invalid arg 'standalone'

## fillbetween.dem
  + Invalid arg '"%lf'
  + Invalid arg 'filledcu'
  + runs ok, need to check output

## fillcrvs.dem
  + runs ok, need to check output
  + Invalid arg 'solid'
  + Invalid arg '1.0'
  + Invalid arg 'lc'
  + Invalid arg 'rgb'
  + Invalid arg ''dark-goldenrod'

## fillstyle.dem
  + runs ok, need to check output

## finance.dem
  + runs ok, need to check output

## fit.dem
  + Invalid expression ' "  fit function:", GPFUN_l'
  + bad 3d
  + crash in CExprPrintF

## fitmulti.dem
  + Invalid filename

## fontfile.dem
  + postscript terminal not supported

## fontfile_latex.dem
  + postscript terminal not supported

## gantt.dem
  + Invalid command name 'A', ... (inline data not supported)
  + Eval failed: strptime("%m","2")

## heatmaps.dem
  + Invalid option name 'tic'
  + Invalid arg 'rgbformula'
  + inline data not supported

## hidden2.dem
  + bad 3d plot

## hidden.dem
  + Missing plot var '/ (x*x + y*y + 1)'
  + Invalid arg '-'
  + some bad plots at end

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
  + Invalid command name 'Junk

## iterate.dem
  + Invalid arg '/2'
  + runs ok, bad output

## kdensity2d.dem
  + bad display
  + Invalid Character "$random#$#random

## key.dem
  + No issues

## layout.dem
  + bad bottom chart
  + Invalid arg 'margins'

## lines_arrows.dem
  + Invalid line modifier 'dt'
  + Invalid arg 'title', 'ls'

## linkedaxes.dem
  + Invalid option name 'ticslevel
  + Invalid arg 'volatile', ...

## macros.dem
  + Missing plot style '@style1 title 'plot1', '1.dat' using 1:($2+1) with @style2 title 'plot2''

## margins.dem
  + maybe bad display

## mgr.dem
  + bad display
  + Invalid plot style 'xerr', 'yerr'

## molecule.dem
  + Display off screen

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
  + Unhandled autoscale y2

## multimsh.dem
  + Unhandled autoscale z

## multipalette.dem
  + Bad display

## multiplt.dem
  + Invalid arg '-'
  + Crash on Qt conv

## named_var.dem
  + bad plot

## nokey.dem
  + Invalid separator type

## orbits.dem
  + Invalid Character "$3#$#3
  + symbol qt conv

## parallel.dem
  + second plot bad range

## param.dem
  + bad plots
  + Invalid arg 'cycle'

## piecewise.dem
  + Failed to eval sample for x=-2

## pm3dcolors.dem
  + Invalid arg '.'
  + Invalid arg 'rgb'

## pm3d.dem
  + Invalid option name 'ticslevel'
  + Invalid command name 'OK'

## pm3dgamma.dem
  + bad plots

## pointsize.dem
  + Invalid option name 'ticslevel'
  + Invalid arg ',-1.2'

## polar.dem
  + check output

## poldat.dem
  + Invalid option name 'noxtics'
  + Invalid arg 'title'
  + Invalid arg ''silver.dat''

## prob2.dem
  + Eval failed:  1.1 * binom(floor((n+1)*p), n, p)
  + Eval failed: normal(mu, mu, sigma)

## prob.dem
  + Eval failed:  (p < 1.0 || q < 1.0) ? 2.0 : 1.4 * beta((p - 1.0)/(p + q - 2.0), p, q)

## rainbow.dem
  + Invalid line modifier 'rgb'
  + Invalid arg 'title'
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

## simple.dem
  + no issues

## singulr.dem
  + some missing data on plots

## smooth.dem
  + Invalid smooth type 'cumulative'

## spline.dem
  + no issues

## stats.dem
  + Failed to eval A_mean for x=-10

## steps.dem
  + bad plots

## stringvar.dem
  + bad expression foo[5:]

## surface1.dem
  + Missing command name '$grid << EOD'

## surface2.dem
  + expression crash " n<=0 || abs(z)>100 ? 1:mand(z*z+a,a,n-1)+1"

## textcolor.dem
  + bad colors

## textrotate.dem
  + bad text rotate

## tics.dem
  + Invalid arg ','

## timedat.dem
  + bad axis format
  + Eval failed: 7"

## transparent.dem
  + Invalid arg 'solid'
  + Invalid arg 'σ'

## transparent_solids.dem
  + Invalid arg ','
  + bad plot

## using.dem
  + no issues

## utf8.dem
  + bad text

## varcolor.dem
  + Invalid color string
  + Invalid arg 'var'

## vector.dem
  + Invalid filename
  + "$equipo2#$#equipo2

## world2.dem
  + Invalid plot var 'pt'

## world.dem
  + Invalid arg 'geographic'
  + bad label format
  + Invalid option name 'ticslevel'
