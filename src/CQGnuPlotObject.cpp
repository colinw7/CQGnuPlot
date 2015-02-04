#include <CQGnuPlotObject.h>
#include <CQGnuPlot.h>
#include <CGnuPlotRenderer.h>
#include <CQUtil.h>

CQGnuPlotAnnotation::
CQGnuPlotAnnotation(CGnuPlotAnnotation *obj) :
 obj_(obj), selected_(false)
{
}

QColor
CQGnuPlotAnnotation::
getStrokeColor() const
{
  return CQUtil::rgbaToColor(obj_->getStrokeColor());
}

void
CQGnuPlotAnnotation::
setStrokeColor(const QColor &c)
{
  obj_->setStrokeColor(CQUtil::colorToRGBA(c));
}

QColor
CQGnuPlotAnnotation::
getFillColor() const
{
  return CQUtil::rgbaToColor(obj_->getFillColor().color());
}

void
CQGnuPlotAnnotation::
setFillColor(const QColor &c)
{
  CGnuPlotColorSpec spec;

  spec.setRGB(CQUtil::colorToRGBA(c));

  obj_->setFillColor(spec);
}

//---

CQGnuPlotArrow::
CQGnuPlotArrow(CQGnuPlot *plot) :
 CQGnuPlotAnnotation(this), CGnuPlotArrow(plot)
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
CQGnuPlotLabel(CQGnuPlot *plot) :
 CQGnuPlotAnnotation(this), CGnuPlotLabel(plot)
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
CQGnuPlotEllipse(CQGnuPlot *plot) :
 CQGnuPlotAnnotation(this), CGnuPlotEllipse(plot)
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
CQGnuPlotPolygon(CQGnuPlot *plot) :
 CQGnuPlotAnnotation(this), CGnuPlotPolygon(plot)
{
}

//---

CQGnuPlotRectangle::
CQGnuPlotRectangle(CQGnuPlot *plot) :
 CQGnuPlotAnnotation(this), CGnuPlotRectangle(plot)
{
}

QPointF
CQGnuPlotRectangle::
getFrom() const
{
  return CQUtil::toQPoint(CGnuPlotRectangle::getFrom().point());
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
  return CQUtil::toQPoint(CGnuPlotRectangle::getTo().point());
}

void
CQGnuPlotRectangle::
setTo(const QPointF &p)
{
  CGnuPlotRectangle::setTo(CQUtil::fromQPoint(p));
}

void
CQGnuPlotRectangle::
draw(CGnuPlotRenderer *renderer) const
{
  CGnuPlotRectangle::draw(renderer);

  if (isSelected())
    renderer->drawRect(bbox_, CRGBA(1,0,0), 2);
}
