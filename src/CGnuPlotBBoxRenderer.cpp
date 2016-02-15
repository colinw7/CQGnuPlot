#include <CGnuPlotBBoxRenderer.h>
#include <CGnuPlotUtil.h>
#include <CSymbol2D.h>
#include <CMathGeom2D.h>

CGnuPlotBBoxRenderer::
CGnuPlotBBoxRenderer(CGnuPlotRenderer *renderer) :
 renderer_(renderer), bbox_()
{
  while (renderer_ && renderer_->isPseudo())
    renderer_ = dynamic_cast<CGnuPlotBBoxRenderer *>(renderer_)->renderer();

  assert(renderer_);

  xmin_ = -std::numeric_limits<double>::max();
  xmax_ =  std::numeric_limits<double>::max();

  width_   = 800;
  height_  = 800;
  mapping_ = true;
  region_  = CBBox2D(0, 0, 1, 1);
  range1_  = CBBox2D(-1, -1, 1, 1);
  range2_  = CBBox2D(-1, -1, 1, 1);
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
  addPoint(point);
}

void
CGnuPlotBBoxRenderer::
drawSymbol(const CPoint2D &point, SymbolType, double size, const CRGBA &, double, bool pixelSize)
{
  if (! pixelSize) {
    CPoint2D d(size/2, size/2);

    addPoint(point - d);
    addPoint(point + d);
  }
  else
    addPoint(point);
}

void
CGnuPlotBBoxRenderer::
drawPath(const std::vector<CPoint2D> &points, const CGnuPlotStroke &)
{
  for (uint i = 0; i < points.size(); ++i)
    addPoint(points[i]);
}

void
CGnuPlotBBoxRenderer::
drawLine(const CPoint2D &point1, const CPoint2D &point2,
         double /*width*/, const CRGBA &, const CLineDash &)
{
  addPoint(point1);
  addPoint(point2);
}

void
CGnuPlotBBoxRenderer::
drawRect(const CBBox2D &rect, const CRGBA &, double /*width*/, const CLineDash &)
{
  addRect(rect);
}

void
CGnuPlotBBoxRenderer::
patternRect(const CBBox2D &rect, CGnuPlotTypes::FillPattern /*pattern*/,
            const CRGBA & /*fg*/, const CRGBA & /*bg*/)
{
  addRect(rect);
}

void
CGnuPlotBBoxRenderer::
fillRect(const CBBox2D &rect, const CRGBA &)
{
  addRect(rect);
}

void
CGnuPlotBBoxRenderer::
drawBezier(const CPoint2D &point1, const CPoint2D &point2,
           const CPoint2D &point3, const CPoint2D &point4,
           double /*width*/, const CRGBA &, const CLineDash &)
{
  addPoint(point1);
  addPoint(point2);
  addPoint(point3);
  addPoint(point4);
}

void
CGnuPlotBBoxRenderer::
drawPolygon(const std::vector<CPoint2D> &points, double /*width*/, const CRGBA &,
            const CLineDash &)
{
  for (uint i = 1; i < points.size(); ++i)
    addPoint(points[i]);
}

void
CGnuPlotBBoxRenderer::
fillPolygon(const std::vector<CPoint2D> &points, const CRGBA &)
{
  for (uint i = 1; i < points.size(); ++i)
    addPoint(points[i]);
}

void
CGnuPlotBBoxRenderer::
patternPolygon(const std::vector<CPoint2D> &points, CGnuPlotTypes::FillPattern,
               const CRGBA &, const CRGBA &)
{
  for (uint i = 1; i < points.size(); ++i)
    addPoint(points[i]);
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

  addPoint(pr1);
  addPoint(pr2);
  addPoint(pr3);
  addPoint(pr4);
}

void
CGnuPlotBBoxRenderer::
fillEllipse(const CPoint2D &center, double rx, double ry, double angle, const CRGBA &c)
{
  drawEllipse(center, rx, ry, angle, c, 0, CLineDash());
}

