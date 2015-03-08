#ifndef CQGnuPlotUtil_H
#define CQGnuPlotUtil_H

#include <CQGnuPlot.h>
#include <CQGnuPlotPlot.h>
#include <CAlignType.h>

namespace CQGnuPlotUtil {
  CQGnuPlot::SymbolType     symbolConv(const CGnuPlotTypes::SymbolType &type);
  CGnuPlotTypes::SymbolType symbolConv(const CQGnuPlot::SymbolType     &type);

  CQGnuPlot::HAlignType alignConv(const CHAlignType           &type);
  CHAlignType           alignConv(const CQGnuPlot::HAlignType &type);

  CQGnuPlot::VAlignType alignConv(const CVAlignType           &type);
  CVAlignType           alignConv(const CQGnuPlot::VAlignType &type);

  CQGnuPlot::PlotStyle plotStyleConv(const CGnuPlot::PlotStyle  &type);
  CGnuPlot::PlotStyle  plotStyleConv(const CQGnuPlot::PlotStyle &type);

  CQGnuPlot::HistogramStyle histogramStyleConv(const CGnuPlot::HistogramStyle  &type);
  CGnuPlot::HistogramStyle  histogramStyleConv(const CQGnuPlot::HistogramStyle &type);

  CQGnuPlot::FillType     fillTypeConv(const CGnuPlotTypes::FillType &type);
  CGnuPlotTypes::FillType fillTypeConv(const CQGnuPlot::FillType     &type);

  CQGnuPlot::FillPattern     fillPatternConv(const CGnuPlotTypes::FillPattern &type);
  CGnuPlotTypes::FillPattern fillPatternConv(const CQGnuPlot::FillPattern     &type);

  Qt::BrushStyle             fillPatternQtConv(const CGnuPlotTypes::FillPattern &type);
  CGnuPlotTypes::FillPattern fillPatternQtConv(const Qt::BrushStyle             &type);

  CQGnuPlot::BoxWidthType boxWidthTypeConv(const CGnuPlot::BoxWidthType  &type);
  CGnuPlot::BoxWidthType  boxWidthTypeConv(const CQGnuPlot::BoxWidthType &type);

  CQGnuPlot::DrawLayerType drawLayerTypeConv(const CGnuPlotLayer            &layer);
  CGnuPlotLayer            drawLayerTypeConv(const CQGnuPlot::DrawLayerType &layer);

  CQGnuPlot::PaletteColorType paletteColorTypeConv(const CGnuPlotPalette::ColorType    &layer);
  CGnuPlotPalette::ColorType  paletteColorTypeConv(const CQGnuPlot::PaletteColorType &layer);
}

#endif
