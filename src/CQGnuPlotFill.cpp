#include <CQGnuPlotFill.h>
#include <CQGnuPlotPlot.h>
#include <CQGnuPlotUtil.h>
#include <CQGnuPlotRenderer.h>

CQGnuPlotFill::
CQGnuPlotFill(CQGnuPlotPlot *plot) :
 CGnuPlotFill(plot)
{
}

CQGnuPlotFill *
CQGnuPlotFill::
dup() const
{
  CQGnuPlotPlot *qplot = dynamic_cast<CQGnuPlotPlot *>(plot_);

  CQGnuPlotFill *fill = new CQGnuPlotFill(qplot);

  fill->CGnuPlotFill::operator=(*this);

  return fill;
}

QColor
CQGnuPlotFill::
color() const
{
  return toQColor(CGnuPlotFill::color());
}

void
CQGnuPlotFill::
setColor(const QColor &color)
{
  CGnuPlotFill::setColor(fromQColor(color));
}

QColor
CQGnuPlotFill::
background() const
{
  return toQColor(CGnuPlotFill::background());
}

void
CQGnuPlotFill::
setBackground(const QColor &color)
{
  CGnuPlotFill::setBackground(fromQColor(color));
}

CQGnuPlotEnum::FillType
CQGnuPlotFill::
type() const
{
  return CQGnuPlotUtil::fillTypeConv(CGnuPlotFill::type());
}

void
CQGnuPlotFill::
setType(const CQGnuPlotEnum::FillType &type)
{
  CGnuPlotFill::setType(CQGnuPlotUtil::fillTypeConv(type));
}

CQGnuPlotEnum::FillPattern
CQGnuPlotFill::
pattern() const
{
  return CQGnuPlotUtil::fillPatternConv(CGnuPlotFill::pattern());
}

void
CQGnuPlotFill::
setPattern(const CQGnuPlotEnum::FillPattern &pattern)
{
  CGnuPlotFill::setPattern(CQGnuPlotUtil::fillPatternConv(pattern));
}
