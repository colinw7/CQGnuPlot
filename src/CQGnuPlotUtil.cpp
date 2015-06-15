#include <CQGnuPlotUtil.h>
#include <CEnumConv.h>

namespace Conv {

typedef CEnumConv<CGnuPlotTypes::SymbolType, CQGnuPlotEnum::SymbolType> SymbolConv;

SymbolConv symbolConv({
  { CGnuPlotTypes::SymbolType::NONE               , CQGnuPlotEnum::SymbolNone            },
  { CGnuPlotTypes::SymbolType::POINT              , CQGnuPlotEnum::SymbolPoint           },
  { CGnuPlotTypes::SymbolType::PLUS               , CQGnuPlotEnum::SymbolPlus            },
  { CGnuPlotTypes::SymbolType::CROSS              , CQGnuPlotEnum::SymbolCross           },
  { CGnuPlotTypes::SymbolType::STAR               , CQGnuPlotEnum::SymbolStar            },
  { CGnuPlotTypes::SymbolType::BOX                , CQGnuPlotEnum::SymbolBox             },
  { CGnuPlotTypes::SymbolType::FILLED_BOX         , CQGnuPlotEnum::SymbolFilledBox       },
  { CGnuPlotTypes::SymbolType::CIRCLE             , CQGnuPlotEnum::SymbolCircle          },
  { CGnuPlotTypes::SymbolType::FILLED_CIRCLE      , CQGnuPlotEnum::SymbolFilledCircle    },
  { CGnuPlotTypes::SymbolType::TRIANGLE           , CQGnuPlotEnum::SymbolTriangle        },
  { CGnuPlotTypes::SymbolType::FILLED_TRIANGLE    , CQGnuPlotEnum::SymbolFilledTriangle  },
  { CGnuPlotTypes::SymbolType::INV_TRIANGLE       , CQGnuPlotEnum::SymbolITriangle       },
  { CGnuPlotTypes::SymbolType::FILLED_INV_TRIANGLE, CQGnuPlotEnum::SymbolFilledITriangle },
  { CGnuPlotTypes::SymbolType::DIAMOND            , CQGnuPlotEnum::SymbolDiamond         },
  { CGnuPlotTypes::SymbolType::FILLED_DIAMOND     , CQGnuPlotEnum::SymbolFilledDiamond   }
});

typedef CEnumConv<CHAlignType, CQGnuPlotEnum::HAlignType> HAlignConv;

HAlignConv halignConv({
 { CHALIGN_TYPE_LEFT  , CQGnuPlotEnum::AlignLeft    },
 { CHALIGN_TYPE_RIGHT , CQGnuPlotEnum::AlignRight   },
 { CHALIGN_TYPE_CENTER, CQGnuPlotEnum::AlignHCenter }
});

typedef CEnumConv<CVAlignType, CQGnuPlotEnum::VAlignType> VAlignConv;

VAlignConv valignConv({
 { CVALIGN_TYPE_BOTTOM, CQGnuPlotEnum::AlignBottom  },
 { CVALIGN_TYPE_TOP   , CQGnuPlotEnum::AlignTop     },
 { CVALIGN_TYPE_CENTER, CQGnuPlotEnum::AlignVCenter }
});

typedef CEnumConv<CGnuPlot::PlotStyle, CQGnuPlotEnum::PlotStyle> PlotStyleConv;

PlotStyleConv plotStyleConv({
  { CGnuPlot::PlotStyle::NONE          , CQGnuPlotEnum::PlotNone           },
  { CGnuPlot::PlotStyle::BOXERRORBARS  , CQGnuPlotEnum::PlotBoxErrorBars   },
  { CGnuPlot::PlotStyle::BOXES         , CQGnuPlotEnum::PlotBoxes          },
  { CGnuPlot::PlotStyle::BOXPLOT       , CQGnuPlotEnum::PlotBoxPlot        },
  { CGnuPlot::PlotStyle::BOXXYERRORBARS, CQGnuPlotEnum::PlotBoxXYErrorBars },
  { CGnuPlot::PlotStyle::BUBBLEPLOT    , CQGnuPlotEnum::PlotBubbleChart    },
  { CGnuPlot::PlotStyle::CANDLESTICKS  , CQGnuPlotEnum::PlotCandlesticks   },
  { CGnuPlot::PlotStyle::CHORDDIAGRAM  , CQGnuPlotEnum::PlotChordDiagram   },
  { CGnuPlot::PlotStyle::CIRCLES       , CQGnuPlotEnum::PlotCircles        },
  { CGnuPlot::PlotStyle::DELAUNAY      , CQGnuPlotEnum::PlotDelaunay       },
  { CGnuPlot::PlotStyle::DOTS          , CQGnuPlotEnum::PlotDots           },
  { CGnuPlot::PlotStyle::ELLIPSES      , CQGnuPlotEnum::PlotEllipses       },
  { CGnuPlot::PlotStyle::ERRORBARS     , CQGnuPlotEnum::PlotErrorBars      },
  { CGnuPlot::PlotStyle::ERRORLINES    , CQGnuPlotEnum::PlotErrorLines     },
  { CGnuPlot::PlotStyle::FILLEDCURVES  , CQGnuPlotEnum::PlotFilledCurves   },
  { CGnuPlot::PlotStyle::FILLSTEPS     , CQGnuPlotEnum::PlotFillSteps      },
  { CGnuPlot::PlotStyle::FINANCEBARS   , CQGnuPlotEnum::PlotFinanceBars    },
  { CGnuPlot::PlotStyle::FSTEPS        , CQGnuPlotEnum::PlotFSteps         },
  { CGnuPlot::PlotStyle::HISTEPS       , CQGnuPlotEnum::PlotHiSteps        },
  { CGnuPlot::PlotStyle::HISTOGRAMS    , CQGnuPlotEnum::PlotHistograms     },
  { CGnuPlot::PlotStyle::IMAGE         , CQGnuPlotEnum::PlotImage          },
  { CGnuPlot::PlotStyle::IMPULSES      , CQGnuPlotEnum::PlotImpulses       },
  { CGnuPlot::PlotStyle::LABELS        , CQGnuPlotEnum::PlotLabels         },
  { CGnuPlot::PlotStyle::LINES         , CQGnuPlotEnum::PlotLines          },
  { CGnuPlot::PlotStyle::LINES_POINTS  , CQGnuPlotEnum::PlotLinesPoints    },
  { CGnuPlot::PlotStyle::PARALLELAXES  , CQGnuPlotEnum::PlotParallelAxes   },
  { CGnuPlot::PlotStyle::PIECHART      , CQGnuPlotEnum::PlotPieChart       },
  { CGnuPlot::PlotStyle::PM3D          , CQGnuPlotEnum::PlotPm3d           },
  { CGnuPlot::PlotStyle::POINTS        , CQGnuPlotEnum::PlotPoints         },
  { CGnuPlot::PlotStyle::POLYGONS      , CQGnuPlotEnum::PlotPolygons       },
  { CGnuPlot::PlotStyle::RADAR         , CQGnuPlotEnum::PlotRadar          },
  { CGnuPlot::PlotStyle::RGBALPHA      , CQGnuPlotEnum::PlotRgbAlpha       },
  { CGnuPlot::PlotStyle::RGBIMAGE      , CQGnuPlotEnum::PlotRgbImage       },
  { CGnuPlot::PlotStyle::STEPS         , CQGnuPlotEnum::PlotSteps          },
  { CGnuPlot::PlotStyle::SUNBURST      , CQGnuPlotEnum::PlotSunburst       },
  { CGnuPlot::PlotStyle::TREEMAP       , CQGnuPlotEnum::PlotTreeMap        },
  { CGnuPlot::PlotStyle::VECTORS       , CQGnuPlotEnum::PlotVectors        },
  { CGnuPlot::PlotStyle::XERRORBARS    , CQGnuPlotEnum::PlotXErrorBars     },
  { CGnuPlot::PlotStyle::XERRORLINES   , CQGnuPlotEnum::PlotXErrorLines    },
  { CGnuPlot::PlotStyle::XYERRORBARS   , CQGnuPlotEnum::PlotXYErrorBars    },
  { CGnuPlot::PlotStyle::XYERRORLINES  , CQGnuPlotEnum::PlotXYErrorLines   },
  { CGnuPlot::PlotStyle::YERRORBARS    , CQGnuPlotEnum::PlotYErrorBars     },
  { CGnuPlot::PlotStyle::YERRORLINES   , CQGnuPlotEnum::PlotYErrorLines    },
  { CGnuPlot::PlotStyle::TEST_TERMINAL , CQGnuPlotEnum::PlotTestTerminal   },
  { CGnuPlot::PlotStyle::TEST_PALETTE  , CQGnuPlotEnum::PlotTestPalette    },
});

typedef CEnumConv<CGnuPlot::HistogramStyle, CQGnuPlotEnum::HistogramStyle> HistogramStyleConv;

HistogramStyleConv histogramStyleConv({
 { CGnuPlot::HistogramStyle::NONE         , CQGnuPlotEnum::HistogramNone          },
 { CGnuPlot::HistogramStyle::CLUSTERED    , CQGnuPlotEnum::HistogramClustered     },
 { CGnuPlot::HistogramStyle::ERRORBARS    , CQGnuPlotEnum::HistogramErrorBars     },
 { CGnuPlot::HistogramStyle::ROWSTACKED   , CQGnuPlotEnum::HistogramRowStacked    },
 { CGnuPlot::HistogramStyle::COLUMNSTACKED, CQGnuPlotEnum::HistogramColumnStacked },
});

typedef CEnumConv<CGnuPlotTypes::FillType, CQGnuPlotEnum::FillType> FillTypeConv;

FillTypeConv fillTypeConv({
  { CGnuPlotTypes::FillType::EMPTY  , CQGnuPlotEnum::FillNone      },
  { CGnuPlotTypes::FillType::SOLID  , CQGnuPlotEnum::FillSolid     },
  { CGnuPlotTypes::FillType::PATTERN, CQGnuPlotEnum::FillPatterned },
});

typedef CEnumConv<CGnuPlotTypes::FillPattern, CQGnuPlotEnum::FillPattern> FillPatternConv;

FillPatternConv fillPatternConv({
  { CGnuPlotTypes::FillPattern::NONE  , CQGnuPlotEnum::PatternNone  },
  { CGnuPlotTypes::FillPattern::HATCH , CQGnuPlotEnum::PatternHatch  },
  { CGnuPlotTypes::FillPattern::DENSE , CQGnuPlotEnum::PatternDense  },
  { CGnuPlotTypes::FillPattern::FG    , CQGnuPlotEnum::PatternFg     },
  { CGnuPlotTypes::FillPattern::FDIAG , CQGnuPlotEnum::PatternFDiag  },
  { CGnuPlotTypes::FillPattern::BDIAG , CQGnuPlotEnum::PatternBDiag  },
  { CGnuPlotTypes::FillPattern::FDIAG1, CQGnuPlotEnum::PatternFDiag1 },
  { CGnuPlotTypes::FillPattern::BDIAG1, CQGnuPlotEnum::PatternBDiag1 },
  { CGnuPlotTypes::FillPattern::BG    , CQGnuPlotEnum::PatternBg     },
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

typedef CEnumConv<CGnuPlotTypes::BoxWidthType, CQGnuPlotEnum::BoxWidthType> BoxWidthTypeConv;

BoxWidthTypeConv boxWidthTypeConv({
  { CGnuPlotTypes::BoxWidthType::AUTO    , CQGnuPlotEnum::BoxWidthAuto     },
  { CGnuPlotTypes::BoxWidthType::ABSOLUTE, CQGnuPlotEnum::BoxWidthAbsolute },
  { CGnuPlotTypes::BoxWidthType::RELATIVE, CQGnuPlotEnum::BoxWidthRelative },
});

typedef CEnumConv<CGnuPlotTypes::ColorModel, CQGnuPlotEnum::ColorModelType> ColorModelTypeConv;

ColorModelTypeConv colorModelTypeConv({
  { CGnuPlotTypes::ColorModel::RGB, CQGnuPlotEnum::ColorModelRGB},
  { CGnuPlotTypes::ColorModel::HSV, CQGnuPlotEnum::ColorModelHSV},
  { CGnuPlotTypes::ColorModel::CMY, CQGnuPlotEnum::ColorModelCMY},
  { CGnuPlotTypes::ColorModel::YIQ, CQGnuPlotEnum::ColorModelYIQ},
  { CGnuPlotTypes::ColorModel::XYZ, CQGnuPlotEnum::ColorModelXYZ},
});

typedef CEnumConv<CGnuPlotTypes::DrawLayer, CQGnuPlotEnum::DrawLayerType> DrawLayerTypeConv;

DrawLayerTypeConv drawLayerTypeConv({
  { CGnuPlotTypes::DrawLayer::DEFAULT, CQGnuPlotEnum::LayerDefault},
  { CGnuPlotTypes::DrawLayer::FRONT  , CQGnuPlotEnum::LayerFront  },
  { CGnuPlotTypes::DrawLayer::BACK   , CQGnuPlotEnum::LayerBack   },
  { CGnuPlotTypes::DrawLayer::BEHIND , CQGnuPlotEnum::LayerBehind },
});

typedef CEnumConv<CGnuPlotPalette::ColorType, CQGnuPlotEnum::PaletteColorType> PaletteColorTypeConv;

PaletteColorTypeConv paletteColorTypeConv({
  { CGnuPlotPalette::ColorType::MODEL    , CQGnuPlotEnum::PaletteColorModel     },
  { CGnuPlotPalette::ColorType::DEFINED  , CQGnuPlotEnum::PaletteColorDefined   },
  { CGnuPlotPalette::ColorType::FUNCTIONS, CQGnuPlotEnum::PaletteColorFunctions },
  { CGnuPlotPalette::ColorType::CUBEHELIX, CQGnuPlotEnum::PaletteColorCubeHelix },
});

typedef CEnumConv<CGnuPlotArrow::CoordType, CQGnuPlotEnum::ArrowCoordType> ArrorColorTypeConv;

ArrorColorTypeConv arrowCoordTypeConv({
  { CGnuPlotArrow::CoordType::FROM_TO   , CQGnuPlotEnum::ArrowCoordFromTo    },
  { CGnuPlotArrow::CoordType::FROM_RTO  , CQGnuPlotEnum::ArrowCoordFromRTo   },
  { CGnuPlotArrow::CoordType::FROM_ANGLE, CQGnuPlotEnum::ArrowCoordFromAngle },
});

}

//---

CQGnuPlotEnum::SymbolType
CQGnuPlotUtil::
symbolConv(const CGnuPlotTypes::SymbolType &type)
{
  if (int(type) >= 0 && int(type) <= int(CGnuPlotTypes::SymbolType::LAST))
    return Conv::symbolConv.conv(type);
  else
    return CQGnuPlotEnum::SymbolNone;
}

CGnuPlotTypes::SymbolType
CQGnuPlotUtil::
symbolConv(const CQGnuPlotEnum::SymbolType &type)
{
  return Conv::symbolConv.conv(type);
}

//---

CQGnuPlotEnum::HAlignType
CQGnuPlotUtil::
alignConv(const CHAlignType &type)
{
  return Conv::halignConv.conv(type);
}

CHAlignType
CQGnuPlotUtil::
alignConv(const CQGnuPlotEnum::HAlignType &type)
{
  return Conv::halignConv.conv(type);
}

//---

CQGnuPlotEnum::VAlignType
CQGnuPlotUtil::
alignConv(const CVAlignType &type)
{
  return Conv::valignConv.conv(type);
}

CVAlignType
CQGnuPlotUtil::
alignConv(const CQGnuPlotEnum::VAlignType &type)
{
  return Conv::valignConv.conv(type);
}

//---

CQGnuPlotEnum::PlotStyle
CQGnuPlotUtil::
plotStyleConv(const CGnuPlot::PlotStyle &type)
{
  return Conv::plotStyleConv.conv(type, CQGnuPlotEnum::PlotNone);
}

CGnuPlot::PlotStyle
CQGnuPlotUtil::
plotStyleConv(const CQGnuPlotEnum::PlotStyle &type)
{
  return Conv::plotStyleConv.conv(type);
}

//---

CQGnuPlotEnum::HistogramStyle
CQGnuPlotUtil::
histogramStyleConv(const CGnuPlot::HistogramStyle &type)
{
  return Conv::histogramStyleConv.conv(type);
}

CGnuPlot::HistogramStyle
CQGnuPlotUtil::
histogramStyleConv(const CQGnuPlotEnum::HistogramStyle &type)
{
  return Conv::histogramStyleConv.conv(type);
}

//---

CQGnuPlotEnum::FillType
CQGnuPlotUtil::
fillTypeConv(const CGnuPlotTypes::FillType &type)
{
  return Conv::fillTypeConv.conv(type);
}

CGnuPlotTypes::FillType
CQGnuPlotUtil::
fillTypeConv(const CQGnuPlotEnum::FillType &type)
{
  return Conv::fillTypeConv.conv(type);
}

//---

CQGnuPlotEnum::FillPattern
CQGnuPlotUtil::
fillPatternConv(const CGnuPlotTypes::FillPattern &type)
{
  return Conv::fillPatternConv.conv(type);
}

CGnuPlotTypes::FillPattern
CQGnuPlotUtil::
fillPatternConv(const CQGnuPlotEnum::FillPattern &type)
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

CQGnuPlotEnum::BoxWidthType
CQGnuPlotUtil::
boxWidthTypeConv(const CGnuPlotTypes::BoxWidthType &type)
{
  return Conv::boxWidthTypeConv.conv(type);
}

CGnuPlotTypes::BoxWidthType
CQGnuPlotUtil::
boxWidthTypeConv(const CQGnuPlotEnum::BoxWidthType &type)
{
  return Conv::boxWidthTypeConv.conv(type);
}


//---

CQGnuPlotEnum::ColorModelType
CQGnuPlotUtil::
colorModelTypeConv(const CGnuPlotTypes::ColorModel &layer)
{
  return Conv::colorModelTypeConv.conv(layer);
}

CGnuPlotTypes::ColorModel
CQGnuPlotUtil::
colorModelTypeConv(const CQGnuPlotEnum::ColorModelType &layer)
{
  return Conv::colorModelTypeConv.conv(layer);
}

//---

CQGnuPlotEnum::DrawLayerType
CQGnuPlotUtil::
drawLayerTypeConv(const CGnuPlotTypes::DrawLayer &layer)
{
  return Conv::drawLayerTypeConv.conv(layer);
}

CGnuPlotTypes::DrawLayer
CQGnuPlotUtil::
drawLayerTypeConv(const CQGnuPlotEnum::DrawLayerType &layer)
{
  return Conv::drawLayerTypeConv.conv(layer);
}

//---

CQGnuPlotEnum::PaletteColorType
CQGnuPlotUtil::
paletteColorTypeConv(const CGnuPlotPalette::ColorType &layer)
{
  return Conv::paletteColorTypeConv.conv(layer);
}

CGnuPlotPalette::ColorType
CQGnuPlotUtil::
paletteColorTypeConv(const CQGnuPlotEnum::PaletteColorType &layer)
{
  return Conv::paletteColorTypeConv.conv(layer);
}

//---

CQGnuPlotEnum::ArrowCoordType
CQGnuPlotUtil::
arrowCoordTypeConv(const CGnuPlotArrow::CoordType  &type)
{
  return Conv::arrowCoordTypeConv.conv(type);
}

CGnuPlotArrow::CoordType
CQGnuPlotUtil::
arrowCoordTypeConv(const CQGnuPlotEnum::ArrowCoordType &type)
{
  return Conv::arrowCoordTypeConv.conv(type);
}
