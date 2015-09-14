#include <CQGnuPlotKey.h>
#include <CQGnuPlotGroup.h>
#include <CQGnuPlotUtil.h>
#include <CGnuPlotRenderer.h>
#include <CQUtil.h>

CQGnuPlotKey::
CQGnuPlotKey(CQGnuPlotGroup *group) :
 CGnuPlotKey(group)
{
}

CQGnuPlotKey::
~CQGnuPlotKey()
{
}

int
CQGnuPlotKey::
boxLineStyle() const
{
  return CGnuPlotKey::boxLineStyle().getValue(-1);
}

CQGnuPlotEnum::HAlignType
CQGnuPlotKey::
halign() const
{
  return CQGnuPlotUtil::alignConv(CGnuPlotKey::getHAlign());
}

void
CQGnuPlotKey::
setHAlign(const CQGnuPlotEnum::HAlignType &a)
{
  CGnuPlotKey::setHAlign(CQGnuPlotUtil::alignConv(a));
}

CQGnuPlotEnum::VAlignType
CQGnuPlotKey::
valign() const
{
  return CQGnuPlotUtil::alignConv(CGnuPlotKey::getVAlign());
}

void
CQGnuPlotKey::
setVAlign(const CQGnuPlotEnum::VAlignType &a)
{
  CGnuPlotKey::setVAlign(CQGnuPlotUtil::alignConv(a));
}

QFont
CQGnuPlotKey::
getFont() const
{
  return CQUtil::toQFont(CGnuPlotKey::getFont());
}

void
CQGnuPlotKey::
setFont(const QFont &f)
{
  CGnuPlotKey::setFont(CQUtil::fromQFont(f));
}

void
CQGnuPlotKey::
draw(CGnuPlotRenderer *renderer)
{
  if (! isDisplayed())
    return;

  CGnuPlotKey::draw(renderer);

  if (isSelected()) {
    renderer->drawRect(getBBox(), CRGBA(1,0,0), 2);
  }
}

bool
CQGnuPlotKey::
mousePress(const QPoint &qp)
{
  CGnuPlotRenderer *renderer = app()->renderer();

  CPoint2D p;

  renderer->pixelToWindow(CPoint2D(qp.x(), qp.y()), p);

  CGnuPlotPlot *plot = group()->getPlotForId(CGnuPlotKey::plotAtPos(p));
  if (! plot) return false;

  plot->setDisplayed(! plot->isDisplayed());

  return true;
}
