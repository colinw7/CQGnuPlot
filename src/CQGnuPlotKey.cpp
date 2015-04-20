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

CQGnuPlot::HAlignType
CQGnuPlotKey::
halign() const
{
  return CQGnuPlotUtil::alignConv(CGnuPlotKey::getHAlign());
}

void
CQGnuPlotKey::
setHAlign(const CQGnuPlot::HAlignType &a)
{
  CGnuPlotKey::setHAlign(CQGnuPlotUtil::alignConv(a));
}

CQGnuPlot::VAlignType
CQGnuPlotKey::
valign() const
{
  return CQGnuPlotUtil::alignConv(CGnuPlotKey::getVAlign());
}

void
CQGnuPlotKey::
setVAlign(const CQGnuPlot::VAlignType &a)
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
