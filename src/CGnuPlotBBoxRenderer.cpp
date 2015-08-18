#include <CGnuPlotBBoxRenderer.h>
#include <CGnuPlotUtil.h>
#include <CSymbol2D.h>
#include <CMathGeom2D.h>

CGnuPlotBBoxRenderer::
CGnuPlotBBoxRenderer(CGnuPlotRenderer *renderer) :
 renderer_(renderer), bbox_()
{
  assert(! renderer_->isPseudo());

  xmin_ = -std::numeric_limits<double>::max();
  xmax_ =  std::numeric_limits<double>::max();

  width_   = 800;
  height_  = 800;
  mapping_ = true;
  region_  = CBBox2D(0, 0, 1, 1);
  range_   = CBBox2D(-1, -1, 1, 1);

  // font_ = renderer_->font();
}

CGnuPlotBBoxRenderer::
~CGnuPlotBBoxRenderer()
{
}

void
CGnuPlotBBoxRenderer::
setXRange(double xmin, double xmax)
{
  xmin_ = xmin;
  xmax_ = xmax;
}

void
CGnuPlotBBoxRenderer::
setCBValue(double x)
{
  cbmin_.updateMin(x);
  cbmax_.updateMax(x);
}

bool
CGnuPlotBBoxRenderer::
isInside(const CPoint2D &p) const
{
  return (p.x >= xmin_ && p.x <= xmax_);
}

void
CGnuPlotBBoxRenderer::
clear(const CRGBA &)
{
}

void
CGnuPlotBBoxRenderer::
drawPoint(const CPoint2D &point, const CRGBA &)
{
  bbox_.add(point);
}

void
CGnuPlotBBoxRenderer::
drawSymbol(const CPoint2D &point, SymbolType, double, const CRGBA &, double)
{
  bbox_.add(point);
}

void
CGnuPlotBBoxRenderer::
drawPath(const std::vector<CPoint2D> &points, double /*width*/, const CRGBA &, const CLineDash &)
{
  for (uint i = 0; i < points.size(); ++i)
    bbox_.add(points[i]);
}

void
CGnuPlotBBoxRenderer::
drawLine(const CPoint2D &point1, const CPoint2D &point2,
         double /*width*/, const CRGBA &, const CLineDash &)
{
  bbox_.add(point1);
  bbox_.add(point2);
}

void
CGnuPlotBBoxRenderer::
drawRect(const CBBox2D &rect, const CRGBA &, double /*width*/)
{
  bbox_.add(rect);
}

void
CGnuPlotBBoxRenderer::
patternRect(const CBBox2D &rect, CGnuPlotTypes::FillPattern /*pattern*/,
            const CRGBA & /*fg*/, const CRGBA & /*bg*/)
{
  bbox_.add(rect);
}

void
CGnuPlotBBoxRenderer::
fillRect(const CBBox2D &rect, const CRGBA &)
{
  bbox_.add(rect);
}

void
CGnuPlotBBoxRenderer::
drawBezier(const CPoint2D &point1, const CPoint2D &point2,
           const CPoint2D &point3, const CPoint2D &point4,
           double /*width*/, const CRGBA &)
{
  bbox_.add(point1);
  bbox_.add(point2);
  bbox_.add(point3);
  bbox_.add(point4);
}

void
CGnuPlotBBoxRenderer::
drawPolygon(const std::vector<CPoint2D> &points, double /*width*/, const CRGBA &)
{
  for (uint i = 1; i < points.size(); ++i)
    bbox_.add(points[i]);
}

void
CGnuPlotBBoxRenderer::
fillPolygon(const std::vector<CPoint2D> &points, const CRGBA &)
{
  for (uint i = 1; i < points.size(); ++i)
    bbox_.add(points[i]);
}

void
CGnuPlotBBoxRenderer::
patternPolygon(const std::vector<CPoint2D> &points, CGnuPlotTypes::FillPattern,
               const CRGBA &, const CRGBA &)
{
  for (uint i = 1; i < points.size(); ++i)
    bbox_.add(points[i]);
}

