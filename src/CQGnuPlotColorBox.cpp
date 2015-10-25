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
  CPoint3D p = CGnuPlotColorBox::origin().point();

  return CQUtil::toQPoint(CPoint2D(p.x, p.y));
}

void
CQGnuPlotColorBox::
setOrigin(const QPointF &p)
{
  CPoint2D p1 = CQUtil::fromQPoint(p);

  CGnuPlotColorBox::setOrigin(CPoint3D(p1.x, p1.y, 0));
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

QString
CQGnuPlotColorBox::
titleString() const
{
  return CGnuPlotColorBox::titleString().c_str();
}

void
CQGnuPlotColorBox::
setTitleString(const QString &s)
{
  CGnuPlotColorBox::setTitleString(s.toStdString());
}

double
CQGnuPlotColorBox::
getMin() const
{
  return CGnuPlotColorBox::min().getValue(0);
}

void
CQGnuPlotColorBox::
setMin(double r)
{
  CGnuPlotColorBox::setMin(r);
}

double
CQGnuPlotColorBox::
getMax() const
{
  return CGnuPlotColorBox::max().getValue(10);
}

void
CQGnuPlotColorBox::
setMax(double r)
{
  CGnuPlotColorBox::setMax(r);
}

void
CQGnuPlotColorBox::
draw(CGnuPlotRenderer *renderer)
{
  if (! isEnabled())
    return;

  CGnuPlotColorBox::draw(renderer);

  if (isSelected())
    renderer->drawRect(bbox(), CRGBA(1,0,0), 2);
}
