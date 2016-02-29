#include <CQGnuPlotBoxObject.h>
#include <CQGnuPlotPlot.h>
#include <CQGnuPlotFill.h>
#include <CQGnuPlotStroke.h>
#include <CQGnuPlotMark.h>

CQGnuPlotBoxObject::
CQGnuPlotBoxObject(CQGnuPlotPlot *plot) :
 CGnuPlotBoxObject(plot)
{
}

CQGnuPlotBoxObject::
~CQGnuPlotBoxObject()
{
}

CQGnuPlotEnum::BoxType
CQGnuPlotBoxObject::
boxType()
{
  return CQGnuPlotEnum::boxTypeConv(CGnuPlotBoxObject::boxType());
}

void
CQGnuPlotBoxObject::
setBoxType(CQGnuPlotEnum::BoxType type)
{
  CGnuPlotBoxObject::setBoxType(CQGnuPlotEnum::boxTypeConv(type));
}

CQGnuPlotEnum::BoxLabels
CQGnuPlotBoxObject::
boxLabels()
{
  return CQGnuPlotEnum::boxLabelsConv(CGnuPlotBoxObject::boxLabels());
}

void
CQGnuPlotBoxObject::
setBoxLabels(CQGnuPlotEnum::BoxLabels labels)
{
  CGnuPlotBoxObject::setBoxLabels(CQGnuPlotEnum::boxLabelsConv(labels));
}

CQGnuPlotFill *
CQGnuPlotBoxObject::
fill() const
{
  return dynamic_cast<CQGnuPlotFill *>(fill_.get());
}

CQGnuPlotStroke *
CQGnuPlotBoxObject::
stroke() const
{
  return dynamic_cast<CQGnuPlotStroke *>(stroke_.get());
}

CQGnuPlotMark *
CQGnuPlotBoxObject::
outlierMark() const
{
  return dynamic_cast<CQGnuPlotMark *>(outlierMark_.get());
}

void
CQGnuPlotBoxObject::
draw(CGnuPlotRenderer *renderer) const
{
  CGnuPlotBoxObject::draw(renderer);
}
