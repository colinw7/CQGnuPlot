#include <CGnuPlotPieObject.h>
#include <CGnuPlotPlot.h>
#include <CGnuPlotRenderer.h>
#include <CGnuPlotUtil.h>

CGnuPlotPieObject::
CGnuPlotPieObject(CGnuPlotPlot *plot) :
 CGnuPlotPlotObject(plot)
{
  fill_   = plot->createFill  ();
  stroke_ = plot->createStroke();
}

bool
CGnuPlotPieObject::
inside(const CGnuPlotMouseEvent &mouseEvent) const
{
  const CPoint2D &p = mouseEvent.window();

  double r = p.distanceTo(center());

  double ir = innerRadius()*radius();

  if (r < ir || r > radius())
    return false;

  // check angle
  double a = CAngle::Rad2Deg(atan2(p.y - center().y, p.x - center().x)); while (a < 0) a += 360.0;

  double a1 = angle1(); while (a1 < 0) a1 += 360.0;
  double a2 = angle2(); while (a2 < 0) a2 += 360.0;

  if (a1 > a2) {
    // crosses zero
    if (a >= 0 && a <= a2)
      return true;

    if (a <= 360 && a >= a1)
      return true;
  }
  else {
    if (a >= a1 && a <= a2)
      return true;
  }

  return false;
}

bool
CGnuPlotPieObject::
keyInside(const CPoint2D &p) const
{
  return keyRect().inside(p);
}

CGnuPlotTipData
CGnuPlotPieObject::
tip() const
{
  CGnuPlotTipData tip;

  tip.setXStr(name());
  tip.setYStr(CStrUtil::strprintf("%g", value()));

  CPoint2D d(radius(), radius());

  CBBox2D rect(center() - d, center() + d);

  tip.setBorderColor(fill_->color());
  tip.setXColor     (fill_->color());
  tip.setYColor     (CRGBA(0,0,0));

  tip.setBBox(rect);

  return tip;
}

void
CGnuPlotPieObject::
draw(CGnuPlotRenderer *renderer) const
{
  if (! isDisplayed()) return;

  bool highlighted = (isHighlighted() || isSelected());

  CGnuPlotFillP   fill   = fill_;
  CGnuPlotStrokeP stroke = stroke_;

  if (highlighted) {
    fill   = fill_  ->dup();
    stroke = stroke_->dup();

    fill->setColor(fill->color().getLightRGBA());

    stroke->setColor(CRGBA(1,0,0));
    stroke->setWidth(2);
  }

  CPoint2D c = center();

  bool exploded = isExploded();

  if (isSelected() && isExplodeSelected())
    exploded = true;

  if (exploded) {
    double angle = CAngle::Deg2Rad((angle1() + angle2())/2.0);

    double dx = 0.1*radius()*cos(angle);
    double dy = 0.1*radius()*sin(angle);

    c.x += dx;
    c.y += dy;
  }

  //---

  double ir = innerRadius()*radius();

  renderer->fillPieSlice  (c, ir, radius(), angle1(), angle2(), *fill  );
  renderer->strokePieSlice(c, ir, radius(), angle1(), angle2(), *stroke);

  //---

  if (name() != "") {
    double ta = (angle1() + angle2())/2.0;

    double tangle = CAngle::Deg2Rad(ta);

    double lr = labelRadius()*radius();

    if (lr < 0.01)
      lr = 0.01;

    double tc = cos(tangle);
    double ts = sin(tangle);

    double tx = c.x + lr*tc;
    double ty = c.y + lr*ts;

    CPoint2D tp(tx, ty);

    CRGBA tc1(0,0,0);

    if (fill->type() == CGnuPlotTypes::FillType::SOLID)
      tc1 = fill->color().bwContrast();

    // aligned ?
    if (isRotatedText()) {
      if (tc >= 0)
        renderer->drawRotatedText(tp, name(), ta, HAlignPos(CHALIGN_TYPE_LEFT, 0),
                                  VAlignPos(CVALIGN_TYPE_CENTER, 0), tc1);
      else
        renderer->drawRotatedText(tp, name(), 180.0 + ta, HAlignPos(CHALIGN_TYPE_RIGHT, 0),
                                  VAlignPos(CVALIGN_TYPE_CENTER, 0), tc1);
    }
    else
      renderer->drawHAlignedText(tp, HAlignPos(CHALIGN_TYPE_CENTER, 0),
                                 VAlignPos(CVALIGN_TYPE_CENTER, 0), name(), tc1);
  }

  //---

  CPoint2D pr(radius(), radius());

  bbox_ = CBBox2D(c - pr, c + pr);
}
