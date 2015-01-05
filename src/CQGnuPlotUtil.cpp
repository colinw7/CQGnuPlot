#include <CQGnuPlotUtil.h>
#include <CEnumConv.h>

namespace Conv {

typedef CEnumConv<CGnuPlotTypes::SymbolType, CQGnuPlot::CQSymbolType> SymbolConv;

SymbolConv symbolConv({
  { CGnuPlotTypes::SymbolType::POINT              , CQGnuPlot::SymbolPoint           },
  { CGnuPlotTypes::SymbolType::PLUS               , CQGnuPlot::SymbolPlus            },
  { CGnuPlotTypes::SymbolType::CROSS              , CQGnuPlot::SymbolCross           },
  { CGnuPlotTypes::SymbolType::STAR               , CQGnuPlot::SymbolStar            },
  { CGnuPlotTypes::SymbolType::BOX                , CQGnuPlot::SymbolBox             },
  { CGnuPlotTypes::SymbolType::FILLED_BOX         , CQGnuPlot::SymbolFilledBox       },
  { CGnuPlotTypes::SymbolType::CIRCLE             , CQGnuPlot::SymbolCircle          },
  { CGnuPlotTypes::SymbolType::FILLED_CIRCLE      , CQGnuPlot::SymbolFilledCircle    },
  { CGnuPlotTypes::SymbolType::TRIANGLE           , CQGnuPlot::SymbolTriangle        },
  { CGnuPlotTypes::SymbolType::FILLED_TRIANGLE    , CQGnuPlot::SymbolFilledTriangle  },
  { CGnuPlotTypes::SymbolType::INV_TRIANGLE       , CQGnuPlot::SymbolITriangle       },
  { CGnuPlotTypes::SymbolType::FILLED_INV_TRIANGLE, CQGnuPlot::SymbolFilledITriangle },
  { CGnuPlotTypes::SymbolType::DIAMOND            , CQGnuPlot::SymbolDiamond         },
  { CGnuPlotTypes::SymbolType::FILLED_DIAMOND     , CQGnuPlot::SymbolFilledDiamond   }
});

typedef CEnumConv<CHAlignType, CQGnuPlot::CQHAlignType> HAlignConv;

HAlignConv halignConv({
 { CHALIGN_TYPE_LEFT  , CQGnuPlot::AlignLeft    },
 { CHALIGN_TYPE_RIGHT , CQGnuPlot::AlignRight   },
 { CHALIGN_TYPE_CENTER, CQGnuPlot::AlignHCenter }
});

typedef CEnumConv<CVAlignType, CQGnuPlot::CQVAlignType> VAlignConv;

VAlignConv valignConv({
 { CVALIGN_TYPE_BOTTOM, CQGnuPlot::AlignBottom  },
 { CVALIGN_TYPE_TOP   , CQGnuPlot::AlignTop     },
 { CVALIGN_TYPE_CENTER, CQGnuPlot::AlignVCenter }
});

typedef CEnumConv<CGnuPlot::PlotStyle, CQGnuPlot::CQPlotStyle> PlotStyleConv;

PlotStyleConv plotStyleConv({
  { CGnuPlot::PlotStyle::NONE        , CQGnuPlot::PlotNone        },
  { CGnuPlot::PlotStyle::LINES       , CQGnuPlot::PlotLines       },
  { CGnuPlot::PlotStyle::POINTS      , CQGnuPlot::PlotPoints      },
  { CGnuPlot::PlotStyle::LINES_POINTS, CQGnuPlot::PlotLinesPoints },
  { CGnuPlot::PlotStyle::DOTS        , CQGnuPlot::PlotDots        },
  { CGnuPlot::PlotStyle::IMPULSES    , CQGnuPlot::PlotImpulses    },
  { CGnuPlot::PlotStyle::LABELS      , CQGnuPlot::PlotLabels      },
  { CGnuPlot::PlotStyle::STEPS       , CQGnuPlot::PlotSteps       },
  { CGnuPlot::PlotStyle::FSTEPS      , CQGnuPlot::PlotFSteps      },
  { CGnuPlot::PlotStyle::HISTEPS     , CQGnuPlot::PlotHiSteps     },
  { CGnuPlot::PlotStyle::HISTOGRAMS  , CQGnuPlot::PlotHistograms  },
  { CGnuPlot::PlotStyle::BOXES       , CQGnuPlot::PlotBoxes       },
  { CGnuPlot::PlotStyle::CANDLESTICKS, CQGnuPlot::PlotCandelSticks},
  { CGnuPlot::PlotStyle::FILLEDCURVES, CQGnuPlot::PlotFilledCurves},
});

typedef CEnumConv<CGnuPlot::HistogramStyle, CQGnuPlot::CQHistogramStyle> HistogramStyleConv;

HistogramStyleConv histogramStyleConv({
 { CGnuPlot::HistogramStyle::NONE         , CQGnuPlot::HistogramNone          },
 { CGnuPlot::HistogramStyle::CLUSTERED    , CQGnuPlot::HistogramClustered     },
 { CGnuPlot::HistogramStyle::ERRORBARS    , CQGnuPlot::HistogramErrorBars     },
 { CGnuPlot::HistogramStyle::ROWSTACKED   , CQGnuPlot::HistogramRowStacked    },
 { CGnuPlot::HistogramStyle::COLUMNSTACKED, CQGnuPlot::HistogramColumnStacked },
});

typedef CEnumConv<CGnuPlot::FillType, CQGnuPlot::CQFillType> FillTypeConv;

FillTypeConv fillTypeConv({
  { CGnuPlot::FillType::EMPTY  , CQGnuPlot::FillNone      },
  { CGnuPlot::FillType::SOLID  , CQGnuPlot::FillSolid     },
  { CGnuPlot::FillType::PATTERN, CQGnuPlot::FillPatterned },
});

typedef CEnumConv<CGnuPlot::FillPattern, CQGnuPlot::CQFillPattern> FillPatternConv;

FillPatternConv fillPatternConv({
  { CGnuPlot::FillPattern::NONE  , CQGnuPlot::PatternNone  },
  { CGnuPlot::FillPattern::HATCH , CQGnuPlot::PatternHatch  },
  { CGnuPlot::FillPattern::DENSE , CQGnuPlot::PatternDense  },
  { CGnuPlot::FillPattern::FG    , CQGnuPlot::PatternFg     },
  { CGnuPlot::FillPattern::FDIAG , CQGnuPlot::PatternFDiag  },
  { CGnuPlot::FillPattern::BDIAG , CQGnuPlot::PatternBDiag  },
  { CGnuPlot::FillPattern::FDIAG1, CQGnuPlot::PatternFDiag1 },
  { CGnuPlot::FillPattern::BDIAG1, CQGnuPlot::PatternBDiag1 },
  { CGnuPlot::FillPattern::BG    , CQGnuPlot::PatternBg     },
});

typedef CEnumConv<CGnuPlot::FillPattern, Qt::BrushStyle> FillPatternQtConv;

FillPatternQtConv fillPatternQtConv({
  { CGnuPlot::FillPattern::NONE  , Qt::NoBrush          },
  { CGnuPlot::FillPattern::HATCH , Qt::DiagCrossPattern },
  { CGnuPlot::FillPattern::DENSE , Qt::Dense2Pattern    },
  { CGnuPlot::FillPattern::FG    , Qt::SolidPattern     },
  { CGnuPlot::FillPattern::FDIAG , Qt::BDiagPattern     },
  { CGnuPlot::FillPattern::BDIAG , Qt::FDiagPattern     },
  { CGnuPlot::FillPattern::FDIAG1, Qt::HorPattern       },
  { CGnuPlot::FillPattern::BDIAG1, Qt::VerPattern       },
  { CGnuPlot::FillPattern::BG    , Qt::SolidPattern     },
});

typedef CEnumConv<CGnuPlot::BoxWidthType, CQGnuPlot::CQBoxWidthType> BoxWidthTypeConv;

BoxWidthTypeConv boxWidthTypeConv({
  { CGnuPlot::BoxWidthType::AUTO    , CQGnuPlot::BoxWidthAuto     },
  { CGnuPlot::BoxWidthType::ABSOLUTE, CQGnuPlot::BoxWidthAbsolute },
  { CGnuPlot::BoxWidthType::RELATIVE, CQGnuPlot::BoxWidthRelative },
});

}

