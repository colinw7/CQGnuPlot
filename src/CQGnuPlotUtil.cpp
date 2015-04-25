#include <CQGnuPlotUtil.h>
#include <CEnumConv.h>

namespace Conv {

typedef CEnumConv<CGnuPlotTypes::SymbolType, CQGnuPlot::SymbolType> SymbolConv;

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

typedef CEnumConv<CHAlignType, CQGnuPlot::HAlignType> HAlignConv;

HAlignConv halignConv({
 { CHALIGN_TYPE_LEFT  , CQGnuPlot::AlignLeft    },
 { CHALIGN_TYPE_RIGHT , CQGnuPlot::AlignRight   },
 { CHALIGN_TYPE_CENTER, CQGnuPlot::AlignHCenter }
});

typedef CEnumConv<CVAlignType, CQGnuPlot::VAlignType> VAlignConv;

VAlignConv valignConv({
 { CVALIGN_TYPE_BOTTOM, CQGnuPlot::AlignBottom  },
 { CVALIGN_TYPE_TOP   , CQGnuPlot::AlignTop     },
 { CVALIGN_TYPE_CENTER, CQGnuPlot::AlignVCenter }
});

typedef CEnumConv<CGnuPlot::PlotStyle, CQGnuPlot::PlotStyle> PlotStyleConv;

PlotStyleConv plotStyleConv({
  { CGnuPlot::PlotStyle::NONE          , CQGnuPlot::PlotNone           },
  { CGnuPlot::PlotStyle::BOXERRORBARS  , CQGnuPlot::PlotBoxErrorBars   },
  { CGnuPlot::PlotStyle::BOXES         , CQGnuPlot::PlotBoxes          },
  { CGnuPlot::PlotStyle::BOXPLOT       , CQGnuPlot::PlotBoxPlot        },
  { CGnuPlot::PlotStyle::BOXXYERRORBARS, CQGnuPlot::PlotBoxXYErrorBars },
  { CGnuPlot::PlotStyle::BUBBLEPLOT    , CQGnuPlot::PlotBubbleChart    },
  { CGnuPlot::PlotStyle::CANDLESTICKS  , CQGnuPlot::PlotCandlesticks   },
  { CGnuPlot::PlotStyle::CHORDDIAGRAM  , CQGnuPlot::PlotChordDiagram   },
  { CGnuPlot::PlotStyle::CIRCLES       , CQGnuPlot::PlotCircles        },
  { CGnuPlot::PlotStyle::DELAUNAY      , CQGnuPlot::PlotDelaunay       },
  { CGnuPlot::PlotStyle::DOTS          , CQGnuPlot::PlotDots           },
  { CGnuPlot::PlotStyle::ELLIPSES      , CQGnuPlot::PlotEllipses       },
  { CGnuPlot::PlotStyle::ERRORBARS     , CQGnuPlot::PlotErrorBars      },
  { CGnuPlot::PlotStyle::ERRORLINES    , CQGnuPlot::PlotErrorLines     },
  { CGnuPlot::PlotStyle::FILLEDCURVES  , CQGnuPlot::PlotFilledCurves   },
  { CGnuPlot::PlotStyle::FILLSTEPS     , CQGnuPlot::PlotFillSteps      },
  { CGnuPlot::PlotStyle::FINANCEBARS   , CQGnuPlot::PlotFinanceBars    },
  { CGnuPlot::PlotStyle::FSTEPS        , CQGnuPlot::PlotFSteps         },
  { CGnuPlot::PlotStyle::HISTEPS       , CQGnuPlot::PlotHiSteps        },
  { CGnuPlot::PlotStyle::HISTOGRAMS    , CQGnuPlot::PlotHistograms     },
  { CGnuPlot::PlotStyle::IMAGE         , CQGnuPlot::PlotImage          },
  { CGnuPlot::PlotStyle::IMPULSES      , CQGnuPlot::PlotImpulses       },
  { CGnuPlot::PlotStyle::LABELS        , CQGnuPlot::PlotLabels         },
  { CGnuPlot::PlotStyle::LINES         , CQGnuPlot::PlotLines          },
  { CGnuPlot::PlotStyle::LINES_POINTS  , CQGnuPlot::PlotLinesPoints    },
  { CGnuPlot::PlotStyle::PARALLELAXES  , CQGnuPlot::PlotParallelAxes   },
  { CGnuPlot::PlotStyle::PIECHART      , CQGnuPlot::PlotPieChart       },
  { CGnuPlot::PlotStyle::PM3D          , CQGnuPlot::PlotPm3d           },
  { CGnuPlot::PlotStyle::POINTS        , CQGnuPlot::PlotPoints         },
  { CGnuPlot::PlotStyle::POLYGONS      , CQGnuPlot::PlotPolygons       },
  { CGnuPlot::PlotStyle::RADAR         , CQGnuPlot::PlotRadar          },
  { CGnuPlot::PlotStyle::RGBALPHA      , CQGnuPlot::PlotRgbAlpha       },
  { CGnuPlot::PlotStyle::RGBIMAGE      , CQGnuPlot::PlotRgbImage       },
  { CGnuPlot::PlotStyle::STEPS         , CQGnuPlot::PlotSteps          },
  { CGnuPlot::PlotStyle::SUNBURST      , CQGnuPlot::PlotSunburst       },
  { CGnuPlot::PlotStyle::TREEMAP       , CQGnuPlot::PlotTreeMap        },
  { CGnuPlot::PlotStyle::VECTORS       , CQGnuPlot::PlotVectors        },
  { CGnuPlot::PlotStyle::XERRORBARS    , CQGnuPlot::PlotXErrorBars     },
  { CGnuPlot::PlotStyle::XERRORLINES   , CQGnuPlot::PlotXErrorLines    },
  { CGnuPlot::PlotStyle::XYERRORBARS   , CQGnuPlot::PlotXYErrorBars    },
  { CGnuPlot::PlotStyle::XYERRORLINES  , CQGnuPlot::PlotXYErrorLines   },
  { CGnuPlot::PlotStyle::YERRORBARS    , CQGnuPlot::PlotYErrorBars     },
  { CGnuPlot::PlotStyle::YERRORLINES   , CQGnuPlot::PlotYErrorLines    },
  { CGnuPlot::PlotStyle::TEST_TERMINAL , CQGnuPlot::PlotTestTerminal   },
  { CGnuPlot::PlotStyle::TEST_PALETTE  , CQGnuPlot::PlotTestPalette    },
});

typedef CEnumConv<CGnuPlot::HistogramStyle, CQGnuPlot::HistogramStyle> HistogramStyleConv;

HistogramStyleConv histogramStyleConv({
 { CGnuPlot::HistogramStyle::NONE         , CQGnuPlot::HistogramNone          },
 { CGnuPlot::HistogramStyle::CLUSTERED    , CQGnuPlot::HistogramClustered     },
 { CGnuPlot::HistogramStyle::ERRORBARS    , CQGnuPlot::HistogramErrorBars     },
 { CGnuPlot::HistogramStyle::ROWSTACKED   , CQGnuPlot::HistogramRowStacked    },
 { CGnuPlot::HistogramStyle::COLUMNSTACKED, CQGnuPlot::HistogramColumnStacked },
});

typedef CEnumConv<CGnuPlotTypes::FillType, CQGnuPlot::FillType> FillTypeConv;

FillTypeConv fillTypeConv({
  { CGnuPlotTypes::FillType::EMPTY  , CQGnuPlot::FillNone      },
  { CGnuPlotTypes::FillType::SOLID  , CQGnuPlot::FillSolid     },
  { CGnuPlotTypes::FillType::PATTERN, CQGnuPlot::FillPatterned },
});

typedef CEnumConv<CGnuPlotTypes::FillPattern, CQGnuPlot::FillPattern> FillPatternConv;

FillPatternConv fillPatternConv({
  { CGnuPlotTypes::FillPattern::NONE  , CQGnuPlot::PatternNone  },
  { CGnuPlotTypes::FillPattern::HATCH , CQGnuPlot::PatternHatch  },
  { CGnuPlotTypes::FillPattern::DENSE , CQGnuPlot::PatternDense  },
  { CGnuPlotTypes::FillPattern::FG    , CQGnuPlot::PatternFg     },
  { CGnuPlotTypes::FillPattern::FDIAG , CQGnuPlot::PatternFDiag  },
  { CGnuPlotTypes::FillPattern::BDIAG , CQGnuPlot::PatternBDiag  },
  { CGnuPlotTypes::FillPattern::FDIAG1, CQGnuPlot::PatternFDiag1 },
  { CGnuPlotTypes::FillPattern::BDIAG1, CQGnuPlot::PatternBDiag1 },
  { CGnuPlotTypes::FillPattern::BG    , CQGnuPlot::PatternBg     },
});

typedef CEnumConv<CGnuPlotTypes::FillPattern, Qt::BrushStyle> FillPatternQtConv;

FillPatternQtConv fillPatternQtConv({
  { CGnuPlotTypes::FillPattern::NONE  , Qt::NoBrush          },
  { CGnuPlotTypes::FillPattern::HATCH , Qt::DiagCrossPattern },
  { CGnuPlotTypes::FillPattern::DENSE , Qt::Dense2Pattern    },
  { CGnuPlotTypes::FillPattern::FG    , Qt::SolidPattern     },
  { CGnuPlotTypes::FillPattern::FDIAG , Qt::BDiagPattern     },
  { CGnuPlotTypes::FillPattern::BDIAG , Qt::FDiagPattern     },
  { CGnuPlotTypes::FillPattern::FDIAG1, Qt::HorPattern       },
  { CGnuPlotTypes::FillPattern::BDIAG1, Qt::VerPattern       },
  { CGnuPlotTypes::FillPattern::BG    , Qt::SolidPattern     },
});

typedef CEnumConv<CGnuPlotTypes::BoxWidthType, CQGnuPlot::BoxWidthType> BoxWidthTypeConv;

BoxWidthTypeConv boxWidthTypeConv({
  { CGnuPlotTypes::BoxWidthType::AUTO    , CQGnuPlot::BoxWidthAuto     },
  { CGnuPlotTypes::BoxWidthType::ABSOLUTE, CQGnuPlot::BoxWidthAbsolute },
  { CGnuPlotTypes::BoxWidthType::RELATIVE, CQGnuPlot::BoxWidthRelative },
});

typedef CEnumConv<CGnuPlotTypes::DrawLayer, CQGnuPlot::DrawLayerType> DrawLayerTypeConv;

DrawLayerTypeConv drawLayerTypeConv({
  { CGnuPlotTypes::DrawLayer::DEFAULT, CQGnuPlot::LayerDefault},
  { CGnuPlotTypes::DrawLayer::FRONT  , CQGnuPlot::LayerFront  },
  { CGnuPlotTypes::DrawLayer::BACK   , CQGnuPlot::LayerBack   },
  { CGnuPlotTypes::DrawLayer::BEHIND , CQGnuPlot::LayerBehind },
});

typedef CEnumConv<CGnuPlotPalette::ColorType, CQGnuPlot::PaletteColorType> PaletteColorTypeConv;

PaletteColorTypeConv paletteColorTypeConv({
  { CGnuPlotPalette::ColorType::MODEL  , CQGnuPlot::PaletteColorModel   },
  { CGnuPlotPalette::ColorType::DEFINED, CQGnuPlot::PaletteColorDefined },
});

typedef CEnumConv<CGnuPlotArrow::CoordType, CQGnuPlot::ArrowCoordType> ArrorColorTypeConv;

ArrorColorTypeConv arrowCoordTypeConv({
  { CGnuPlotArrow::CoordType::FROM_TO   , CQGnuPlot::ArrowCoordFromTo    },
  { CGnuPlotArrow::CoordType::FROM_RTO  , CQGnuPlot::ArrowCoordFromRTo   },
  { CGnuPlotArrow::CoordType::FROM_ANGLE, CQGnuPlot::ArrowCoordFromAngle },
});

}

