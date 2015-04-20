#ifndef CGnuPlotTypes_H
#define CGnuPlotTypes_H

#include <string>
#include <vector>

typedef std::vector<std::string> StringArray;

namespace CGnuPlotTypes {
  enum class CommandName {
    NONE,
    BIND,
    CALL,
    CD,
    CLEAR,
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
    WHILE
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
    ARRAY,
    ARROWSTYLE,
    AXES,
    BINARY,
    CENTER,
    DASHTYPE,
    DX,
    DY,
    DZ,
    ENDIAN,
    EVERY,
    FILETYPE,
    FILLSTYLE,
    FLIP,
    FLIPX,
    FLIPY,
    FLIPZ,
    FORMAT,
    HEADS,
    INDEX,
    LINETYPE,
    LINEWIDTH,
    MATRIX,
    NOTITLE,
    ORIGIN,
    POINTSIZE,
    ROTATION,
    SCAN,
    SKIP,
    SMOOTH,
    TITLE,
    TRANSPOSE,
    USING,
    WHISKERBARS,
    WITH
  };

  enum class PlotStyle {
    NONE,
    BOXERRORBARS,
    BOXES,
    BOXPLOT,
    BOXXYERRORBARS,
    BUBBLEPLOT,
    CANDLESTICKS,
    CIRCLES,
    DOTS,
    ELLIPSES,
    ERRORBARS,
    ERRORLINES,
    FILLEDCURVES,
    FILLSTEPS,
    FINANCEBARS,
    FSTEPS,
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
    RADAR,
    RGBALPHA,
    RGBIMAGE,
    STEPS,
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
    BEZIER,
    SBEZIER,
    CSPLINES,
    ACSPLINES
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

  enum class ColorModel {
    RGB,
    HSV,
    CMY,
    YIQ,
    XYZ,
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
    NONE,
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
    LAST=FILLED_DIAMOND
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

  enum class LogScale {
    X,
    Y,
    Z,
    X2,
    Y2,
    CB
  };

  enum class Mapping {
    CARTESIAN_MAPPING,
    CYLINDRICAL_MAPPING,
    SPHERICAL_MAPPING
  };
}

#endif
