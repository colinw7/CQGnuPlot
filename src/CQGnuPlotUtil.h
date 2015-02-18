#ifndef CQGnuPlotUtil_H
#define CQGnuPlotUtil_H

#include <CQGnuPlot.h>
#include <CQGnuPlotPlot.h>
#include <CAlignType.h>

namespace CQGnuPlotUtil {
  CQGnuPlot::CQSymbolType   symbolConv(const CGnuPlotTypes::SymbolType &type);
  CGnuPlotTypes::SymbolType symbolConv(const CQGnuPlot::CQSymbolType   &type);

  CQGnuPlot::CQHAlignType alignConv(const CHAlignType             &type);
  CHAlignType             alignConv(const CQGnuPlot::CQHAlignType &type);

  CQGnuPlot::CQVAlignType alignConv(const CVAlignType             &type);
  CVAlignType             alignConv(const CQGnuPlot::CQVAlignType &type);

  CQGnuPlot::CQPlotStyle plotStyleConv(const CGnuPlot::PlotStyle    &type);
  CGnuPlot::PlotStyle    plotStyleConv(const CQGnuPlot::CQPlotStyle &type);

  CQGnuPlot::CQHistogramStyle histogramStyleConv(const CGnuPlot::HistogramStyle    &type);
  CGnuPlot::HistogramStyle    histogramStyleConv(const CQGnuPlot::CQHistogramStyle &type);

  CQGnuPlot::CQFillType   fillTypeConv(const CGnuPlotTypes::FillType &type);
  CGnuPlotTypes::FillType fillTypeConv(const CQGnuPlot::CQFillType   &type);

  CQGnuPlot::CQFillPattern   fillPatternConv(const CGnuPlotTypes::FillPattern &type);
  CGnuPlotTypes::FillPattern fillPatternConv(const CQGnuPlot::CQFillPattern   &type);

  Qt::BrushStyle             fillPatternQtConv(const CGnuPlotTypes::FillPattern &type);
  CGnuPlotTypes::FillPattern fillPatternQtConv(const Qt::BrushStyle             &type);

  CQGnuPlot::CQBoxWidthType boxWidthTypeConv(const CGnuPlot::BoxWidthType    &type);
  CGnuPlot::BoxWidthType    boxWidthTypeConv(const CQGnuPlot::CQBoxWidthType &type);

  CQGnuPlot::CQDrawLayerType drawLayerTypeConv(const CGnuPlotLayer              &layer);
  CGnuPlotLayer              drawLayerTypeConv(const CQGnuPlot::CQDrawLayerType &layer);
}

#endif
