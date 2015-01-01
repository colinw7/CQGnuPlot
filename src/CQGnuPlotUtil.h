#ifndef CQGnuPlotUtil_H
#define CQGnuPlotUtil_H

#include <CQGnuPlot.h>

namespace CQGnuPlotUtil {
  CQGnuPlot::SymbolType symbolType(const CGnuPlotTypes::SymbolType &type);

  CGnuPlotTypes::SymbolType symbolType(const CQGnuPlot::SymbolType &type);
}

#endif