void
CGnuPlotBBoxRenderer::
drawEllipse(const CPoint2D &center, double rx, double ry, double angle,
            const CRGBA &, double /*width*/, const CLineDash & /*dash*/)
{
  CPoint2D p1 = center + CPoint2D(-rx, -ry);
  CPoint2D p2 = center + CPoint2D(-rx,  ry);
  CPoint2D p3 = center + CPoint2D( rx,  ry);
  CPoint2D p4 = center + CPoint2D( rx, -ry);

  CPoint2D pr1 = CMathGeom2D::RotatePoint(p1, angle, center);
  CPoint2D pr2 = CMathGeom2D::RotatePoint(p2, angle, center);
  CPoint2D pr3 = CMathGeom2D::RotatePoint(p3, angle, center);
  CPoint2D pr4 = CMathGeom2D::RotatePoint(p4, angle, center);

  bbox_.add(pr1);
  bbox_.add(pr2);
  bbox_.add(pr3);
  bbox_.add(pr4);
}

void
CGnuPlotBBoxRenderer::
fillEllipse(const CPoint2D &center, double rx, double ry, double angle, const CRGBA &c)
{
  drawEllipse(center, rx, ry, angle, c, 0, CLineDash());
}

void
CGnuPlotBBoxRenderer::
drawText(const CPoint2D &point, const std::string &, const CRGBA &)
{
  bbox_.add(point);
}

void
CGnuPlotBBoxRenderer::
drawRotatedText(const CPoint2D &p, const std::string &text, double /*ta*/,
                CHAlignType /*hlaign*/, CVAlignType /*vlaign*/, const CRGBA &c)
{
  drawText(p, text, c);
}

void
CGnuPlotBBoxRenderer::
drawPieSlice(const CPoint2D &pc, double /*ri*/, double ro, double angle1, double angle2,
             double /*lw*/, const CRGBA &/*c*/)
{
  bbox_.add(pc);

  double x1 = pc.x + ro*cos(angle1);
  double y1 = pc.y + ro*sin(angle1);
  double x2 = pc.x + ro*cos(angle2);
  double y2 = pc.y + ro*sin(angle2);

  bbox_.add(CPoint2D(x1, y1));
  bbox_.add(CPoint2D(x2, y2));
}

void
CGnuPlotBBoxRenderer::
fillPieSlice(const CPoint2D &pc, double ri, double ro, double angle1, double angle2,
             const CRGBA &c)
{
  drawPieSlice(pc, ri, ro, angle1, angle2, 0, c);
}

void
CGnuPlotBBoxRenderer::
drawArc(const CPoint2D &p, double r1, double r2, double a1, double a2, const CRGBA &)
{
  bbox_.add(p);

  double x1 = p.x + r1*cos(a1);
  double y1 = p.y + r1*sin(a1);
  double x2 = p.x + r2*cos(a2);
  double y2 = p.y + r2*sin(a2);

  bbox_.add(CPoint2D(x1, y1));
  bbox_.add(CPoint2D(x2, y2));
}

void
CGnuPlotBBoxRenderer::
drawChord(const CPoint2D &p, double r, double a1, double a2, const CRGBA &)
{
  bbox_.add(p);

  double x1 = p.x + r*cos(a1);
  double y1 = p.y + r*sin(a1);
  double x2 = p.x + r*cos(a2);
  double y2 = p.y + r*sin(a2);

  bbox_.add(CPoint2D(x1, y1));
  bbox_.add(CPoint2D(x2, y2));
}

void
CGnuPlotBBoxRenderer::
drawChord(const CPoint2D &p, double r, double a11, double a12,
          double a21, double a22, const CRGBA &)
{
  bbox_.add(p);

  double x11 = p.x + r*cos(a11);
  double y11 = p.y + r*sin(a11);
  double x12 = p.x + r*cos(a12);
  double y12 = p.y + r*sin(a12);
  double x21 = p.x + r*cos(a21);
  double y21 = p.y + r*sin(a21);
  double x22 = p.x + r*cos(a22);
  double y22 = p.y + r*sin(a22);

  bbox_.add(CPoint2D(x11, y11));
  bbox_.add(CPoint2D(x12, y12));
  bbox_.add(CPoint2D(x21, y21));
  bbox_.add(CPoint2D(x22, y22));
}
