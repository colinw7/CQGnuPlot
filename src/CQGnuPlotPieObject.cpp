#include <CQGnuPlotPieObject.h>
#include <CQGnuPlotPlot.h>
#include <CQGnuPlotFill.h>
#include <CQGnuPlotStroke.h>

CQGnuPlotPieObject::
CQGnuPlotPieObject(CQGnuPlotPlot *plot) :
 CGnuPlotPieObject(plot)
{
}

CQGnuPlotPieObject::
~CQGnuPlotPieObject()
{
}

QString
CQGnuPlotPieObject::
getName() const
{
  return QString(CGnuPlotPieObject::name().c_str());
}

void
CQGnuPlotPieObject::
setName(const QString &name)
{
  CGnuPlotPieObject::setName(name.toStdString());
}

CQGnuPlotFill *
CQGnuPlotPieObject::
fill() const
{
  return dynamic_cast<CQGnuPlotFill *>(fill_.get());
}

CQGnuPlotStroke *
CQGnuPlotPieObject::
stroke() const
{
  return dynamic_cast<CQGnuPlotStroke *>(stroke_.get());
}

void
CQGnuPlotPieObject::
draw(CGnuPlotRenderer *renderer) const
{
  CGnuPlotPieObject::draw(renderer);
}
