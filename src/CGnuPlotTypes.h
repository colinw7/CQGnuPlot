#ifndef CGnuPlotTypes_H
#define CGnuPlotTypes_H

namespace CGnuPlotTypes {
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

  enum class ChangeState {
    PLOT_ADDED,
    AXIS_DISPLAY,
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

  enum class BoxWidthType {
    AUTO,
    ABSOLUTE,
    RELATIVE
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

  enum class HistogramStyle {
    NONE,
    CLUSTERED,
    ERRORBARS,
    ROWSTACKED,
    COLUMNSTACKED
  };

  enum class LogScale {
    X,
    Y,
    Z,
    X2,
    Y2,
    CB
  };
}

#endif
