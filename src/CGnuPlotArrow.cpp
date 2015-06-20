#include <CGnuPlotArrow.h>
#include <CGnuPlotGroup.h>
#include <CGnuPlotRenderer.h>
#include <CGnuPlotLineStyle.h>
#include <CGnuPlotUtil.h>
#include <CMathGeom2D.h>

CRGBA
CGnuPlotArrow::
getLineColor() const
{
  if (! lineColor_.isValid()) {
    int ls = getLineStyle();

    CGnuPlotLineStyleP lsp = group_->app()->getLineStyleInd(ls);

    return lsp->calcColor(group_, CRGBA(0,0,0));
  }
  else
    return lineColor_.getValue();
}

CGnuPlot *
CGnuPlotArrow::
app() const
{
  return (group_ ? group_->app() : 0);
}

double
CGnuPlotArrow::
getLineWidth() const
{
  return style_.lineWidth(app());
}

void
CGnuPlotArrow::
draw(CGnuPlotRenderer *renderer) const
{
  const CGnuPlotArrow *arrow = this;

  CPoint2D from = arrow->from_.getPoint(renderer);

  CPoint2D to;

  if      (coordType_ == CoordType::FROM_TO)
    to = arrow->to_.getPoint(renderer);
  else if (coordType_ == CoordType::FROM_RTO)
    to = from + arrow->to_.getPoint(renderer);
  else if (coordType_ == CoordType::FROM_ANGLE) {
    double dx = arrow->length_.getXValue(renderer)*cos(arrow->angle_.radians());
    double dy = arrow->length_.getXValue(renderer)*sin(arrow->angle_.radians());

    to = from + CPoint2D(dx, dy);
  }

  double fx, fy, tx, ty;

  renderer->windowToPixel(from.x, from.y, &fx, &fy);
  renderer->windowToPixel(to  .x, to  .y, &tx, &ty);

  double w = (arrow->getLineWidth() > 0 ? arrow->getLineWidth() : 1);

  double a = atan2(ty - fy, tx - fx);

  double aa = CAngle::Deg2Rad(arrow->getHeadAngle() > 0 ? arrow->getHeadAngle() : 30);

  const CGnuPlotCoordValue &al = arrow->getHeadLength();

  double l = (al.value() > 0 ? al.pixelXValue(renderer) : 16);

  double l1 = l*cos(aa);

  double c = cos(a), s = sin(a);

  double x1 = fx, y1 = fy;
  double x4 = tx, y4 = ty;

  double x2 = x1 + l1*c;
  double y2 = y1 + l1*s;
  double x3 = x4 - l1*c;
  double y3 = y4 - l1*s;

  double x11 = x1, y11 = y1;
  double x41 = x4, y41 = y4;

  if (arrow->getFHead()) {
    if (arrow->getHeadFilled() || arrow->getHeadEmpty()) {
      x11 = x2;
      y11 = y2;
    }
    else {
      x11 = x1 + w*c;
      y11 = y1 + w*s;
    }
  }

  if (arrow->getTHead()) {
    if (arrow->getHeadFilled() || arrow->getHeadEmpty()) {
      x41 = x3;
      y41 = y3;
    }
    else {
      x41 = x4 - w*c;
      y41 = y4 - w*s;
    }
  }

  double ba = CAngle::Deg2Rad(arrow->getHeadBackAngle() > 0 ? arrow->getHeadBackAngle() : 90);

  renderer->setMapping(false);

  CRGBA lc = arrow->getLineColor();

  if (arrow->isVariable() && arrow->style().varValue().isValid())
    lc = CGnuPlotStyleInst->indexColor(arrow->getVarValue());

  CLineDash dash = getDash().calcDash(group()->app());

  if (arrow->getFHead()) {
    double a1 = a + aa;
    double a2 = a - aa;

    double c1 = cos(a1), s1 = sin(a1);
    double c2 = cos(a2), s2 = sin(a2);

    double xf1 = x1 + l*c1;
    double yf1 = y1 + l*s1;
    double xf2 = x1 + l*c2;
    double yf2 = y1 + l*s2;

    double xf3 = x2, yf3 = y2;

    if (! arrow->getHeadFilled() && ! arrow->getHeadEmpty()) {
      renderer->drawLine(CPoint2D(x1, y1), CPoint2D(xf1, yf1), w, lc, dash);
      renderer->drawLine(CPoint2D(x1, y1), CPoint2D(xf2, yf2), w, lc, dash);
    }
    else {
      if (ba > aa && ba < M_PI) {
        double a3 = a + ba;

        double c3 = cos(a3), s3 = sin(a3);

        CMathGeom2D::IntersectLine(x1, y1, x2, y2, xf1, yf1, xf1 - 10*c3, yf1 - 10*s3, &xf3, &yf3);

        x11 = xf3;
        y11 = yf3;
      }

      std::vector<CPoint2D> points;

      points.push_back(CPoint2D(x1 , y1 ));
      points.push_back(CPoint2D(xf1, yf1));
      points.push_back(CPoint2D(xf3, yf3));
      points.push_back(CPoint2D(xf2, yf2));

      if (arrow->getHeadEmpty())
        renderer->drawPolygon(points, w, lc);
      else
        renderer->fillPolygon(points, lc);
    }
  }

  if (arrow->getTHead()) {
    double a1 = a + M_PI - aa;
    double a2 = a + M_PI + aa;

    double c1 = cos(a1), s1 = sin(a1);
    double c2 = cos(a2), s2 = sin(a2);

    double xt1 = x4 + l*c1;
    double yt1 = y4 + l*s1;
    double xt2 = x4 + l*c2;
    double yt2 = y4 + l*s2;

    double xt3 = x3, yt3 = y3;

    if (! arrow->getHeadFilled() && ! arrow->getHeadEmpty()) {
      renderer->drawLine(CPoint2D(x4, y4), CPoint2D(xt1, yt1), w, lc, dash);
      renderer->drawLine(CPoint2D(x4, y4), CPoint2D(xt2, yt2), w, lc, dash);
    }
    else {
      if (ba > aa && ba < M_PI) {
        double a3 = a + M_PI - ba;

        double c3 = cos(a3), s3 = sin(a3);

        CMathGeom2D::IntersectLine(x3, y3, x4, y4, xt1, yt1, xt1 - 10*c3, yt1 - 10*s3, &xt3, &yt3);

        x41 = xt3;
        y41 = yt3;
      }

      std::vector<CPoint2D> points;

      points.push_back(CPoint2D(x4 , y4 ));
      points.push_back(CPoint2D(xt1, yt1));
      points.push_back(CPoint2D(xt3, yt3));
      points.push_back(CPoint2D(xt2, yt2));

      if (arrow->getHeadEmpty())
        renderer->drawPolygon(points, w, lc);
      else
        renderer->fillPolygon(points, lc);
    }
  }

  renderer->drawLine(CPoint2D(x11, y11), CPoint2D(x41, y41), w, lc, dash);

  renderer->setMapping(true);
}

bool
CGnuPlotArrow::
inside(const CPoint2D &) const
{
  return false;
}

void
CGnuPlotArrow::
print(std::ostream &os) const
{
  style_.print(app(), os);

  os << " from " << from_;

  if      (coordType_ == CoordType::FROM_RTO)
    os << " rto " << to_;
  else if (coordType_ == CoordType::FROM_TO)
    os << " to " << to_;
  else if (coordType_ == CoordType::FROM_ANGLE)
    os << " length " << length_ << " angle " << angle_;;
}
