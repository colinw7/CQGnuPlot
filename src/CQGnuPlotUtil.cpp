#include <CQGnuPlotUtil.h>

CQGnuPlot::SymbolType
CQGnuPlotUtil::
symbolType(const CGnuPlotTypes::SymbolType &type)
{
  switch (type) {
    case CGnuPlotTypes::SymbolType::POINT              : return CQGnuPlot::SymbolPoint;
    case CGnuPlotTypes::SymbolType::PLUS               : return CQGnuPlot::SymbolPlus;
    case CGnuPlotTypes::SymbolType::CROSS              : return CQGnuPlot::SymbolCross;
    case CGnuPlotTypes::SymbolType::STAR               : return CQGnuPlot::SymbolStar;
    case CGnuPlotTypes::SymbolType::BOX                : return CQGnuPlot::SymbolBox;
    case CGnuPlotTypes::SymbolType::FILLED_BOX         : return CQGnuPlot::SymbolFilledBox;
    case CGnuPlotTypes::SymbolType::CIRCLE             : return CQGnuPlot::SymbolCircle;
    case CGnuPlotTypes::SymbolType::FILLED_CIRCLE      : return CQGnuPlot::SymbolFilledCircle;
    case CGnuPlotTypes::SymbolType::TRIANGLE           : return CQGnuPlot::SymbolTriangle;
    case CGnuPlotTypes::SymbolType::FILLED_TRIANGLE    : return CQGnuPlot::SymbolFilledTriangle;
    case CGnuPlotTypes::SymbolType::INV_TRIANGLE       : return CQGnuPlot::SymbolITriangle;
    case CGnuPlotTypes::SymbolType::FILLED_INV_TRIANGLE: return CQGnuPlot::SymbolFilledITriangle;
    case CGnuPlotTypes::SymbolType::DIAMOND            : return CQGnuPlot::SymbolDiamond;
    case CGnuPlotTypes::SymbolType::FILLED_DIAMOND     : return CQGnuPlot::SymbolFilledDiamond;
    default                                            : return CQGnuPlot::SymbolCross;
  }
}

CGnuPlotTypes::SymbolType
CQGnuPlotUtil::
symbolType(const CQGnuPlot::SymbolType &type)
{
  switch (type) {
    case CQGnuPlot::SymbolPoint          : return CGnuPlotTypes::SymbolType::POINT;
    case CQGnuPlot::SymbolPlus           : return CGnuPlotTypes::SymbolType::PLUS;
    case CQGnuPlot::SymbolCross          : return CGnuPlotTypes::SymbolType::CROSS;
    case CQGnuPlot::SymbolStar           : return CGnuPlotTypes::SymbolType::STAR;
    case CQGnuPlot::SymbolBox            : return CGnuPlotTypes::SymbolType::BOX;
    case CQGnuPlot::SymbolFilledBox      : return CGnuPlotTypes::SymbolType::FILLED_BOX;
    case CQGnuPlot::SymbolCircle         : return CGnuPlotTypes::SymbolType::CIRCLE;
    case CQGnuPlot::SymbolFilledCircle   : return CGnuPlotTypes::SymbolType::FILLED_CIRCLE;
    case CQGnuPlot::SymbolTriangle       : return CGnuPlotTypes::SymbolType::TRIANGLE;
    case CQGnuPlot::SymbolFilledTriangle : return CGnuPlotTypes::SymbolType::FILLED_TRIANGLE;
    case CQGnuPlot::SymbolITriangle      : return CGnuPlotTypes::SymbolType::INV_TRIANGLE;
    case CQGnuPlot::SymbolFilledITriangle: return CGnuPlotTypes::SymbolType::FILLED_INV_TRIANGLE;
    case CQGnuPlot::SymbolDiamond        : return CGnuPlotTypes::SymbolType::DIAMOND;
    case CQGnuPlot::SymbolFilledDiamond  : return CGnuPlotTypes::SymbolType::FILLED_DIAMOND;
    default                              : return CGnuPlotTypes::SymbolType::CROSS;
  }
}
