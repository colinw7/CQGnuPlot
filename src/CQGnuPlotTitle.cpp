#include <CQGnuPlotTitle.h>
#include <CQGnuPlotGroup.h>
#include <CQGnuPlotUtil.h>
#include <CQGnuPlotRenderer.h>
#include <CQFontUtil.h>
#include <CQUtil.h>
#include <CQUtilGeom.h>

CQGnuPlotTitle::
CQGnuPlotTitle(CQGnuPlotGroup *group) :
 CGnuPlotTitle(group)
{
}

CQGnuPlotTitle::
~CQGnuPlotTitle()
{
}

QString
CQGnuPlotTitle::
text() const
{
  return CGnuPlotTitle::text().c_str();
}

void
CQGnuPlotTitle::
setText(const QString &s)
{
  CGnuPlotTitle::setText(s.toStdString());
}

QPointF
CQGnuPlotTitle::
offset() const
{
  return CQUtil::toQPoint(CGnuPlotTitle::lastOffset());
}

void
CQGnuPlotTitle::
setOffset(const QPointF &p)
{
  CPoint2D p1 = CQUtil::fromQPoint(p);

  CGnuPlotPosition pos(CPoint3D(p1.x, p1.y, 0));

  CGnuPlotTitle::setOffset(pos);
}

QColor
CQGnuPlotTitle::
color() const
{
  const CGnuPlotColorSpec &cs = CGnuPlotTitle::color();

  return toQColor(cs.color());
}

void
CQGnuPlotTitle::
setColor(const QColor &c)
{
  CGnuPlotColorSpec cs = CGnuPlotTitle::color();

  cs.setRGB(fromQColor(c));

  CGnuPlotTitle::setColor(cs);
}

QFont
CQGnuPlotTitle::
getFont() const
{
  return CQFontUtil::toQFont(CGnuPlotTitle::font());
}

void
CQGnuPlotTitle::
setFont(const QFont &f)
{
  CGnuPlotTitle::setFont(CQFontUtil::fromQFont(f));
}

void
CQGnuPlotTitle::
draw(CGnuPlotRenderer *renderer) const
{
  CQGnuPlotTitle *th = const_cast<CQGnuPlotTitle *>(this);

  CGnuPlotColorSpec cs = CGnuPlotTitle::color();

  CGnuPlotColorSpec cs1 = cs;

  if (isSelected()) {
    cs1.setRGB(CRGBA(1,0,0));

    th->CGnuPlotTitle::setColor(cs1);
  }

  CGnuPlotTitle::draw(renderer);

  if (isSelected())
    th->CGnuPlotTitle::setColor(cs);
}
