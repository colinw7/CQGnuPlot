#include <CQGnuPlotRectObject.h>
#include <CQGnuPlotPlot.h>
#include <CQGnuPlotFill.h>
#include <CQGnuPlotStroke.h>

CQGnuPlotRectObject::
CQGnuPlotRectObject(CQGnuPlotPlot *plot) :
 CGnuPlotRectObject(plot)
{
}

CQGnuPlotRectObject::
~CQGnuPlotRectObject()
{
}

QString
CQGnuPlotRectObject::
getText() const
{
  return QString(CGnuPlotRectObject::text().c_str());
}

void
CQGnuPlotRectObject::
setText(const QString &text)
{
  CGnuPlotRectObject::setText(text.toStdString());
}

CQGnuPlotFill *
CQGnuPlotRectObject::
fill() const
{
  return dynamic_cast<CQGnuPlotFill *>(fill_.get());
}

CQGnuPlotStroke *
CQGnuPlotRectObject::
stroke() const
{
  return dynamic_cast<CQGnuPlotStroke *>(stroke_.get());
}

void
CQGnuPlotRectObject::
draw(CGnuPlotRenderer *renderer) const
{
  CGnuPlotRectObject::draw(renderer);
}
