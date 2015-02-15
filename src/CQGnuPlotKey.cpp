#include <CQGnuPlotKey.h>
#include <CQGnuPlotGroup.h>
#include <CQGnuPlotUtil.h>
#include <CGnuPlotRenderer.h>

CQGnuPlotKey::
CQGnuPlotKey(CQGnuPlotGroup *group) :
 CGnuPlotKey(group)
{
}

CQGnuPlotKey::
~CQGnuPlotKey()
{
}

CQGnuPlot::CQHAlignType
CQGnuPlotKey::
halign() const
{
  return CQGnuPlotUtil::alignConv(CGnuPlotKey::getHAlign());
}

void
CQGnuPlotKey::
setHAlign(const CQGnuPlot::CQHAlignType &a)
{
  CGnuPlotKey::setHAlign(CQGnuPlotUtil::alignConv(a));
}

CQGnuPlot::CQVAlignType
CQGnuPlotKey::
valign() const
{
  return CQGnuPlotUtil::alignConv(CGnuPlotKey::getVAlign());
}

void
CQGnuPlotKey::
setVAlign(const CQGnuPlot::CQVAlignType &a)
{
  CGnuPlotKey::setVAlign(CQGnuPlotUtil::alignConv(a));
}

void
CQGnuPlotKey::
draw()
{
  if (! isDisplayed())
    return;

  CGnuPlotKey::draw();

  if (isSelected()) {
    CGnuPlotRenderer *renderer = app()->renderer();

    renderer->drawRect(getBBox(), CRGBA(1,0,0), 2);
  }
}
