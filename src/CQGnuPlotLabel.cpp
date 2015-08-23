#include <CQGnuPlotLabel.h>
#include <CQGnuPlotGroup.h>
#include <CQGnuPlotRenderer.h>
#include <CQGnuPlotUtil.h>
#include <CQUtil.h>

CQGnuPlotLabel::
CQGnuPlotLabel(CQGnuPlotGroup *group) :
 CQGnuPlotAnnotation(this), CGnuPlotLabel(group)
{
  setObjectName("label");
}

QString
CQGnuPlotLabel::
getText() const
{
  return CGnuPlotLabel::getText().text().c_str();
}

void
CQGnuPlotLabel::
setText(const QString &s)
{
  CGnuPlotLabel::setText(s.toStdString());
}

CQGnuPlotEnum::HAlignType
CQGnuPlotLabel::
getAlign() const
{
  return CQGnuPlotUtil::alignConv(CGnuPlotLabel::getAlign());
}

void
CQGnuPlotLabel::
setAlign(const CQGnuPlotEnum::HAlignType &a)
{
  CGnuPlotLabel::setAlign(CQGnuPlotUtil::alignConv(a));
}

QPointF
CQGnuPlotLabel::
getPos() const
{
  CPoint3D p = CGnuPlotLabel::getPos().point();

  return CQUtil::toQPoint(CPoint2D(p.x, p.y));
}

void
CQGnuPlotLabel::
setPos(const QPointF &p)
{
  CPoint2D p1 = CQUtil::fromQPoint(p);

  CGnuPlotLabel::setPos(CPoint3D(p1.x, p1.y, 0));
}

QFont
CQGnuPlotLabel::
getFont() const
{
  return CQUtil::toQFont(CGnuPlotLabel::getFont());
}

void
CQGnuPlotLabel::
setFont(const QFont &f)
{
  CGnuPlotLabel::setFont(CQUtil::fromQFont(f));
}

QPointF
CQGnuPlotLabel::
getOffset() const
{
  return CQUtil::toQPoint(CGnuPlotLabel::getOffset().getValue(CPoint2D(0, 0)));
}

void
CQGnuPlotLabel::
setOffset(const QPointF &p)
{
  CGnuPlotLabel::setOffset(CQUtil::fromQPoint(p));
}

QColor
CQGnuPlotLabel::
color() const
{
  const CGnuPlotColorSpec &cs = CGnuPlotLabel::textColor();

  return toQColor(cs.color());
}

void
CQGnuPlotLabel::
setColor(const QColor &c)
{
  CGnuPlotColorSpec cs = CGnuPlotLabel::textColor();

  cs.setRGB(fromQColor(c));

  CGnuPlotLabel::setTextColor(cs);
}

void
CQGnuPlotLabel::
draw(CGnuPlotRenderer *renderer) const
{
  if (isSelected()) {
    CQGnuPlotLabel *th = const_cast<CQGnuPlotLabel *>(this);

    CRGBA c = CGnuPlotLabel::getStrokeColor().getValue(CRGBA(0,0,0));

    th->CGnuPlotLabel::setStrokeColor(CRGBA(1,0,0));

    CGnuPlotLabel::draw(renderer);

    th->CGnuPlotLabel::setStrokeColor(c);

    CBBox2D bbox = getBBox();

    renderer->drawRect(bbox, CRGBA(1,0,0), 2);
  }
  else
    CGnuPlotLabel::draw(renderer);
}
