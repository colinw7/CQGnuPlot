#include <CGnuPlotArrowData.h>
#include <CGnuPlotGroup.h>
#include <CGnuPlotRenderer.h>
#include <CMathGeom2D.h>

CRGBA
CGnuPlotArrowData::
calcLineColor(CGnuPlotGroup *group) const
{
  if (! getLineColor().isValid()) {
    if (group) {
      int ls = getLineStyle();

      CGnuPlotLineStyleP lsp = group->app()->getLineStyleInd(ls);

      return lsp->calcColor(group, CRGBA(0,0,0));
    }
    else
      return CRGBA(0,0,0);
  }
  else
    return getLineColor().getValue();
}

double
CGnuPlotArrowData::
calcLineWidth(CGnuPlotGroup *group) const
{
  if (group)
    return style().lineWidth(group->app());
  else
    return style().lineWidth(nullptr);
}

void
CGnuPlotArrowData::
draw(CGnuPlotRenderer *renderer, CGnuPlotGroup *group, bool highlighted) const
{
  CGnuPlotStroke stroke;

  lc_ = calcLineColor(group);

  if (isVariable() && style().varValue().isValid())
    lc_ = CGnuPlotStyleInst->indexColor(int(getVarValue()));

  CRGBA lc = lc_;

  if (highlighted)
    lc = CRGBA(1,0,0);

  CLineDash dash = getDash().calcDash(group->app());

  double w = (calcLineWidth(group) > 0 ? calcLineWidth(group) : 1);

  stroke.setEnabled (true);
  stroke.setColor   (lc);
  stroke.setWidth   (w);
  stroke.setLineDash(dash);

  //----

  draw(renderer, group, stroke);
}

void
CGnuPlotArrowData::
draw(CGnuPlotRenderer *renderer, CGnuPlotGroup *group, const CGnuPlotStroke &stroke) const
{
  CGnuPlotFill fill;

  fill.setType (CGnuPlotTypes::FillType::SOLID);
  fill.setColor(stroke.color());

  tol_ = renderer->pixelWidthToWindowWidth(4);

  //---

  bbox_.reset();

  CPoint3D from, to;

  getLine(renderer, from, to);

  line_ = CLine2D(from.toPoint2D(), to.toPoint2D());

  bbox_.add(from.toPoint2D());
  bbox_.add(to  .toPoint2D());

  //---

  double fx, fy, tx, ty;

  renderer->windowToPixel(from.x, from.y, &fx, &fy);
  renderer->windowToPixel(to  .x, to  .y, &tx, &ty);

  pline_ = CLine2D(CPoint2D(fx, fy), CPoint2D(tx, ty));

  double a = atan2(ty - fy, tx - fx);

  double aa = CAngle::Deg2Rad(getHeadAngle() > 0 ? getHeadAngle() : 30);

  const CGnuPlotCoordValue &al = getHeadLength();

  double l = (al.value() > 0 ? al.pixelXValue(renderer) : 16);

  double l1 = l*cos(aa);

  double c = cos(a), s = sin(a);

  double x1 = fx, y1 = fy, z1 = from.z;
  double x4 = tx, y4 = ty, z4 = to  .z;

  double x2 = x1 + l1*c, y2 = y1 + l1*s, z2 = z1;
  double x3 = x4 - l1*c, y3 = y4 - l1*s, z3 = z4;

  double x11 = x1, y11 = y1, z11 = z1;
  double x41 = x4, y41 = y4, z41 = z4;

  if (getFHead()) {
    if (getHeadFilled() || getHeadEmpty()) {
      x11 = x2;
      y11 = y2;
      z11 = z2;
    }
    else {
      x11 = x1 + stroke.width()*c;
      y11 = y1 + stroke.width()*s;
      z11 = z1;
    }
  }

  if (getTHead()) {
    if (getHeadFilled() || getHeadEmpty()) {
      x41 = x3;
      y41 = y3;
      z41 = z3;
    }
    else {
      x41 = x4 - stroke.width()*c;
      y41 = y4 - stroke.width()*s;
      z41 = z4;
    }
  }

  double ba = CAngle::Deg2Rad(getHeadBackAngle() > 0 ? getHeadBackAngle() : 90);

  //---

  renderer->setMapping(false);

  //---

  if (getFHead()) {
    double a1 = a + aa;
    double a2 = a - aa;

    double c1 = cos(a1), s1 = sin(a1);
    double c2 = cos(a2), s2 = sin(a2);

    double xf1 = x1 + l*c1, yf1 = y1 + l*s1, zf1 = z1;
    double xf2 = x1 + l*c2, yf2 = y1 + l*s2, zf2 = z1;

    double xf3 = x2, yf3 = y2, zf3 = z2;

    if (! getHeadFilled() && ! getHeadEmpty()) {
      std::vector<CPoint2D> points = {{CPoint2D(xf1, yf1), CPoint2D(x1, y1), CPoint2D(xf2, yf2)}};

      if (group->isHidden3D()) {
        double zm = CGnuPlotUtil::avg({zf1, z1, zf2});

        CGnuPlotHiddenObjectP obj = renderer->strokeHiddenPath(points, zm, stroke);

        if (obj.isValid())
          obj->setMapping(false);
      }
      else
        renderer->strokePath(points, stroke);
    }
    else {
      if (ba > aa && ba < M_PI) {
        double a3 = a + ba;

        double c3 = cos(a3), s3 = sin(a3);

        CMathGeom2D::IntersectLine(x1, y1, x2, y2, xf1, yf1, xf1 - 10*c3, yf1 - 10*s3, &xf3, &yf3);

        x11 = xf3;
        y11 = yf3;
        z11 = zf3;
      }

      std::vector<CPoint2D> points;

      points.push_back(CPoint2D(x1 , y1 ));
      points.push_back(CPoint2D(xf1, yf1));
      points.push_back(CPoint2D(xf3, yf3));
      points.push_back(CPoint2D(xf2, yf2));

      if (getHeadEmpty()) {
        if (group->isHidden3D()) {
          double zm = CGnuPlotUtil::avg({z1, zf1, zf3, zf2});

          CGnuPlotHiddenObjectP obj = renderer->strokeHiddenPolygon(points, zm, stroke);

          if (obj.isValid())
            obj->setMapping(false);
        }
        else
          renderer->strokePolygon(points, stroke);
      }
      else {
        if (group->isHidden3D()) {
          double zm = CGnuPlotUtil::avg({z1, zf1, zf3, zf2});

          CGnuPlotHiddenObjectP obj = renderer->fillHiddenPolygon(points, zm, fill);

          if (obj.isValid())
            obj->setMapping(false);
        }
        else
          renderer->fillPolygon(points, fill);
      }
    }
  }

  //---

  if (getTHead()) {
    double a1 = a + M_PI - aa;
    double a2 = a + M_PI + aa;

    double c1 = cos(a1), s1 = sin(a1);
    double c2 = cos(a2), s2 = sin(a2);

    double xt1 = x4 + l*c1, yt1 = y4 + l*s1, zt1 = z4;
    double xt2 = x4 + l*c2, yt2 = y4 + l*s2, zt2 = z4;

    double xt3 = x3, yt3 = y3, zt3 = z3;

    if (! getHeadFilled() && ! getHeadEmpty()) {
      std::vector<CPoint2D> points = {{CPoint2D(xt1, yt1), CPoint2D(x4, y4), CPoint2D(xt2, yt2)}};

      if (group->isHidden3D()) {
        double zm = CGnuPlotUtil::avg({zt1, z4, zt2});

        CGnuPlotHiddenObjectP obj = renderer->strokeHiddenPath(points, zm, stroke);

        if (obj.isValid())
          obj->setMapping(false);
      }
      else
        renderer->strokePath(points, stroke);
    }
    else {
      if (ba > aa && ba < M_PI) {
        double a3 = a + M_PI - ba;

        double c3 = cos(a3), s3 = sin(a3);

        CMathGeom2D::IntersectLine(x3, y3, x4, y4, xt1, yt1, xt1 - 10*c3, yt1 - 10*s3, &xt3, &yt3);

        x41 = xt3;
        y41 = yt3;
        z41 = zt3;
      }

      std::vector<CPoint2D> points;

      points.push_back(CPoint2D(x4 , y4 ));
      points.push_back(CPoint2D(xt1, yt1));
      points.push_back(CPoint2D(xt3, yt3));
      points.push_back(CPoint2D(xt2, yt2));

      if (getHeadEmpty()) {
        if (group->isHidden3D()) {
          double zm = CGnuPlotUtil::avg({z4, zt1, zt3, zt2});

          CGnuPlotHiddenObjectP obj = renderer->strokeHiddenPolygon(points, zm, stroke);

          if (obj.isValid())
            obj->setMapping(false);
        }
        else
          renderer->strokePolygon(points, stroke);
      }
      else {
        if (group->isHidden3D()) {
          double zm = CGnuPlotUtil::avg({z4, zt1, zt3, zt2});

          CGnuPlotHiddenObjectP obj = renderer->fillHiddenPolygon(points, zm, fill);

          if (obj.isValid())
            obj->setMapping(false);
        }
        else
          renderer->fillPolygon(points, fill);
      }
    }
  }

  //---

  std::vector<CPoint2D> points = {{CPoint2D(x11, y11), CPoint2D(x41, y41)}};

  if (group->isHidden3D()) {
    double zm = CGnuPlotUtil::avg({z11, z41});

    CGnuPlotHiddenObjectP obj = renderer->strokeHiddenLine(points[0], points[1], zm, stroke);

    if (obj.isValid())
      obj->setMapping(false);
  }
  else
    renderer->strokePath(points, stroke);

  //---

  renderer->setMapping(true);
}

