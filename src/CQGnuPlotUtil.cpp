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

typedef CEnumConv<CGnuPlotPlot::PlotStyle, CQGnuPlot::CQPlotStyle> PlotStyleConv;

PlotStyleConv plotStyleConv({
  { CGnuPlotPlot::PlotStyle::NONE        , CQGnuPlot::PlotNone        },
  { CGnuPlotPlot::PlotStyle::LINES       , CQGnuPlot::PlotLines       },
  { CGnuPlotPlot::PlotStyle::POINTS      , CQGnuPlot::PlotPoints      },
  { CGnuPlotPlot::PlotStyle::LINES_POINTS, CQGnuPlot::PlotLinesPoints },
  { CGnuPlotPlot::PlotStyle::DOTS        , CQGnuPlot::PlotDots        },
  { CGnuPlotPlot::PlotStyle::LABELS      , CQGnuPlot::PlotLabels      },
  { CGnuPlotPlot::PlotStyle::HISTEPS     , CQGnuPlot::PlotHiSteps     },
  { CGnuPlotPlot::PlotStyle::STEPS       , CQGnuPlot::PlotSteps       },
  { CGnuPlotPlot::PlotStyle::FSTEPS      , CQGnuPlot::PlotFSteps      },
  { CGnuPlotPlot::PlotStyle::HISTOGRAMS  , CQGnuPlot::PlotHistograms  },
  { CGnuPlotPlot::PlotStyle::BOXES       , CQGnuPlot::PlotBoxes       },
  { CGnuPlotPlot::PlotStyle::IMPULSES    , CQGnuPlot::PlotImpulses    },
});

typedef CEnumConv<CGnuPlotPlot::HistogramStyle, CQGnuPlot::CQHistogramStyle> HistogramStyleConv;

HistogramStyleConv histogramStyleConv({
 { CGnuPlotPlot::HistogramStyle::NONE         , CQGnuPlot::HistogramNone          },
 { CGnuPlotPlot::HistogramStyle::CLUSTERED    , CQGnuPlot::HistogramClustered     },
 { CGnuPlotPlot::HistogramStyle::ERRORBARS    , CQGnuPlot::HistogramErrorBars     },
 { CGnuPlotPlot::HistogramStyle::ROWSTACKED   , CQGnuPlot::HistogramRowStacked    },
 { CGnuPlotPlot::HistogramStyle::COLUMNSTACKED, CQGnuPlot::HistogramColumnStacked },
});

typedef CEnumConv<CGnuPlotPlot::FillType, CQGnuPlot::CQFillType> FillTypeConv;

FillTypeConv fillTypeConv({
  { CGnuPlotPlot::FillType::EMPTY  , CQGnuPlot::FillNone      },
  { CGnuPlotPlot::FillType::SOLID  , CQGnuPlot::FillSolid     },
  { CGnuPlotPlot::FillType::PATTERN, CQGnuPlot::FillPatterned },
});

typedef CEnumConv<CGnuPlotPlot::FillPattern, CQGnuPlot::CQFillPattern> FillPatternConv;

FillPatternConv fillPatternConv({
  { CGnuPlotPlot::FillPattern::NONE  , CQGnuPlot::PatternNone  },
  { CGnuPlotPlot::FillPattern::HATCH , CQGnuPlot::PatternHatch  },
  { CGnuPlotPlot::FillPattern::DENSE , CQGnuPlot::PatternDense  },
  { CGnuPlotPlot::FillPattern::FG    , CQGnuPlot::PatternFg     },
  { CGnuPlotPlot::FillPattern::FDIAG , CQGnuPlot::PatternFDiag  },
  { CGnuPlotPlot::FillPattern::BDIAG , CQGnuPlot::PatternBDiag  },
  { CGnuPlotPlot::FillPattern::FDIAG1, CQGnuPlot::PatternFDiag1 },
  { CGnuPlotPlot::FillPattern::BDIAG1, CQGnuPlot::PatternBDiag1 },
  { CGnuPlotPlot::FillPattern::BG    , CQGnuPlot::PatternBg     },
});

typedef CEnumConv<CGnuPlot::FillPattern, Qt::BrushStyle> FillPatternQtConv;

FillPatternQtConv fillPatternQtConv({
  { CGnuPlotPlot::FillPattern::NONE  , Qt::NoBrush          },
  { CGnuPlotPlot::FillPattern::HATCH , Qt::DiagCrossPattern },
  { CGnuPlotPlot::FillPattern::DENSE , Qt::Dense2Pattern    },
  { CGnuPlotPlot::FillPattern::FG    , Qt::SolidPattern     },
  { CGnuPlotPlot::FillPattern::FDIAG , Qt::BDiagPattern     },
  { CGnuPlotPlot::FillPattern::BDIAG , Qt::FDiagPattern     },
  { CGnuPlotPlot::FillPattern::FDIAG1, Qt::HorPattern       },
  { CGnuPlotPlot::FillPattern::BDIAG1, Qt::VerPattern       },
  { CGnuPlotPlot::FillPattern::BG    , Qt::SolidPattern     },
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
plotStyleConv(const CGnuPlotPlot::PlotStyle &type)
{
  return Conv::plotStyleConv.conv(type);
}

CGnuPlotPlot::PlotStyle
CQGnuPlotUtil::
plotStyleConv(const CQGnuPlot::CQPlotStyle &type)
{
  return Conv::plotStyleConv.conv(type);
}

//---

CQGnuPlot::CQHistogramStyle
CQGnuPlotUtil::
histogramStyleConv(const CGnuPlotPlot::HistogramStyle &type)
{
  return Conv::histogramStyleConv.conv(type);
}

CGnuPlotPlot::HistogramStyle
CQGnuPlotUtil::
histogramStyleConv(const CQGnuPlot::CQHistogramStyle &type)
{
  return Conv::histogramStyleConv.conv(type);
}

//---

CQGnuPlot::CQFillType
CQGnuPlotUtil::
fillTypeConv(const CGnuPlotPlot::FillType &type)
{
  return Conv::fillTypeConv.conv(type);
}

CGnuPlotPlot::FillType
CQGnuPlotUtil::
fillTypeConv(const CQGnuPlot::CQFillType &type)
{
  return Conv::fillTypeConv.conv(type);
}

//---

CQGnuPlot::CQFillPattern
CQGnuPlotUtil::
fillPatternConv(const CGnuPlotPlot::FillPattern &type)
{
  return Conv::fillPatternConv.conv(type);
}

CGnuPlotPlot::FillPattern
CQGnuPlotUtil::
fillPatternConv(const CQGnuPlot::CQFillPattern &type)
{
  return Conv::fillPatternConv.conv(type);
}

//---

Qt::BrushStyle
CQGnuPlotUtil::
fillPatternQtConv(const CGnuPlotPlot::FillPattern &type)
{
  return Conv::fillPatternQtConv.conv(type);
}

CGnuPlotPlot::FillPattern
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
