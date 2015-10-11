#include <CQGnuPlotLabelObject.h>
#include <CQGnuPlotGroup.h>
#include <CQGnuPlotPlot.h>
#include <CQGnuPlotUtil.h>
#include <CQGnuPlotRenderer.h>
#include <CQGnuPlotFill.h>
#include <CQGnuPlotStroke.h>
#include <CQUtil.h>

CQGnuPlotLabelObject::
CQGnuPlotLabelObject(CQGnuPlotPlot *plot) :
 CGnuPlotLabelObject(plot)
{
}

CQGnuPlotLabelObject::
~CQGnuPlotLabelObject()
{
}

QString
CQGnuPlotLabelObject::
getText() const
{
  return CGnuPlotLabelObject::getText().c_str();
}

void
CQGnuPlotLabelObject::
setText(const QString &s)
{
  CGnuPlotLabelObject::setText(s.toStdString());
}

CQGnuPlotEnum::HAlignType
CQGnuPlotLabelObject::
getAlign() const
{
  return CQGnuPlotUtil::alignConv(CGnuPlotLabelObject::getAlign());
}

void
CQGnuPlotLabelObject::
setAlign(const CQGnuPlotEnum::HAlignType &a)
{
  CGnuPlotLabelObject::setAlign(CQGnuPlotUtil::alignConv(a));
}

QPointF
CQGnuPlotLabelObject::
getPos() const
{
  CPoint3D p = CGnuPlotLabelObject::getPos().point();

  return CQUtil::toQPoint(CPoint2D(p.x, p.y));
}

void
CQGnuPlotLabelObject::
setPos(const QPointF &p)
{
  CPoint2D p1 = CQUtil::fromQPoint(p);

  CGnuPlotLabelObject::setPos(CPoint3D(p1.x, p1.y, 0));
}

QFont
CQGnuPlotLabelObject::
getFont() const
{
  return CQUtil::toQFont(CGnuPlotLabelObject::getFont());
}

void
CQGnuPlotLabelObject::
setFont(const QFont &f)
{
  CGnuPlotLabelObject::setFont(CQUtil::fromQFont(f));
}

QPointF
CQGnuPlotLabelObject::
getOffset() const
{
  return CQUtil::toQPoint(CGnuPlotLabelObject::getOffset().getValue(CPoint2D(0, 0)));
}

void
CQGnuPlotLabelObject::
setOffset(const QPointF &p)
{
  CGnuPlotLabelObject::setOffset(CQUtil::fromQPoint(p));
}

QColor
CQGnuPlotLabelObject::
color() const
{
  const CGnuPlotColorSpec &cs = CGnuPlotLabelObject::textColor();

  return toQColor(cs.color());
}

void
CQGnuPlotLabelObject::
setColor(const QColor &c)
{
  CGnuPlotColorSpec cs = CGnuPlotLabelObject::textColor();

  cs.setRGB(fromQColor(c));

  CGnuPlotLabelObject::setTextColor(cs);
}

CQGnuPlotFill *
CQGnuPlotLabelObject::
boxFill() const
{
  return dynamic_cast<CQGnuPlotFill *>(CGnuPlotLabelObject::boxFill());
}

CQGnuPlotStroke *
CQGnuPlotLabelObject::
boxStroke() const
{
  return dynamic_cast<CQGnuPlotStroke *>(CGnuPlotLabelObject::boxStroke());
}

void
CQGnuPlotLabelObject::
draw(CGnuPlotRenderer *renderer) const
{
  CGnuPlotLabelObject::draw(renderer);
}
