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
  return CQUtil::toQPoint(CGnuPlotColorBox::origin());
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
draw()
{
  if (! isEnabled())
    return;

  CGnuPlotColorBox::draw();

  if (isSelected()) {
    CGnuPlotRenderer *renderer = app()->renderer();

    renderer->drawRect(getBBox(), CRGBA(1,0,0), 2);
  }
}