//---

CQGnuPlot::CQSymbolType
CQGnuPlotUtil::
symbolConv(const CGnuPlotTypes::SymbolType &type)
{
  return Conv::symbolConv.conv(type);
}

CGnuPlotTypes::SymbolType
CQGnuPlotUtil::
symbolConv(const CQGnuPlot::CQSymbolType &type)
{
  return Conv::symbolConv.conv(type);
}

//---

CQGnuPlot::CQHAlignType
CQGnuPlotUtil::
alignConv(const CHAlignType &type)
{
  return Conv::halignConv.conv(type);
}

CHAlignType
CQGnuPlotUtil::
alignConv(const CQGnuPlot::CQHAlignType &type)
{
  return Conv::halignConv.conv(type);
}

//---

CQGnuPlot::CQVAlignType
CQGnuPlotUtil::
alignConv(const CVAlignType &type)
{
  return Conv::valignConv.conv(type);
}

CVAlignType
CQGnuPlotUtil::
alignConv(const CQGnuPlot::CQVAlignType &type)
{
  return Conv::valignConv.conv(type);
}

//---

CQGnuPlot::CQPlotStyle
CQGnuPlotUtil::
plotStyleConv(const CGnuPlot::PlotStyle &type)
{
  return Conv::plotStyleConv.conv(type, CQGnuPlot::PlotNone);
}

