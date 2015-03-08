#include <CGnuPlotBBoxRenderer.h>
#include <CGnuPlotUtil.h>
#include <CSymbol2D.h>
#include <CMathGeom2D.h>

CGnuPlotBBoxRenderer::
CGnuPlotBBoxRenderer(CGnuPlotRenderer *renderer) :
 renderer_(renderer), bbox_()
{
  width_   = 800;
  height_  = 800;
  mapping_ = true;
  region_  = CBBox2D(0, 0, 1, 1);
  margin_  = CRange2D(10, 10, 10, 10);
  range_   = CBBox2D(-1, -1, 1, 1);

  // font_ = renderer_->font();
}

CGnuPlotBBoxRenderer::
~CGnuPlotBBoxRenderer()
{
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
drawSymbol(const CPoint2D &point, SymbolType, double size, const CRGBA &)
{
  CPoint2D p;

  double pw = renderer_->pixelWidthToWindowWidth  (size);
  double ph = renderer_->pixelHeightToWindowHeight(size);

  bbox_.add(point - CPoint2D(pw, ph));
  bbox_.add(point + CPoint2D(pw, ph));
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
drawEllipse(const CPoint2D &center, double rx, double ry, double a,
            const CRGBA &, double /*width*/)
{
  CPoint2D p1 = center - CPoint2D(rx, ry);
  CPoint2D p2 = center + CPoint2D(rx, ry);

  CPoint2D pr1 = CMathGeom2D::RotatePoint(p1, a, center);
  CPoint2D pr2 = CMathGeom2D::RotatePoint(p2, a, center);

  CBBox2D rect(pr1, pr2);

  bbox_.add(rect);
}

void
CGnuPlotBBoxRenderer::
fillEllipse(const CPoint2D &center, double rx, double ry, double /*a*/, const CRGBA &)
{
  // TODO: rotate by angle
  CBBox2D rect(center - CPoint2D(rx, ry), center + CPoint2D(rx, ry));

  bbox_.add(rect);
}

void
CGnuPlotBBoxRenderer::
drawText(const CPoint2D &point, const std::string &text, const CRGBA &)
{
  // TODO: rotate by font angle
  double w = renderer_->pixelWidthToWindowWidth  (getFont()->getStringWidth(text));
  double h = renderer_->pixelHeightToWindowHeight(getFont()->getCharHeight());

  bbox_.add(point);
  bbox_.add(point + CPoint2D(w, h));
}

void
CGnuPlotBBoxRenderer::
drawPieSlice(const CPoint2D &pc, double r, double angle1, double angle2, const CRGBA &)
{
  bbox_.add(pc);

  double x1 = pc.x + r*cos(angle1);
  double y1 = pc.y + r*sin(angle1);
  double x2 = pc.x + r*cos(angle2);
  double y2 = pc.y + r*sin(angle2);

  bbox_.add(CPoint2D(x1, y1));
  bbox_.add(CPoint2D(x2, y2));
}
