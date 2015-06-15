#ifndef CQGnuPlotUtil_H
#define CQGnuPlotUtil_H

#include <CQGnuPlot.h>
#include <CQGnuPlotPlot.h>
#include <CAlignType.h>

namespace CQGnuPlotUtil {
  CQGnuPlotEnum::SymbolType symbolConv(const CGnuPlotTypes::SymbolType &type);
  CGnuPlotTypes::SymbolType symbolConv(const CQGnuPlotEnum::SymbolType &type);

  CQGnuPlotEnum::HAlignType alignConv(const CHAlignType &type);
  CHAlignType alignConv(const CQGnuPlotEnum::HAlignType &type);

  CQGnuPlotEnum::VAlignType alignConv(const CVAlignType &type);
  CVAlignType alignConv(const CQGnuPlotEnum::VAlignType &type);

  CQGnuPlotEnum::PlotStyle plotStyleConv(const CGnuPlot::PlotStyle &type);
  CGnuPlot::PlotStyle plotStyleConv(const CQGnuPlotEnum::PlotStyle &type);

  CQGnuPlotEnum::HistogramStyle histogramStyleConv(const CGnuPlot::HistogramStyle &type);
  CGnuPlot::HistogramStyle histogramStyleConv(const CQGnuPlotEnum::HistogramStyle &type);

  CQGnuPlotEnum::FillType fillTypeConv(const CGnuPlotTypes::FillType &type);
  CGnuPlotTypes::FillType fillTypeConv(const CQGnuPlotEnum::FillType &type);

  CQGnuPlotEnum::FillPattern fillPatternConv(const CGnuPlotTypes::FillPattern &type);
  CGnuPlotTypes::FillPattern fillPatternConv(const CQGnuPlotEnum::FillPattern &type);

  Qt::BrushStyle fillPatternQtConv(const CGnuPlotTypes::FillPattern &type);
  CGnuPlotTypes::FillPattern fillPatternQtConv(const Qt::BrushStyle &type);

  CQGnuPlotEnum::BoxWidthType boxWidthTypeConv(const CGnuPlotTypes::BoxWidthType &type);
  CGnuPlotTypes::BoxWidthType boxWidthTypeConv(const CQGnuPlotEnum::BoxWidthType &type);

  CQGnuPlotEnum::ColorModelType colorModelTypeConv(const CGnuPlotTypes::ColorModel &layer);
  CGnuPlotTypes::ColorModel colorModelTypeConv(const CQGnuPlotEnum::ColorModelType &layer);

  CQGnuPlotEnum::DrawLayerType drawLayerTypeConv(const CGnuPlotTypes::DrawLayer &layer);
  CGnuPlotTypes::DrawLayer drawLayerTypeConv(const CQGnuPlotEnum::DrawLayerType &layer);

  CQGnuPlotEnum::PaletteColorType paletteColorTypeConv(const CGnuPlotPalette::ColorType &layer);
  CGnuPlotPalette::ColorType paletteColorTypeConv(const CQGnuPlotEnum::PaletteColorType &layer);

  CQGnuPlotEnum::ArrowCoordType arrowCoordTypeConv(const CGnuPlotArrow::CoordType &type);
  CGnuPlotArrow::CoordType  arrowCoordTypeConv(const CQGnuPlotEnum::ArrowCoordType &type);
}

#endif
