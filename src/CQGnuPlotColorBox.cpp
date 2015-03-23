#include <CQGnuPlotColorBox.h>
#include <CQGnuPlotGroup.h>
#include <CQGnuPlotUtil.h>
#include <CGnuPlotRenderer.h>
#include <CQUtil.h>

CQGnuPlotColorBox::
CQGnuPlotColorBox(CQGnuPlotGroup *group) :
 CGnuPlotColorBox(group)
{
}

CQGnuPlotColorBox::
~CQGnuPlotColorBox()
{
}

QPointF
CQGnuPlotColorBox::
origin() const
{
  return CQUtil::toQPoint(CGnuPlotColorBox::origin().point());
}

void
CQGnuPlotColorBox::
setOrigin(const QPointF &p)
{
  CGnuPlotColorBox::setOrigin(CQUtil::fromQPoint(p));
}

QSizeF
CQGnuPlotColorBox::
size() const
{
  return CQUtil::toQSize(CGnuPlotColorBox::size());
}

void
CQGnuPlotColorBox::
setSize(const QSizeF &s)
{
  CGnuPlotColorBox::setSize(CQUtil::fromQSize(s));
}

void
CQGnuPlotColorBox::
draw(CGnuPlotRenderer *renderer)
{
  if (! isEnabled())
    return;

  CGnuPlotColorBox::draw(renderer);

  if (isSelected())
    renderer->drawRect(getBBox(), CRGBA(1,0,0), 2);
}
