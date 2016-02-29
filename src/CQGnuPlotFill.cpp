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

void
CQGnuPlotFill::
setValues(const CQGnuPlotFill &fill)
{
  CGnuPlotFill::setColor     (fromQColor(fill.color()));
  CGnuPlotFill::setBackground(fromQColor(fill.background()));
  CGnuPlotFill::setType      (CQGnuPlotEnum::fillTypeConv(fill.type()));
  CGnuPlotFill::setPattern   (CQGnuPlotEnum::fillPatternConv(fill.pattern()));
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

  emit changed();
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

  emit changed();
}

CQGnuPlotEnum::FillType
CQGnuPlotFill::
type() const
{
  return CQGnuPlotEnum::fillTypeConv(CGnuPlotFill::type());
}

void
CQGnuPlotFill::
setType(const CQGnuPlotEnum::FillType &type)
{
  CGnuPlotFill::setType(CQGnuPlotEnum::fillTypeConv(type));

  emit changed();
}

CQGnuPlotEnum::FillPattern
CQGnuPlotFill::
pattern() const
{
  return CQGnuPlotEnum::fillPatternConv(CGnuPlotFill::pattern());
}

void
CQGnuPlotFill::
setPattern(const CQGnuPlotEnum::FillPattern &pattern)
{
  CGnuPlotFill::setPattern(CQGnuPlotEnum::fillPatternConv(pattern));

  emit changed();
}