//---

CQGnuPlot::SymbolType
CQGnuPlotUtil::
symbolConv(const CGnuPlotTypes::SymbolType &type)
{
  return Conv::symbolConv.conv(type);
}

CGnuPlotTypes::SymbolType
CQGnuPlotUtil::
symbolConv(const CQGnuPlot::SymbolType &type)
{
  return Conv::symbolConv.conv(type);
}

//---

CQGnuPlot::HAlignType
CQGnuPlotUtil::
alignConv(const CHAlignType &type)
{
  return Conv::halignConv.conv(type);
}

CHAlignType
CQGnuPlotUtil::
alignConv(const CQGnuPlot::HAlignType &type)
{
  return Conv::halignConv.conv(type);
}

//---

CQGnuPlot::VAlignType
CQGnuPlotUtil::
alignConv(const CVAlignType &type)
{
  return Conv::valignConv.conv(type);
}

CVAlignType
CQGnuPlotUtil::
alignConv(const CQGnuPlot::VAlignType &type)
{
  return Conv::valignConv.conv(type);
}

//---

CQGnuPlot::PlotStyle
CQGnuPlotUtil::
plotStyleConv(const CGnuPlot::PlotStyle &type)
{
  return Conv::plotStyleConv.conv(type, CQGnuPlot::PlotNone);
}

