#include <CQGnuPlotEnum.h>
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

typedef CEnumConv<CGnuPlotTypes::PlotStyle, CQGnuPlotEnum::PlotStyle> PlotStyleConv;

PlotStyleConv plotStyleConv({
  { CGnuPlotTypes::PlotStyle::NONE          , CQGnuPlotEnum::PlotNone           },
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