CGnuPlot::PlotStyle
CQGnuPlotUtil::
plotStyleConv(const CQGnuPlot::CQPlotStyle &type)
{
  return Conv::plotStyleConv.conv(type);
}

//---

CQGnuPlot::CQHistogramStyle
CQGnuPlotUtil::
histogramStyleConv(const CGnuPlot::HistogramStyle &type)
{
  return Conv::histogramStyleConv.conv(type);
}

CGnuPlot::HistogramStyle
CQGnuPlotUtil::
histogramStyleConv(const CQGnuPlot::CQHistogramStyle &type)
{
  return Conv::histogramStyleConv.conv(type);
}

//---

CQGnuPlot::CQFillType
CQGnuPlotUtil::
fillTypeConv(const CGnuPlot::FillType &type)
{
  return Conv::fillTypeConv.conv(type);
}

CGnuPlot::FillType
CQGnuPlotUtil::
fillTypeConv(const CQGnuPlot::CQFillType &type)
{
  return Conv::fillTypeConv.conv(type);
}

//---

CQGnuPlot::CQFillPattern
CQGnuPlotUtil::
fillPatternConv(const CGnuPlot::FillPattern &type)
{
  return Conv::fillPatternConv.conv(type);
}

CGnuPlot::FillPattern
CQGnuPlotUtil::
fillPatternConv(const CQGnuPlot::CQFillPattern &type)
{
  return Conv::fillPatternConv.conv(type);
}

//---

Qt::BrushStyle
CQGnuPlotUtil::
fillPatternQtConv(const CGnuPlot::FillPattern &type)
{
  return Conv::fillPatternQtConv.conv(type);
}

CGnuPlot::FillPattern
CQGnuPlotUtil::
fillPatternQtConv(const Qt::BrushStyle &type)
{
  return Conv::fillPatternQtConv.conv(type);
}

//---

CQGnuPlot::CQBoxWidthType
CQGnuPlotUtil::
boxWidthTypeConv(const CGnuPlot::BoxWidthType &type)
{
  return Conv::boxWidthTypeConv.conv(type);
}

CGnuPlot::BoxWidthType
CQGnuPlotUtil::
boxWidthTypeConv(const CQGnuPlot::CQBoxWidthType &type)
{
  return Conv::boxWidthTypeConv.conv(type);
}
