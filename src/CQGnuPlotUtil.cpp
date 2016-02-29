#include <CQGnuPlotUtil.h>
#include <CEnumConv.h>

namespace Conv {

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
