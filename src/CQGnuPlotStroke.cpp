#include <CQGnuPlotStroke.h>
#include <CQGnuPlotPlot.h>
#include <CQGnuPlotRenderer.h>

CQGnuPlotStroke::
CQGnuPlotStroke(CQGnuPlotPlot *plot) :
 CGnuPlotStroke(plot)
{
}

CQGnuPlotStroke *
CQGnuPlotStroke::
dup() const
{
  CQGnuPlotPlot *qplot = static_cast<CQGnuPlotPlot *>(plot_);

  CQGnuPlotStroke *stroke = new CQGnuPlotStroke(qplot);

  stroke->CGnuPlotStroke::operator=(*this);

  return stroke;
}

void
CQGnuPlotStroke::
setValues(const CQGnuPlotStroke &stroke)
{
  CGnuPlotStroke::setEnabled   (stroke.isEnabled());
  CGnuPlotStroke::setColor     (fromQColor(stroke.color()));
  CGnuPlotStroke::setWidth     (stroke.width());
  CGnuPlotStroke::setLineDash  (stroke.dash());
  CGnuPlotStroke::setLineCap   (CQGnuPlotEnum::lineCapConv(stroke.lineCap()));
  CGnuPlotStroke::setLineJoin  (CQGnuPlotEnum::lineJoinConv(stroke.lineJoin()));
  CGnuPlotStroke::setMitreLimit(stroke.mitreLimit());
}

QColor
CQGnuPlotStroke::
color() const
{
  return toQColor(CGnuPlotStroke::color());
}

void
CQGnuPlotStroke::
setColor(const QColor &color)
{
  CGnuPlotStroke::setColor(fromQColor(color));

  emit changed();
}

CLineDash
CQGnuPlotStroke::
dash() const
{
  return CGnuPlotStroke::lineDash();
}

void
CQGnuPlotStroke::
setDash(const CLineDash &dash)
{
  CGnuPlotStroke::setLineDash(dash);

  emit changed();
}

CQGnuPlotEnum::LineCapType
CQGnuPlotStroke::
lineCap() const
{
  return CQGnuPlotEnum::lineCapConv(CGnuPlotStroke::lineCap());
}

void
CQGnuPlotStroke::
setLineCap(const CQGnuPlotEnum::LineCapType &a)
{
  CGnuPlotStroke::setLineCap(CQGnuPlotEnum::lineCapConv(a));

  emit changed();
}

CQGnuPlotEnum::LineJoinType
CQGnuPlotStroke::
lineJoin() const
{
  return CQGnuPlotEnum::lineJoinConv(CGnuPlotStroke::lineJoin());
}

void
CQGnuPlotStroke::
setLineJoin(const CQGnuPlotEnum::LineJoinType &a)
{
  CGnuPlotStroke::setLineJoin(CQGnuPlotEnum::lineJoinConv(a));

  emit changed();
}