CGnuPlot::PlotStyle
CQGnuPlotUtil::
plotStyleConv(const CQGnuPlot::PlotStyle &type)
{
  return Conv::plotStyleConv.conv(type);
}

//---

CQGnuPlot::HistogramStyle
CQGnuPlotUtil::
histogramStyleConv(const CGnuPlot::HistogramStyle &type)
{
  return Conv::histogramStyleConv.conv(type);
}

CGnuPlot::HistogramStyle
CQGnuPlotUtil::
histogramStyleConv(const CQGnuPlot::HistogramStyle &type)
{
  return Conv::histogramStyleConv.conv(type);
}

//---

CQGnuPlot::FillType
CQGnuPlotUtil::
fillTypeConv(const CGnuPlotTypes::FillType &type)
{
  return Conv::fillTypeConv.conv(type);
}

CGnuPlotTypes::FillType
CQGnuPlotUtil::
fillTypeConv(const CQGnuPlot::FillType &type)
{
  return Conv::fillTypeConv.conv(type);
}

//---

CQGnuPlot::FillPattern
CQGnuPlotUtil::
fillPatternConv(const CGnuPlotTypes::FillPattern &type)
{
  return Conv::fillPatternConv.conv(type);
}

CGnuPlotTypes::FillPattern
CQGnuPlotUtil::
fillPatternConv(const CQGnuPlot::FillPattern &type)
{
  return Conv::fillPatternConv.conv(type);
}

