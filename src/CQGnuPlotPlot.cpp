#include <CQGnuPlotPlot.h>
#include <CQGnuPlotWindow.h>
#include <CQGnuPlotRenderer.h>
#include <CQUtil.h>

CQGnuPlotPlot::
CQGnuPlotPlot(CQGnuPlotWindow *window) :
 CGnuPlotPlot(window), window_(window), renderer_(0)
{
  renderer_ = new CQGnuPlotRenderer(canvas());

  setRenderer(renderer_);
}

CQGnuPlotPlot::
~CQGnuPlotPlot()
{
}

CQGnuPlotCanvas *
CQGnuPlotPlot::
canvas() const
{
  return window_->canvas();
}

void
CQGnuPlotPlot::
setPainter(QPainter *p)
{
  renderer_->setPainter(p);
}

QColor
CQGnuPlotPlot::
lineColor() const
{
  return toQColor(CGnuPlotPlot::lineColor());
}

void
CQGnuPlotPlot::
setLineColor(const QColor &c)
{
  CGnuPlotPlot::setLineColor(fromQColor(c));
}

CQGnuPlotPlot::CQHAlignType
CQGnuPlotPlot::
getKeyHAlign() const
{
  CHAlignType type = CGnuPlotPlot::getKeyHAlign();

  switch (type) {
    case CHALIGN_TYPE_LEFT  : return AlignLeft;
    case CHALIGN_TYPE_RIGHT : return AlignRight;
    case CHALIGN_TYPE_CENTER: return AlignHCenter;
    default:                  return AlignLeft;
  }
}

void
CQGnuPlotPlot::
setKeyHAlign(const CQHAlignType &a)
{
  switch (a) {
    case AlignLeft   : CGnuPlotPlot::setKeyHAlign(CHALIGN_TYPE_LEFT  ); break;
    case AlignRight  : CGnuPlotPlot::setKeyHAlign(CHALIGN_TYPE_RIGHT ); break;
    case AlignHCenter: CGnuPlotPlot::setKeyHAlign(CHALIGN_TYPE_CENTER); break;
    default:           CGnuPlotPlot::setKeyHAlign(CHALIGN_TYPE_LEFT  ); break;
  }
}

CQGnuPlotPlot::CQVAlignType
CQGnuPlotPlot::
getKeyVAlign() const
{
  CVAlignType type = CGnuPlotPlot::getKeyVAlign();

  switch (type) {
    case CVALIGN_TYPE_BOTTOM: return AlignBottom;
    case CVALIGN_TYPE_TOP   : return AlignTop;
    case CVALIGN_TYPE_CENTER: return AlignVCenter;
    default:                  return AlignBottom;
  }
}

void
CQGnuPlotPlot::
setKeyVAlign(const CQVAlignType &a)
{
  switch (a) {
    case AlignBottom : CGnuPlotPlot::setKeyVAlign(CVALIGN_TYPE_BOTTOM); break;
    case AlignTop    : CGnuPlotPlot::setKeyVAlign(CVALIGN_TYPE_TOP   ); break;
    case AlignVCenter: CGnuPlotPlot::setKeyVAlign(CVALIGN_TYPE_CENTER); break;
    default:           CGnuPlotPlot::setKeyVAlign(CVALIGN_TYPE_BOTTOM); break;
  }
}

CQGnuPlotPlot::CQSymbolType
CQGnuPlotPlot::
pointType() const
{
  SymbolType type = CGnuPlotPlot::pointType();

  switch (type) {
    case SymbolType::POINT              : return SymbolPoint;
    case SymbolType::PLUS               : return SymbolPlus;
    case SymbolType::CROSS              : return SymbolCross;
    case SymbolType::STAR               : return SymbolStar;
    case SymbolType::BOX                : return SymbolBox;
    case SymbolType::FILLED_BOX         : return SymbolFilledBox;
    case SymbolType::CIRCLE             : return SymbolCircle;
    case SymbolType::FILLED_CIRCLE      : return SymbolFilledCircle;
    case SymbolType::TRIANGLE           : return SymbolTriangle;
    case SymbolType::FILLED_TRIANGLE    : return SymbolFilledTriangle;
    case SymbolType::INV_TRIANGLE       : return SymbolITriangle;
    case SymbolType::FILLED_INV_TRIANGLE: return SymbolFilledITriangle;
    case SymbolType::DIAMOND            : return SymbolDiamond;
    case SymbolType::FILLED_DIAMOND     : return SymbolFilledDiamond;
    default                             : return SymbolCross;
  }
}

void
CQGnuPlotPlot::
setPointType(const CQSymbolType &type)
{
  switch (type) {
    case SymbolPoint          : CGnuPlotPlot::setPointType(SymbolType::POINT); break;
    case SymbolPlus           : CGnuPlotPlot::setPointType(SymbolType::PLUS); break;
    case SymbolCross          : CGnuPlotPlot::setPointType(SymbolType::CROSS); break;
    case SymbolStar           : CGnuPlotPlot::setPointType(SymbolType::STAR); break;
    case SymbolBox            : CGnuPlotPlot::setPointType(SymbolType::BOX); break;
    case SymbolFilledBox      : CGnuPlotPlot::setPointType(SymbolType::FILLED_BOX); break;
    case SymbolCircle         : CGnuPlotPlot::setPointType(SymbolType::CIRCLE); break;
    case SymbolFilledCircle   : CGnuPlotPlot::setPointType(SymbolType::FILLED_CIRCLE); break;
    case SymbolTriangle       : CGnuPlotPlot::setPointType(SymbolType::TRIANGLE); break;
    case SymbolFilledTriangle : CGnuPlotPlot::setPointType(SymbolType::FILLED_TRIANGLE); break;
    case SymbolITriangle      : CGnuPlotPlot::setPointType(SymbolType::INV_TRIANGLE); break;
    case SymbolFilledITriangle: CGnuPlotPlot::setPointType(SymbolType::FILLED_INV_TRIANGLE); break;
    case SymbolDiamond        : CGnuPlotPlot::setPointType(SymbolType::DIAMOND); break;
    case SymbolFilledDiamond  : CGnuPlotPlot::setPointType(SymbolType::FILLED_DIAMOND); break;
    default                   : CGnuPlotPlot::setPointType(SymbolType::CROSS); break;
  }
}
