#include <CQGnuPlotEllipseObject.h>
#include <CQGnuPlotPlot.h>
#include <CQGnuPlotFill.h>
#include <CQGnuPlotStroke.h>

CQGnuPlotEllipseObject::
CQGnuPlotEllipseObject(CQGnuPlotPlot *plot) :
 CGnuPlotEllipseObject(plot)
{
}

CQGnuPlotEllipseObject::
~CQGnuPlotEllipseObject()
{
}

QString
CQGnuPlotEllipseObject::
getText() const
{
  return QString(CGnuPlotEllipseObject::text().c_str());
}

void
CQGnuPlotEllipseObject::
setText(const QString &text)
{
  CGnuPlotEllipseObject::setText(text.toStdString());
}

CQGnuPlotFill *
CQGnuPlotEllipseObject::
fill() const
{
  return dynamic_cast<CQGnuPlotFill *>(fill_.get());
}

CQGnuPlotStroke *
CQGnuPlotEllipseObject::
stroke() const
{
  return dynamic_cast<CQGnuPlotStroke *>(stroke_.get());
}

void
CQGnuPlotEllipseObject::
draw(CGnuPlotRenderer *renderer) const
{
  CGnuPlotEllipseObject::draw(renderer);
}
