#include <CQGnuPlotEnum.h>
#include <CEnumConv.h>

namespace Conv {

typedef CEnumConv<CHAlignType, CQGnuPlotEnum::HAlignType> HAlignConv;

HAlignConv halignConv({
 { CHALIGN_TYPE_LEFT  , CQGnuPlotEnum::AlignLeft    },
 { CHALIGN_TYPE_RIGHT , CQGnuPlotEnum::AlignRight   },
 { CHALIGN_TYPE_CENTER, CQGnuPlotEnum::AlignHCenter }
});

//---

typedef CEnumConv<CVAlignType, CQGnuPlotEnum::VAlignType> VAlignConv;

VAlignConv valignConv({
 { CVALIGN_TYPE_BOTTOM, CQGnuPlotEnum::AlignBottom  },
 { CVALIGN_TYPE_TOP   , CQGnuPlotEnum::AlignTop     },
 { CVALIGN_TYPE_CENTER, CQGnuPlotEnum::AlignVCenter }
});

//---

typedef CEnumConv<CLineCapType, CQGnuPlotEnum::LineCapType> LineCapConv;

LineCapConv lineCapConv({
 { LINE_CAP_TYPE_BUTT  , CQGnuPlotEnum::LineCapButt   },
 { LINE_CAP_TYPE_ROUND , CQGnuPlotEnum::LineCapRound  },
 { LINE_CAP_TYPE_SQUARE, CQGnuPlotEnum::LineCapSquare }
});

//---

typedef CEnumConv<CLineJoinType, CQGnuPlotEnum::LineJoinType> LineJoinConv;

LineJoinConv lineJoinConv({
 { LINE_JOIN_TYPE_MITRE, CQGnuPlotEnum::LineJoinMitre },
 { LINE_JOIN_TYPE_ROUND, CQGnuPlotEnum::LineJoinRound },
 { LINE_JOIN_TYPE_BEVEL, CQGnuPlotEnum::LineJoinBevel }
});

//---

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

//---

typedef CEnumConv<CGnuPlotTypes::PlotStyle, CQGnuPlotEnum::PlotStyle> PlotStyleConv;

PlotStyleConv plotStyleConv({
  { CGnuPlotTypes::PlotStyle::NONE          , CQGnuPlotEnum::PlotNone           },
  { CGnuPlotTypes::PlotStyle::ADJACENCY     , CQGnuPlotEnum::PlotAdjacency      },
  { CGnuPlotTypes::PlotStyle::BOXERRORBARS  , CQGnuPlotEnum::PlotBoxErrorBars   },
  { CGnuPlotTypes::PlotStyle::BOXES         , CQGnuPlotEnum::PlotBoxes          },
  { CGnuPlotTypes::PlotStyle::BOXPLOT       , CQGnuPlotEnum::PlotBoxPlot        },
  { CGnuPlotTypes::PlotStyle::BOXXYERRORBARS, CQGnuPlotEnum::PlotBoxXYErrorBars },
  { CGnuPlotTypes::PlotStyle::BUBBLEPLOT    , CQGnuPlotEnum::PlotBubbleChart    },
  { CGnuPlotTypes::PlotStyle::CANDLESTICKS  , CQGnuPlotEnum::PlotCandlesticks   },
  { CGnuPlotTypes::PlotStyle::CHORDDIAGRAM  , CQGnuPlotEnum::PlotChordDiagram   },
  { CGnuPlotTypes::PlotStyle::CIRCLES       , CQGnuPlotEnum::PlotCircles        },
  { CGnuPlotTypes::PlotStyle::DELAUNAY      , CQGnuPlotEnum::PlotDelaunay       },
  { CGnuPlotTypes::PlotStyle::DOTS          , CQGnuPlotEnum::PlotDots           },
  { CGnuPlotTypes::PlotStyle::ELLIPSES      , CQGnuPlotEnum::PlotEllipses       },
  { CGnuPlotTypes::PlotStyle::ERRORBARS     , CQGnuPlotEnum::PlotErrorBars      },
  { CGnuPlotTypes::PlotStyle::ERRORLINES    , CQGnuPlotEnum::PlotErrorLines     },
  { CGnuPlotTypes::PlotStyle::FILLEDCURVES  , CQGnuPlotEnum::PlotFilledCurves   },
  { CGnuPlotTypes::PlotStyle::FILLSTEPS     , CQGnuPlotEnum::PlotFillSteps      },
  { CGnuPlotTypes::PlotStyle::FINANCEBARS   , CQGnuPlotEnum::PlotFinanceBars    },
  { CGnuPlotTypes::PlotStyle::FSTEPS        , CQGnuPlotEnum::PlotFSteps         },
  { CGnuPlotTypes::PlotStyle::HISTEPS       , CQGnuPlotEnum::PlotHiSteps        },
  { CGnuPlotTypes::PlotStyle::HISTOGRAMS    , CQGnuPlotEnum::PlotHistograms     },
  { CGnuPlotTypes::PlotStyle::IMAGE         , CQGnuPlotEnum::PlotImage          },
  { CGnuPlotTypes::PlotStyle::IMPULSES      , CQGnuPlotEnum::PlotImpulses       },
  { CGnuPlotTypes::PlotStyle::LABELS        , CQGnuPlotEnum::PlotLabels         },
  { CGnuPlotTypes::PlotStyle::LINES         , CQGnuPlotEnum::PlotLines          },
  { CGnuPlotTypes::PlotStyle::LINES_POINTS  , CQGnuPlotEnum::PlotLinesPoints    },
  { CGnuPlotTypes::PlotStyle::PARALLELAXES  , CQGnuPlotEnum::PlotParallelAxes   },
  { CGnuPlotTypes::PlotStyle::PIECHART      , CQGnuPlotEnum::PlotPieChart       },
  { CGnuPlotTypes::PlotStyle::PM3D          , CQGnuPlotEnum::PlotPm3d           },
  { CGnuPlotTypes::PlotStyle::POINTS        , CQGnuPlotEnum::PlotPoints         },
  { CGnuPlotTypes::PlotStyle::POLYGONS      , CQGnuPlotEnum::PlotPolygons       },
  { CGnuPlotTypes::PlotStyle::RADAR         , CQGnuPlotEnum::PlotRadar          },
  { CGnuPlotTypes::PlotStyle::RGBALPHA      , CQGnuPlotEnum::PlotRgbAlpha       },
  { CGnuPlotTypes::PlotStyle::RGBIMAGE      , CQGnuPlotEnum::PlotRgbImage       },
  { CGnuPlotTypes::PlotStyle::STEPS         , CQGnuPlotEnum::PlotSteps          },
  { CGnuPlotTypes::PlotStyle::SUNBURST      , CQGnuPlotEnum::PlotSunburst       },
  { CGnuPlotTypes::PlotStyle::TREEMAP       , CQGnuPlotEnum::PlotTreeMap        },
  { CGnuPlotTypes::PlotStyle::VECTORS       , CQGnuPlotEnum::PlotVectors        },
  { CGnuPlotTypes::PlotStyle::XERRORBARS    , CQGnuPlotEnum::PlotXErrorBars     },
  { CGnuPlotTypes::PlotStyle::XERRORLINES   , CQGnuPlotEnum::PlotXErrorLines    },
  { CGnuPlotTypes::PlotStyle::XYERRORBARS   , CQGnuPlotEnum::PlotXYErrorBars    },
  { CGnuPlotTypes::PlotStyle::XYERRORLINES  , CQGnuPlotEnum::PlotXYErrorLines   },
  { CGnuPlotTypes::PlotStyle::YERRORBARS    , CQGnuPlotEnum::PlotYErrorBars     },
  { CGnuPlotTypes::PlotStyle::YERRORLINES   , CQGnuPlotEnum::PlotYErrorLines    },
  { CGnuPlotTypes::PlotStyle::TEST_TERMINAL , CQGnuPlotEnum::PlotTestTerminal   },
  { CGnuPlotTypes::PlotStyle::TEST_PALETTE  , CQGnuPlotEnum::PlotTestPalette    },
});

//---

typedef CEnumConv<CGnuPlotTypes::FillType, CQGnuPlotEnum::FillType> FillTypeConv;

FillTypeConv fillTypeConv({
  { CGnuPlotTypes::FillType::EMPTY  , CQGnuPlotEnum::FillNone      },
  { CGnuPlotTypes::FillType::SOLID  , CQGnuPlotEnum::FillSolid     },
  { CGnuPlotTypes::FillType::PATTERN, CQGnuPlotEnum::FillPatterned },
});

//---

typedef CEnumConv<CGnuPlotTypes::HistogramStyle, CQGnuPlotEnum::HistogramStyle> HistogramStyleConv;

HistogramStyleConv histogramStyleConv({
 { CGnuPlotTypes::HistogramStyle::NONE         , CQGnuPlotEnum::HistogramNone          },
 { CGnuPlotTypes::HistogramStyle::CLUSTERED    , CQGnuPlotEnum::HistogramClustered     },
 { CGnuPlotTypes::HistogramStyle::ERRORBARS    , CQGnuPlotEnum::HistogramErrorBars     },
 { CGnuPlotTypes::HistogramStyle::ROWSTACKED   , CQGnuPlotEnum::HistogramRowStacked    },
 { CGnuPlotTypes::HistogramStyle::COLUMNSTACKED, CQGnuPlotEnum::HistogramColumnStacked },
});

//---

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

//---

typedef CEnumConv<CGnuPlotTypes::ObjectType, CQGnuPlotEnum::ObjectType> ObjectTypeConv;

ObjectTypeConv objectTypeConv({
  { CGnuPlotTypes::ObjectType::NONE     , CQGnuPlotEnum::ObjectTypeNone      },
  { CGnuPlotTypes::ObjectType::ARROW    , CQGnuPlotEnum::ObjectTypeArrow     },
  { CGnuPlotTypes::ObjectType::CIRCLE   , CQGnuPlotEnum::ObjectTypeCircle    },
  { CGnuPlotTypes::ObjectType::ELLIPSE  , CQGnuPlotEnum::ObjectTypeEllipse   },
  { CGnuPlotTypes::ObjectType::LABEL    , CQGnuPlotEnum::ObjectTypeLabel     },
  { CGnuPlotTypes::ObjectType::POLYGON  , CQGnuPlotEnum::ObjectTypePolygon   },
  { CGnuPlotTypes::ObjectType::RECTANGLE, CQGnuPlotEnum::ObjectTypeRectangle },
});

//---

typedef CEnumConv<CGnuPlotTypes::BoxLabels, CQGnuPlotEnum::BoxLabels> BoxLabelsConv;

BoxLabelsConv boxLabelsConv({
  { CGnuPlotTypes::BoxLabels::Off , CQGnuPlotEnum::BoxLabelOff  },
  { CGnuPlotTypes::BoxLabels::Auto, CQGnuPlotEnum::BoxLabelAuto },
  { CGnuPlotTypes::BoxLabels::X   , CQGnuPlotEnum::BoxLabelX    },
  { CGnuPlotTypes::BoxLabels::X2  , CQGnuPlotEnum::BoxLabelX2   },
});

//---

typedef CEnumConv<CGnuPlotTypes::BoxType, CQGnuPlotEnum::BoxType> BoxTypeConv;

BoxTypeConv boxTypeConv({
  { CGnuPlotTypes::BoxType::CandleSticks, CQGnuPlotEnum::BoxTypeCandleSticks },
  { CGnuPlotTypes::BoxType::FinanceBars , CQGnuPlotEnum::BoxTypeFinanceBars  },
});

//---

}

