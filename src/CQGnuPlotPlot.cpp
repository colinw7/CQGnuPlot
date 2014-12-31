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

CQGnuPlotPlot::CQFillType
CQGnuPlotPlot::
fillType() const
{
  FillType type = CGnuPlotPlot::fillType();

  switch (type) {
    case FillType::EMPTY  : return FillNone;
    case FillType::SOLID  : return FillSolid;
    case FillType::PATTERN: return FillPatterned;
    default               : return FillNone;
  }
}

void
CQGnuPlotPlot::
setFillType(const CQFillType &type)
{
  switch (type) {
    case FillNone     : CGnuPlotPlot::setFillType(FillType::EMPTY); break;
    case FillSolid    : CGnuPlotPlot::setFillType(FillType::SOLID); break;
    case FillPatterned: CGnuPlotPlot::setFillType(FillType::PATTERN); break;
  }
}

CQGnuPlotPlot::CQFillPattern
CQGnuPlotPlot::
fillPattern() const
{
  FillPattern pattern = CGnuPlotPlot::fillPattern();

  switch (pattern) {
    case FillPattern::NONE  : return PatternNone;
    case FillPattern::HATCH : return PatternHatch;
    case FillPattern::DENSE : return PatternDense;
    case FillPattern::FG    : return PatternFg;
    case FillPattern::FDIAG : return PatternFDiag;
    case FillPattern::BDIAG : return PatternBDiag;
    case FillPattern::FDIAG1: return PatternFDiag1;
    case FillPattern::BDIAG1: return PatternBDiag1;
    case FillPattern::BG    : return PatternBg;
    default                 : return PatternNone;
  }
}

void
CQGnuPlotPlot::
setFillPattern(const CQFillPattern &pattern)
{
  switch (pattern) {
    case PatternNone  : CGnuPlotPlot::setFillPattern(FillPattern::NONE); break;
    case PatternHatch : CGnuPlotPlot::setFillPattern(FillPattern::HATCH); break;
    case PatternDense : CGnuPlotPlot::setFillPattern(FillPattern::DENSE); break;
    case PatternFg    : CGnuPlotPlot::setFillPattern(FillPattern::FG); break;
    case PatternFDiag : CGnuPlotPlot::setFillPattern(FillPattern::FDIAG); break;
    case PatternBDiag : CGnuPlotPlot::setFillPattern(FillPattern::BDIAG); break;
    case PatternFDiag1: CGnuPlotPlot::setFillPattern(FillPattern::FDIAG1); break;
    case PatternBDiag1: CGnuPlotPlot::setFillPattern(FillPattern::BDIAG1); break;
    case PatternBg    : CGnuPlotPlot::setFillPattern(FillPattern::BG); break;
    default           : CGnuPlotPlot::setFillPattern(FillPattern::NONE); break;
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

void
CQGnuPlotPlot::
draw()
{
  bars_.clear();

  CGnuPlotPlot::draw();
}

void
CQGnuPlotPlot::
drawBar(double x, double y, const CBBox2D &bbox, FillType fillType,
        FillPattern fillPattern, const CRGBA &fillColor, const CRGBA &lineColor)
{
  if (selectedPos_.isValid() && bbox.inside(selectedPos_.getValue()))
    CGnuPlotPlot::drawBar(x, y, bbox, CGnuPlot::FillType::SOLID,
                          fillPattern, CRGBA(0.5,0.5,0.5), CRGBA(1,1,1));
  else
    CGnuPlotPlot::drawBar(x, y, bbox, fillType, fillPattern, fillColor, lineColor);

  bars_.push_back(Bar(x, y, bbox));
}

void
CQGnuPlotPlot::
mouseMove(const CPoint2D &p)
{
  selectedPos_.setInvalid();

  for (auto &bar : bars_) {
    if (! bar.bbox.inside(p))
      continue;

    selectedPos_ = p;

    window_->redraw();

    return;
  }
}

bool
CQGnuPlotPlot::
mouseTip(const CPoint2D &p, CQGnuPlot::TipRect &tip)
{
  selectedPos_.setInvalid();

  for (auto &bar : bars_) {
    if (! bar.bbox.inside(p))
      continue;

    tip.str  = QString("%1,%2").arg(bar.x).arg(bar.y);
    tip.rect = CQUtil::toQRect(bar.bbox);

    return true;
  }

  return false;
}
