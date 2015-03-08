#include <CQGnuPlotPie.h>
#include <CQGnuPlotPlot.h>
#include <CQGnuPlotUtil.h>
#include <CQGnuPlotRenderer.h>

CQGnuPlotPie::
CQGnuPlotPie(CQGnuPlotPlot *plot) :
 CGnuPlotPie(plot)
{
}

CQGnuPlotPie::
~CQGnuPlotPie()
{
}

QString
CQGnuPlotPie::
getName() const
{
  return QString(CGnuPlotPie::name().c_str());
}

void
CQGnuPlotPie::
setName(const QString &name)
{
  CGnuPlotPie::setName(name.toStdString());
}

QColor
CQGnuPlotPie::
getColor() const
{
  return toQColor(CGnuPlotPie::color());
}

void
CQGnuPlotPie::
setColor(const QColor &color)
{
  CGnuPlotPie::setColor(fromQColor(color));
}

void
CQGnuPlotPie::
draw(CGnuPlotRenderer *renderer) const
{
  CQGnuPlotPie *th = const_cast<CQGnuPlotPie *>(this);

  CRGBA c = color();

  if (isSelected())
    th->CGnuPlotPie::setColor(CRGBA(1,0,0));

  CGnuPlotPie::draw(renderer);

  if (isSelected())
    th->CGnuPlotPie::setColor(c);
}
