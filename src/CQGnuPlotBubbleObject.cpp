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

CQGnuPlotFill *
CQGnuPlotBubbleObject::
fill() const
{
  return dynamic_cast<CQGnuPlotFill *>(fill_.get());
}

CQGnuPlotStroke *
CQGnuPlotBubbleObject::
stroke() const
{
  return dynamic_cast<CQGnuPlotStroke *>(stroke_.get());
}

void
CQGnuPlotBubbleObject::
draw(CGnuPlotRenderer *renderer) const
{
  CGnuPlotBubbleObject::draw(renderer);
}