//---

Qt::BrushStyle
CQGnuPlotUtil::
fillPatternQtConv(const CGnuPlotTypes::FillPattern &type)
{
  return Conv::fillPatternQtConv.conv(type);
}

CGnuPlotTypes::FillPattern
CQGnuPlotUtil::
fillPatternQtConv(const Qt::BrushStyle &type)
{
  return Conv::fillPatternQtConv.conv(type);
}

//---

CQGnuPlot::BoxWidthType
CQGnuPlotUtil::
boxWidthTypeConv(const CGnuPlotTypes::BoxWidthType &type)
{
  return Conv::boxWidthTypeConv.conv(type);
}

CGnuPlotTypes::BoxWidthType
CQGnuPlotUtil::
boxWidthTypeConv(const CQGnuPlot::BoxWidthType &type)
{
  return Conv::boxWidthTypeConv.conv(type);
}

//---

CQGnuPlot::DrawLayerType
CQGnuPlotUtil::
drawLayerTypeConv(const CGnuPlotTypes::DrawLayer &layer)
{
  return Conv::drawLayerTypeConv.conv(layer);
}

CGnuPlotTypes::DrawLayer
CQGnuPlotUtil::
drawLayerTypeConv(const CQGnuPlot::DrawLayerType &layer)
{
  return Conv::drawLayerTypeConv.conv(layer);
}

//---

CQGnuPlot::PaletteColorType
CQGnuPlotUtil::
paletteColorTypeConv(const CGnuPlotPalette::ColorType &layer)
{
  return Conv::paletteColorTypeConv.conv(layer);
}

CGnuPlotPalette::ColorType
CQGnuPlotUtil::
paletteColorTypeConv(const CQGnuPlot::PaletteColorType &layer)
{
  return Conv::paletteColorTypeConv.conv(layer);
}

//---

CQGnuPlot::ArrowCoordType
CQGnuPlotUtil::
arrowCoordTypeConv(const CGnuPlotArrow::CoordType  &type)
{
  return Conv::arrowCoordTypeConv.conv(type);
}

CGnuPlotArrow::CoordType
CQGnuPlotUtil::
arrowCoordTypeConv(const CQGnuPlot::ArrowCoordType &type)
{
  return Conv::arrowCoordTypeConv.conv(type);
}
