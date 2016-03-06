#ifndef CGnuPlotTypes_H
#define CGnuPlotTypes_H

#include <string>
#include <vector>
#include <COptVal.h>
#include <CRGBA.h>
#include <CPoint2D.h>
#include <CPoint3D.h>
#include <CBBox2D.h>
#include <CBBox3D.h>
#include <CLineDash.h>
#include <CAlignType.h>

typedef std::vector<std::string> StringArray;
typedef COptValT<CRGBA>          COptRGBA;
typedef COptValT<CPoint2D>       COptPoint2D;
typedef COptValT<CPoint3D>       COptPoint3D;
typedef COptValT<CBBox2D>        COptBBox2D;
typedef COptValT<CLineDash>      COptLineDash;

namespace CGnuPlotTypes {
  enum class CommandName {
    NONE,
    BIND,
    CALL,
    CD,
    CLEAR,
    DATA,
    DO,
    EVALUATE,
    EXIT,
    FIT,
    GET,
    HELP,
    HISTORY,
    IF,
    LOAD,
    LOWER,
    PAUSE,
    PLOT,
    PRINT,
    PRINTF,
    PWD,
    QUIT,
    RAISE,
    REPLOT,
    REFRESH,
    REREAD,
    RESET,
    SAVE,
    SET,
    SHELL,
    SHOW,
    SPLOT,
    STATS,
    SYSTEM,
    TEST,
    UNDEFINE,
    UNSET,
    UPDATE,
    WHILE,
    WINDOW
  };

  enum class VariableName {
    NONE,

    ANGLES,
    ARROW,
    AUTOSCALE,
    BARS,
    BIND,
    BMARGIN,
    BORDER,
    BOXWIDTH,
    CBDATA,
    CBDTICS,
    CBLABEL,
    CBMTICS,
    CBRANGE,
    CBTICS,
    CIRCLE,
    CLIP,
    CNTRLABEL,
    CNTRPARAM,
    COLORBOX,
    COLORNAMES,
    COLORSEQUENCE,
    CONTOUR,
    CPLANE,
    DASHTYPE,
    DATABLOCK,
    DATAFILE,
    DEBUG,
    DECIMALSIGN,
    DGRID3D,
    DUMMY,
    EDEBUG,
    ELLIPSE,
    ENCODING,
    FIT,
    FONTPATH,
    FORMAT,
    FUNCTIONS,
    GRID,
    HIDDEN3D,
    HISTORY,
    HISTORYSIZE,
    ISOSAMPLES,
    KEY,
    LABEL,
    LINETYPE,
    LINK,
    LMARGIN,
    LOADPATH,
    LOCALE,
    LOGSCALE,
    MACROS,
    MAPPING,
    MARGINS,
    MOUSE,
    MULTIPLOT,
    MX2TICS,
    MXTICS,
    MY2TICS,
    MYTICS,
    MZTICS,
    NOX2TICS,
    NOXTICS,
    NOY2TICS,
    NOYTICS,
    OBJECT,
    OFFSETS,
    ORIGIN,
    OUTPUT,
    PALETTE,
    PARAMETRIC,
    PAXIS,
    PLOT,
    PM3D,
    POINTINTERVALBOX,
    POINTSIZE,
    POLAR,
    POLYGON,
    PRINT,
    PSDIR,
    RAXIS,
    RECTANGLE,
    RMARGIN,
    RRANGE,
    RTICS,
    SAMPLES,
    SIZE,
    STYLE,
    SURFACE,
    TABLE,
    TERMINAL,
    TERMOPTION,
    TICS,
    TICSCALE,
    TICSLEVEL,
    TIMEFMT,
    TIMESTAMP,
    TITLE,
    TMARGIN,
    TRANGE,
    URANGE,
    VARIABLES,
    VERSION,
    VIEW,
    VRANGE,
    X2DATA,
    X2DTICS,
    X2LABEL,
    X2MTICS,
    X2RANGE,
    X2TICS,
    X2ZEROAXIS,
    XDATA,
    XDTICS,
    XLABEL,
    XMTICS,
    XRANGE,
    XTICS,
    XYPLANE,
    XZEROAXIS,
    Y2DATA,
    Y2DTICS,
    Y2LABEL,
    Y2MTICS,
    Y2RANGE,
    Y2TICS,
    Y2ZEROAXIS,
    YDATA,
    YDTICS,
    YLABEL,
    YMTICS,
    YRANGE,
    YTICS,
    YZEROAXIS,
    ZDATA,
    ZDTICS,
    ZERO,
    ZEROAXIS,
    ZLABEL,
    ZMTICS,
    ZRANGE,
    ZTICS,
    ZZEROAXIS,
  };

  enum class DataFileVar {
    NONE,
    FORTRAN,
    NOFPE_TRAP,
    MISSING,
    SEPARATOR,
    COMMENTS_CHARS,
    BINARY,
    CSV,
  };

  enum class StyleVar {
    NONE,
    ARROW,
    BOXPLOT,
    CIRCLE,
    DATA,
    ELLIPSE,
    FILL,
    FUNCTION,
    HISTOGRAM,
    INCREMENT,
    LINE,
    RECTANGLE,
    TEXTBOX
  };

