#include <CQGnuPlotBubbleObject.h>
#include <CQGnuPlotPlot.h>
#include <CQGnuPlotUtil.h>
#include <CQGnuPlotRenderer.h>

CQGnuPlotBubbleObject::
CQGnuPlotBubbleObject(CQGnuPlotPlot *plot) :
 CGnuPlotBubbleObject(plot)
{
}

CQGnuPlotBubbleObject::
~CQGnuPlotBubbleObject()
{
}

QString
CQGnuPlotBubbleObject::
getName() const
{
  return QString(CGnuPlotBubbleObject::name().c_str());
}

void
CQGnuPlotBubbleObject::
setName(const QString &name)
{
  CGnuPlotBubbleObject::setName(name.toStdString());
}

QColor
CQGnuPlotBubbleObject::
getColor() const
{
  return toQColor(CGnuPlotBubbleObject::color());
}

void
CQGnuPlotBubbleObject::
setColor(const QColor &color)
{
  CGnuPlotBubbleObject::setColor(fromQColor(color));
}

void
CQGnuPlotBubbleObject::
draw(CGnuPlotRenderer *renderer) const
{
  CGnuPlotBubbleObject::draw(renderer);
}
