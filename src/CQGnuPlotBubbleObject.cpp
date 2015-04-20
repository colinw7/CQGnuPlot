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
  CQGnuPlotBubbleObject *th = const_cast<CQGnuPlotBubbleObject *>(this);

  CRGBA c = color();

  if (isSelected())
    th->CGnuPlotBubbleObject::setColor(CRGBA(1,0,0));

  CGnuPlotBubbleObject::draw(renderer);

  if (isSelected())
    th->CGnuPlotBubbleObject::setColor(c);
}