//---

CQGnuPlotEnum::HAlignType
CQGnuPlotEnum::
halignConv(const CHAlignType &type)
{
  return Conv::halignConv.conv(type);
}

CHAlignType
CQGnuPlotEnum::
halignConv(const CQGnuPlotEnum::HAlignType &type)
{
  return Conv::halignConv.conv(type);
}

//---

CQGnuPlotEnum::VAlignType
CQGnuPlotEnum::
valignConv(const CVAlignType &type)
{
  return Conv::valignConv.conv(type);
}

CVAlignType
CQGnuPlotEnum::
valignConv(const CQGnuPlotEnum::VAlignType &type)
{
  return Conv::valignConv.conv(type);
}

//---

CQGnuPlotEnum::LineCapType
CQGnuPlotEnum::
lineCapConv(const CLineCapType &type)
{
  return Conv::lineCapConv.conv(type);
}

CLineCapType
CQGnuPlotEnum::
lineCapConv(const CQGnuPlotEnum::LineCapType &type)
{
  return Conv::lineCapConv.conv(type);
}

//---

CQGnuPlotEnum::LineJoinType
CQGnuPlotEnum::
lineJoinConv(const CLineJoinType &type)
{
  return Conv::lineJoinConv.conv(type);
}

CLineJoinType
CQGnuPlotEnum::
lineJoinConv(const CQGnuPlotEnum::LineJoinType &type)
{
  return Conv::lineJoinConv.conv(type);
}