void
CGnuPlotBBoxRenderer::
patternEllipse(const CPoint2D &center, double rx, double ry, double angle,
               CGnuPlotTypes::FillPattern, const CRGBA &, const CRGBA &)
{
  drawEllipse(center, rx, ry, angle, CRGBA(0,0,0), 0, CLineDash());
}

void
CGnuPlotBBoxRenderer::
drawText(const CPoint2D &point, const std::string &str, const CRGBA &)
{
  if (! renderer_->mapping()) {
    double w  = renderer_->getFont()->getStringWidth(str);
    double fa = renderer_->getFont()->getCharAscent ();
    double fd = renderer_->getFont()->getCharDescent();

    addPoint(CPoint2D(point.x    , point.y - fd));
    addPoint(CPoint2D(point.x + w, point.y + fa));
  }
  else
    addPoint(point);
}

void
CGnuPlotBBoxRenderer::
drawRotatedText(const CPoint2D &point, const std::string &str, double /*ta*/,
                const HAlignPos &/*halignPos*/, const VAlignPos &/*valignPos*/, const CRGBA &)
{
  if (! renderer_->mapping()) {
    double w  = renderer_->getFont()->getStringWidth(str);
    double fa = renderer_->getFont()->getCharAscent ();
    double fd = renderer_->getFont()->getCharDescent();

    CBBox2D bbox;

    bbox.add(CPoint2D(point.x    , point.y - fd));
    bbox.add(CPoint2D(point.x + w, point.y + fa));

    addRect(bbox);
  }
  else
    addPoint(point);
}

void
CGnuPlotBBoxRenderer::
drawPieSlice(const CPoint2D &pc, double /*ri*/, double ro, double angle1, double angle2,
             double /*lw*/, const CRGBA &/*c*/, const CLineDash &)
{
  addPoint(pc);

  double x1 = pc.x + ro*cos(angle1);
  double y1 = pc.y + ro*sin(angle1);
  double x2 = pc.x + ro*cos(angle2);
  double y2 = pc.y + ro*sin(angle2);

  addPoint(CPoint2D(x1, y1));
  addPoint(CPoint2D(x2, y2));
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
  addPoint(p);

  double x1 = p.x + r1*cos(a1);
  double y1 = p.y + r1*sin(a1);
  double x2 = p.x + r2*cos(a2);
  double y2 = p.y + r2*sin(a2);

  addPoint(CPoint2D(x1, y1));
  addPoint(CPoint2D(x2, y2));
}

void
CGnuPlotBBoxRenderer::
drawChord(const CPoint2D &p, double r, double a1, double a2, const CRGBA &, const CRGBA &)
{
  addPoint(p);

  double x1 = p.x + r*cos(a1);
  double y1 = p.y + r*sin(a1);
  double x2 = p.x + r*cos(a2);
  double y2 = p.y + r*sin(a2);

  addPoint(CPoint2D(x1, y1));
  addPoint(CPoint2D(x2, y2));
}

void
CGnuPlotBBoxRenderer::
drawComplexChord(const CPoint2D &p, double r, double a11, double a12,
                 double a21, double a22, const CRGBA &, const CRGBA &)
{
  addPoint(p);

  double x11 = p.x + r*cos(a11);
  double y11 = p.y + r*sin(a11);
  double x12 = p.x + r*cos(a12);
  double y12 = p.y + r*sin(a12);
  double x21 = p.x + r*cos(a21);
  double y21 = p.y + r*sin(a21);
  double x22 = p.x + r*cos(a22);
  double y22 = p.y + r*sin(a22);

  addPoint(CPoint2D(x11, y11));
  addPoint(CPoint2D(x12, y12));
  addPoint(CPoint2D(x21, y21));
  addPoint(CPoint2D(x22, y22));
}

void
CGnuPlotBBoxRenderer::
drawImage(const CPoint2D &, const CImagePtr &)
{
}

void
CGnuPlotBBoxRenderer::
addRect(const CBBox2D &b)
{
  bbox_.add(b);
}

void
CGnuPlotBBoxRenderer::
addPoint(const CPoint2D &p)
{
  bbox_.add(p);
}
