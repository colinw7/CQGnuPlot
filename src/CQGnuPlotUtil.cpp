#include <CQGnuPlotUtil.h>
#include <CEnumConv.h>

namespace Conv {

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

typedef CEnumConv<CGradientPalette::ColorModel, CQGnuPlotEnum::ColorModelType> ColorModelTypeConv;

ColorModelTypeConv colorModelTypeConv({
  { CGradientPalette::ColorModel::RGB, CQGnuPlotEnum::ColorModelRGB},
  { CGradientPalette::ColorModel::HSV, CQGnuPlotEnum::ColorModelHSV},
  { CGradientPalette::ColorModel::CMY, CQGnuPlotEnum::ColorModelCMY},
  { CGradientPalette::ColorModel::YIQ, CQGnuPlotEnum::ColorModelYIQ},
  { CGradientPalette::ColorModel::XYZ, CQGnuPlotEnum::ColorModelXYZ},
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
colorModelTypeConv(const CGradientPalette::ColorModel &layer)
{
  return Conv::colorModelTypeConv.conv(layer);
}

CGradientPalette::ColorModel
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