  enum class PlotVar {
    ARROWSTYLE,
    AXES,
    BINARY,
    COLUMNHEADERS,
    DASHTYPE,
    EVERY,
    FILLSTYLE,
    FLIPX,
    FLIPY,
    FLIPZ,
    HEADS,
    INDEX,
    LINETYPE,
    LINEWIDTH,
    MATRIX,
    NOTITLE,
    PIXELS,
    POINTINTERVAL,
    POINTSIZE,
    ROWHEADERS,
    SCAN,
    SMOOTH,
    TITLE,
    TRANSPOSE,
    USING,
    VOLATILE,
    WHISKERBARS,
    WITH
  };

  enum class PlotStyle {
    NONE,
    ADJACENCY,
    BIVARIATE,
    BOXERRORBARS,
    BOXES,
    BOXPLOT,
    BOXXYERRORBARS,
    BUBBLEPLOT,
    CANDLESTICKS,
    CHORDDIAGRAM,
    CIRCLES,
    DELAUNAY,
    DENDROGRAM,
    DOTS,
    ELLIPSES,
    ERRORBARS,
    ERRORLINES,
    FILLEDCURVES,
    FILLSTEPS,
    FINANCEBARS,
    FORCEDIRECTED,
    FSTEPS,
    HIERBUBBLEPLOT,
    HIERTREEMAP,
    HISTEPS,
    HISTOGRAMS,
    IMAGE,
    IMPULSES,
    LABELS,
    LINES,
    LINES_POINTS,
    PARALLELAXES,
    PIECHART,
    PM3D,
    POINTS,
    POLYGONS,
    RADAR,
    RGBALPHA,
    RGBIMAGE,
    STACKEDAREA,
    STEPS,
    SURFACE,
    SUNBURST,
    TREEMAP,
    VECTORS,
    XERRORBARS,
    XERRORLINES,
    XYERRORBARS,
    XYERRORLINES,
    YERRORBARS,
    YERRORLINES,
    TEST_TERMINAL,
    TEST_PALETTE
  };

  enum class Smooth {
    NONE,
    UNIQUE,
    FREQUENCY,
    CUMULATIVE,
    CNORMAL,
    KDENSITY,
    CSPLINES,
    ACSPLINES,
    BEZIER,
    SBEZIER
  };

  enum class FillType {
    EMPTY,
    SOLID,
    PATTERN
  };

  enum class FillPattern {
    NONE,
    HATCH,
    DENSE,
    FG,
    FDIAG,
    BDIAG,
    FDIAG1,
    BDIAG1,
    BG,
    LAST=BG
  };

  enum class HistogramStyle {
    NONE,
    CLUSTERED,
    ERRORBARS,
    ROWSTACKED,
    COLUMNSTACKED
  };

  enum class AngleType {
    RADIANS,
    DEGREES
  };

  enum class CoordSys {
    FIRST,
    SECOND,
    GRAPH,
    SCREEN,
    CHARACTER
  };

  enum class ObjectType {
    NONE,
    ARROW,
    CIRCLE,
    ELLIPSE,
    LABEL,
    POLYGON,
    RECTANGLE
  };

  enum class DrawLayer {
    DEFAULT,
    FRONT,
    BACK,
    BEHIND
  };

  enum class ImageType {
    NONE,
    JPEG,
    PNG,
    GIF
  };

  enum class StyleIncrementType {
    USER,
    DEFAULT
  };

  enum class TestType {
    NONE,
    TERMINAL,
    PALETTE
  };

  enum class SymbolType {
    NONE=-1,
    POINT,
    PLUS,
    CROSS,
    STAR,
    BOX,
    FILLED_BOX,
    CIRCLE,
    FILLED_CIRCLE,
    TRIANGLE,
    FILLED_TRIANGLE,
    INV_TRIANGLE,
    FILLED_INV_TRIANGLE,
    DIAMOND,
    FILLED_DIAMOND,
    PENTAGON,
    FILLED_PENTAGON,
    LAST=FILLED_PENTAGON,
    BIVARIATE=998,
    STRING=999
  };

  enum class ChangeState {
    PLOT_ADDED,
    AXIS_DISPLAY,
  };

  enum class BoxWidthType {
    AUTO,
    ABSOLUTE,
    RELATIVE
  };

  enum class AxisType {
    X,
    Y,
    Z,
    R,
    P,
    T,
    U,
    V,
    CB
  };

  enum class AxisDirection {
    X,
    Y,
    Z
  };

  enum class Mapping {
    CARTESIAN_MAPPING,
    CYLINDRICAL_MAPPING,
    SPHERICAL_MAPPING
  };

  enum class EllipseUnits {
    X_ = (1<<0),
    Y_ = (1<<1),
    _X = (X_<<8),
    _Y = (Y_<<8),

    XX = (X_|_X),
    XY = (X_|_Y),
    YY = (Y_|_Y)
  };

  typedef std::pair<AxisType, int> AxisTypeId;

  enum class PauseMask {
    KEY     = (1<<0),
    BUTTON1 = (1<<1),
    BUTTON2 = (1<<2),
    BUTTON3 = (1<<3),
    CLOSE   = (1<<4)
  };

  enum class Endian {
    LITTLE,
    BIG,
    DEFAULT,
    SWAP
  };

  enum class BoxLabels {
    Off,
    Auto,
    X,
    X2
  };

  enum class BoxType {
    CandleSticks,
    FinanceBars
  };

  typedef std::pair<CHAlignType,double> HAlignPos;
  typedef std::pair<CVAlignType,double> VAlignPos;
}

#endif
