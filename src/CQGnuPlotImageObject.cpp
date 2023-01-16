#include <CQGnuPlotImageObject.h>
#include <CQGnuPlotPlot.h>
#include <CQUtil.h>
#include <CQUtilGeom.h>

CQGnuPlotImageObject::
CQGnuPlotImageObject(CQGnuPlotPlot *plot) :
 CGnuPlotImageObject(plot)
{
}

CQGnuPlotImageObject::
~CQGnuPlotImageObject()
{
}

QSizeF
CQGnuPlotImageObject::
delta() const
{
  return CQUtil::toQSize(CGnuPlotImageObject::delta());
}

void
CQGnuPlotImageObject::
setDelta(const QSizeF &c)
{
  CGnuPlotImageObject::setDelta(CQUtil::fromQSize(c));
}

QSize
CQGnuPlotImageObject::
size() const
{
  return CQUtil::toQSize(CGnuPlotImageObject::size());
}

void
CQGnuPlotImageObject::
setSize(const QSize &c)
{
  CGnuPlotImageObject::setSize(CQUtil::fromQSize(c));
}

QPointF
CQGnuPlotImageObject::
origin() const
{
  CPoint3D p = CGnuPlotImageObject::origin();

  return CQUtil::toQPoint(CPoint2D(p.x, p.y));
}

void
CQGnuPlotImageObject::
setOrigin(const QPointF &c)
{
  CPoint3D c1 = CGnuPlotImageObject::origin();

  CGnuPlotImageObject::setOrigin(CPoint3D(c.x(), c.y(), c1.z));
}

QPointF
CQGnuPlotImageObject::
center() const
{
  CPoint3D p = CGnuPlotImageObject::center();

  return CQUtil::toQPoint(CPoint2D(p.x, p.y));
}

void
CQGnuPlotImageObject::
setCenter(const QPointF &c)
{
  CPoint3D c1 = CGnuPlotImageObject::center();

  CGnuPlotImageObject::setCenter(CPoint3D(c.x(), c.y(), c1.z));
}

double
CQGnuPlotImageObject::
x() const
{
  return CGnuPlotImageObject::bbox().getXMin();
}

double
CQGnuPlotImageObject::
y() const
{
  return CGnuPlotImageObject::bbox().getYMin();
}

void
CQGnuPlotImageObject::
draw(CGnuPlotRenderer *renderer) const
{
  CGnuPlotImageObject::draw(renderer);
}
