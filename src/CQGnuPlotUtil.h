#ifndef CQGnuPlotUtil_H
#define CQGnuPlotUtil_H

#include <CQGnuPlot.h>
#include <CQGnuPlotPlot.h>
#include <CAlignType.h>
#include <QIcon>
#include <QPixmap>

namespace CQGnuPlotUtil {
  inline QIcon svgIcon(const uchar *data, int len) {
    QPixmap pixmap;

    pixmap.loadFromData(data, len);

    return QIcon(pixmap);
  }

  //---

  Qt::BrushStyle fillPatternQtConv(const CGnuPlotTypes::FillPattern &type);
  CGnuPlotTypes::FillPattern fillPatternQtConv(const Qt::BrushStyle &type);

  CQGnuPlotEnum::BoxWidthType boxWidthTypeConv(const CGnuPlotTypes::BoxWidthType &type);
  CGnuPlotTypes::BoxWidthType boxWidthTypeConv(const CQGnuPlotEnum::BoxWidthType &type);

  CQGnuPlotEnum::ColorModelType colorModelTypeConv(const CGradientPalette::ColorModel &layer);
  CGradientPalette::ColorModel colorModelTypeConv(const CQGnuPlotEnum::ColorModelType &layer);

  CQGnuPlotEnum::DrawLayerType drawLayerTypeConv(const CGnuPlotTypes::DrawLayer &layer);
  CGnuPlotTypes::DrawLayer drawLayerTypeConv(const CQGnuPlotEnum::DrawLayerType &layer);

  CQGnuPlotEnum::PaletteColorType paletteColorTypeConv(const CGnuPlotPalette::ColorType &layer);
  CGnuPlotPalette::ColorType paletteColorTypeConv(const CQGnuPlotEnum::PaletteColorType &layer);

  CQGnuPlotEnum::ArrowCoordType arrowCoordTypeConv(const CGnuPlotArrow::CoordType &type);
  CGnuPlotArrow::CoordType  arrowCoordTypeConv(const CQGnuPlotEnum::ArrowCoordType &type);
}

#endif