//---

CQGnuPlotEnum::SymbolType
CQGnuPlotEnum::
symbolConv(const CGnuPlotTypes::SymbolType &type)
{
  if (int(type) >= 0 && int(type) <= int(CGnuPlotTypes::SymbolType::LAST))
    return Conv::symbolConv.conv(type);
  else
    return CQGnuPlotEnum::SymbolNone;
}

CGnuPlotTypes::SymbolType
CQGnuPlotEnum::
symbolConv(const SymbolType &type)
{
  return Conv::symbolConv.conv(type);
}

//---

CQGnuPlotEnum::PlotStyle
CQGnuPlotEnum::
plotStyleConv(const CGnuPlotTypes::PlotStyle &type)
{
  return Conv::plotStyleConv.conv(type, CQGnuPlotEnum::PlotNone);
}

CGnuPlotTypes::PlotStyle
CQGnuPlotEnum::
plotStyleConv(const PlotStyle &type)
{
  return Conv::plotStyleConv.conv(type);
}

//---

CQGnuPlotEnum::FillType
CQGnuPlotEnum::
fillTypeConv(const CGnuPlotTypes::FillType &type)
{
  return Conv::fillTypeConv.conv(type);
}

CGnuPlotTypes::FillType
CQGnuPlotEnum::
fillTypeConv(const FillType &type)
{
  return Conv::fillTypeConv.conv(type);
}

