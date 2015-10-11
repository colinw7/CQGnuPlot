#include <CQGnuPlotPolygonObject.h>
#include <CQGnuPlotPlot.h>
#include <CQGnuPlotFill.h>
#include <CQGnuPlotStroke.h>

CQGnuPlotPolygonObject::
CQGnuPlotPolygonObject(CQGnuPlotPlot *plot) :
 CGnuPlotPolygonObject(plot)
{
}

CQGnuPlotPolygonObject::
~CQGnuPlotPolygonObject()
{
}

QString
CQGnuPlotPolygonObject::
getText() const
{
  return QString(CGnuPlotPolygonObject::text().c_str());
}

void
CQGnuPlotPolygonObject::
setText(const QString &text)
{
  CGnuPlotPolygonObject::setText(text.toStdString());
}

CQGnuPlotFill *
CQGnuPlotPolygonObject::
fill() const
{
  return dynamic_cast<CQGnuPlotFill *>(fill_.get());
}

CQGnuPlotStroke *
CQGnuPlotPolygonObject::
stroke() const
{
  return dynamic_cast<CQGnuPlotStroke *>(stroke_.get());
}

void
CQGnuPlotPolygonObject::
draw(CGnuPlotRenderer *renderer) const
{
  CGnuPlotPolygonObject::draw(renderer);
}
