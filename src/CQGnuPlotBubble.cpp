#include <CQGnuPlotBubble.h>
#include <CQGnuPlotPlot.h>
#include <CQGnuPlotUtil.h>
#include <CQGnuPlotRenderer.h>

CQGnuPlotBubble::
CQGnuPlotBubble(CQGnuPlotPlot *plot) :
 CGnuPlotBubble(plot)
{
}

CQGnuPlotBubble::
~CQGnuPlotBubble()
{
}

QString
CQGnuPlotBubble::
getName() const
{
  return QString(CGnuPlotBubble::name().c_str());
}

void
CQGnuPlotBubble::
setName(const QString &name)
{
  CGnuPlotBubble::setName(name.toStdString());
}

QColor
CQGnuPlotBubble::
getColor() const
{
  return toQColor(CGnuPlotBubble::color());
}

void
CQGnuPlotBubble::
setColor(const QColor &color)
{
  CGnuPlotBubble::setColor(fromQColor(color));
}

void
CQGnuPlotBubble::
draw() const
{
  CQGnuPlotBubble *th = const_cast<CQGnuPlotBubble *>(this);

  CRGBA c = color();

  if (isSelected())
    th->CGnuPlotBubble::setColor(CRGBA(1,0,0));

  CGnuPlotBubble::draw();

  if (isSelected())
    th->CGnuPlotBubble::setColor(c);
}