//---

CQGnuPlotEnum::HistogramStyle
CQGnuPlotEnum::
histogramStyleConv(const CGnuPlotTypes::HistogramStyle &type)
{
  return Conv::histogramStyleConv.conv(type);
}

CGnuPlotTypes::HistogramStyle
CQGnuPlotEnum::
histogramStyleConv(const CQGnuPlotEnum::HistogramStyle &type)
{
  return Conv::histogramStyleConv.conv(type);
}

//---

CQGnuPlotEnum::FillPattern
CQGnuPlotEnum::
fillPatternConv(const CGnuPlotTypes::FillPattern &type)
{
  return Conv::fillPatternConv.conv(type);
}

CGnuPlotTypes::FillPattern
CQGnuPlotEnum::
fillPatternConv(const CQGnuPlotEnum::FillPattern &type)
{
  return Conv::fillPatternConv.conv(type);
}

//---

CQGnuPlotEnum::ObjectType
CQGnuPlotEnum::
objectTypeConv(const CGnuPlotTypes::ObjectType &type)
{
  return Conv::objectTypeConv.conv(type, CQGnuPlotEnum::ObjectTypeNone);
}

CGnuPlotTypes::ObjectType
CQGnuPlotEnum::
objectTypeConv(const ObjectType &type)
{
  return Conv::objectTypeConv.conv(type);
}

//---

CQGnuPlotEnum::BoxLabels
CQGnuPlotEnum::
boxLabelsConv(const CGnuPlotTypes::BoxLabels &labels)
{
  return Conv::boxLabelsConv.conv(labels);
}

CGnuPlotTypes::BoxLabels
CQGnuPlotEnum::
boxLabelsConv(const BoxLabels &labels)
{
  return Conv::boxLabelsConv.conv(labels);
}

//---

CQGnuPlotEnum::BoxType
CQGnuPlotEnum::
boxTypeConv(const CGnuPlotTypes::BoxType &type)
{
  return Conv::boxTypeConv.conv(type);
}

CGnuPlotTypes::BoxType
CQGnuPlotEnum::
boxTypeConv(const BoxType &type)
{
  return Conv::boxTypeConv.conv(type);
}
