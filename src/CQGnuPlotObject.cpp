#include <CQGnuPlotObject.h>
#include <CQUtil.h>

CQGnuPlotObject::
CQGnuPlotObject(CGnuPlotObject *obj) :
 obj_(obj)
{
}

QColor
CQGnuPlotObject::
getStrokeColor() const
{
  return CQUtil::rgbaToColor(obj_->getStrokeColor());
}

void
CQGnuPlotObject::
setStrokeColor(const QColor &c)
{
  obj_->setStrokeColor(CQUtil::colorToRGBA(c));
}

QColor
CQGnuPlotObject::
getFillColor() const
{
  return CQUtil::rgbaToColor(obj_->getFillColor());
}

void
CQGnuPlotObject::
setFillColor(const QColor &c)
{
  obj_->setFillColor(CQUtil::colorToRGBA(c));
}

//---

CQGnuPlotArrow::
CQGnuPlotArrow() :
 CQGnuPlotObject(this)
{
}

QPointF
CQGnuPlotArrow::
getFrom() const
{
  return CQUtil::toQPoint(CGnuPlotArrow::getFrom());
}

void
CQGnuPlotArrow::
setFrom(const QPointF &p)
{
  CGnuPlotArrow::setFrom(CQUtil::fromQPoint(p));
}

QPointF
CQGnuPlotArrow::
getTo() const
{
  return CQUtil::toQPoint(CGnuPlotArrow::getTo());
}

void
CQGnuPlotArrow::
setTo(const QPointF &p)
{
  CGnuPlotArrow::setTo(CQUtil::fromQPoint(p));
}

//---

CQGnuPlotLabel::
CQGnuPlotLabel() :
 CQGnuPlotObject(this)
{
}

QString
CQGnuPlotLabel::
getText() const
{
  return CGnuPlotLabel::getText().c_str();
}

void
CQGnuPlotLabel::
setText(const QString &s)
{
  CGnuPlotLabel::setText(s.toStdString());
}

QPointF
CQGnuPlotLabel::
getPos() const
{
  return CQUtil::toQPoint(CGnuPlotLabel::getPos());
}

void
CQGnuPlotLabel::
setPos(const QPointF &p)
{
  CGnuPlotLabel::setPos(CQUtil::fromQPoint(p));
}

QString
CQGnuPlotLabel::
getFont() const
{
  return CGnuPlotLabel::getFont().c_str();
}

void
CQGnuPlotLabel::
setFont(const QString &s)
{
  CGnuPlotLabel::setFont(s.toStdString());
}

//---

CQGnuPlotEllipse::
CQGnuPlotEllipse() :
 CQGnuPlotObject(this)
{
}

QPointF
CQGnuPlotEllipse::
getCenter() const
{
  return CQUtil::toQPoint(CGnuPlotEllipse::getCenter());
}

void
CQGnuPlotEllipse::
setCenter(const QPointF &p)
{
  CGnuPlotEllipse::setCenter(CQUtil::fromQPoint(p));
}

//---

CQGnuPlotPolygon::
CQGnuPlotPolygon() :
 CQGnuPlotObject(this)
{
}

//---

CQGnuPlotRectangle::
CQGnuPlotRectangle() :
 CQGnuPlotObject(this)
{
}

QPointF
CQGnuPlotRectangle::
getFrom() const
{
  return CQUtil::toQPoint(CGnuPlotRectangle::getFrom());
}

void
CQGnuPlotRectangle::
setFrom(const QPointF &p)
{
  CGnuPlotRectangle::setFrom(CQUtil::fromQPoint(p));
}

QPointF
CQGnuPlotRectangle::
getTo() const
{
  return CQUtil::toQPoint(CGnuPlotRectangle::getTo());
}

void
CQGnuPlotRectangle::
setTo(const QPointF &p)
{
  CGnuPlotRectangle::setTo(CQUtil::fromQPoint(p));
}
