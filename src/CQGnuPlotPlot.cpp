#include <CQGnuPlotPlot.h>
#include <CQGnuPlotWindow.h>
#include <CQGnuPlotRenderer.h>
#include <CQGnuPlotUtil.h>
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

CQGnuPlot::CQHAlignType
CQGnuPlotPlot::
keyHAlign() const
{
  return CQGnuPlotUtil::alignConv(CGnuPlotPlot::getKeyHAlign());
}

void
CQGnuPlotPlot::
setKeyHAlign(const CQGnuPlot::CQHAlignType &a)
{
  CGnuPlotPlot::setKeyHAlign(CQGnuPlotUtil::alignConv(a));
}

CQGnuPlot::CQVAlignType
CQGnuPlotPlot::
keyVAlign() const
{
  return CQGnuPlotUtil::alignConv(CGnuPlotPlot::getKeyVAlign());
}

void
CQGnuPlotPlot::
setKeyVAlign(const CQGnuPlot::CQVAlignType &a)
{
  CGnuPlotPlot::setKeyVAlign(CQGnuPlotUtil::alignConv(a));
}

CQGnuPlot::CQPlotStyle
CQGnuPlotPlot::
plotStyle() const
{
  return CQGnuPlotUtil::plotStyleConv(CGnuPlotPlot::getStyle());
}

void
CQGnuPlotPlot::
setPlotStyle(const CQGnuPlot::CQPlotStyle &s)
{
  CGnuPlotPlot::setStyle(CQGnuPlotUtil::plotStyleConv(s));
}

CQGnuPlot::CQHistogramStyle
CQGnuPlotPlot::
histogramStyle() const
{
  return CQGnuPlotUtil::histogramStyleConv(CGnuPlotPlot::getHistogramStyle());
}

void
CQGnuPlotPlot::
setHistogramStyle(const CQGnuPlot::CQHistogramStyle &s)
{
  CGnuPlotPlot::setHistogramStyle(CQGnuPlotUtil::histogramStyleConv(s));
}

CQGnuPlot::CQFillType
CQGnuPlotPlot::
fillType() const
{
  return CQGnuPlotUtil::fillTypeConv(CGnuPlotPlot::fillType());
}

void
CQGnuPlotPlot::
setFillType(const CQGnuPlot::CQFillType &type)
{
  CGnuPlotPlot::setFillType(CQGnuPlotUtil::fillTypeConv(type));
}

CQGnuPlot::CQFillPattern
CQGnuPlotPlot::
fillPattern() const
{
  return CQGnuPlotUtil::fillPatternConv(CGnuPlotPlot::fillPattern());
}

void
CQGnuPlotPlot::
setFillPattern(const CQGnuPlot::CQFillPattern &pattern)
{
  CGnuPlotPlot::setFillPattern(CQGnuPlotUtil::fillPatternConv(pattern));
}

CQGnuPlot::CQSymbolType
CQGnuPlotPlot::
pointType() const
{
  return CQGnuPlotUtil::symbolConv(CGnuPlotPlot::pointType());
}

void
CQGnuPlotPlot::
setPointType(const CQGnuPlot::CQSymbolType &type)
{
  CGnuPlotPlot::setPointType(CQGnuPlotUtil::symbolConv(type));
}

CQGnuPlot::CQBoxWidthType
CQGnuPlotPlot::
getBoxWidthType() const
{
  return CQGnuPlotUtil::boxWidthTypeConv(CGnuPlotPlot::getBoxWidthType());
}

void
CQGnuPlotPlot::
setBoxWidthType(const CQGnuPlot::CQBoxWidthType &type)
{
  CGnuPlotPlot::setBoxWidthType(CQGnuPlotUtil::boxWidthTypeConv(type));
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