CGnuPlotTipData
CGnuPlotArrowData::
tip() const
{
  const CLine2D &l = this->drawLine();

  CGnuPlotTipData tip;

  tip.setXStr(CStrUtil::strprintf("%g, %g", l.start().x, l.start().y));
  tip.setYStr(CStrUtil::strprintf("%g, %g", l.end  ().x, l.end  ().y));

  CBBox2D rect(l.start(), l.end());

  CRGBA c = this->drawColor();

  tip.setBorderColor(c);
  tip.setXColor(c);

  tip.setBBox(rect);

  return tip;
}

void
CGnuPlotArrowData::
getLine(CGnuPlotRenderer *renderer, CPoint3D &from, CPoint3D &to) const
{
  CPoint3D from3 = getFrom().getPoint3D(renderer);

  CPoint3D to3;

  if      (coordType() == CoordType::FROM_TO)
    to3 = getTo().getPoint3D(renderer);
  else if (coordType() == CoordType::FROM_RTO)
    to3 = from3 + getTo().getPoint3D(renderer);
  else if (coordType() == CoordType::FROM_ANGLE) {
    double dx = getLength().getXValue(renderer)*cos(getAngle().radians());
    double dy = getLength().getXValue(renderer)*sin(getAngle().radians());
    double dz = 0;

    to3 = from3 + CPoint3D(dx, dy, dz);
  }

  from = renderer->transform(from3);
  to   = renderer->transform(to3);
}

bool
CGnuPlotArrowData::
inside(const CGnuPlotMouseEvent &mouseEvent) const
{
  double d;

  CMathGeom2D::PointLineDistance(mouseEvent.pixel(), drawPixelLine(), &d);

  return (d < 4);
}
