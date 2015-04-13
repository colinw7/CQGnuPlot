#include <CGnuPlot3DRenderer.h>
#include <CGnuPlotGroup.h>
#include <CGnuPlotCamera.h>

CGnuPlot3DRenderer::
CGnuPlot3DRenderer(CGnuPlotRenderer *renderer, CGnuPlotGroup *group, Axis axis, double pos) :
 renderer_(renderer), group_(group), axis_(axis), pos_(pos)
{
  setWidth  (renderer_->width  ());
  setHeight (renderer_->height ());
  setRegion (renderer_->region ());
  setMargin (renderer_->margin ());
  setRange  (renderer_->range  ());
  setRatio  (renderer_->ratio  ());
  setMapping(renderer_->mapping());
  setClip   (renderer_->clip   ());

  setReverse(renderer_->reverseX(), renderer_->reverseY());
}

CGnuPlot3DRenderer::
~CGnuPlot3DRenderer()
{
}

void
CGnuPlot3DRenderer::
clear(const CRGBA &c)
{
  renderer_->clear(c);
}

void
CGnuPlot3DRenderer::
drawPoint(const CPoint2D &point, const CRGBA &c)
{
  renderer_->drawPoint(point, c);
}

void
CGnuPlot3DRenderer::
drawSymbol(const CPoint2D &point, SymbolType type, double size, const CRGBA &c)
{
  renderer_->drawSymbol(point, type, size, c);
}

void
CGnuPlot3DRenderer::
drawPath(const std::vector<CPoint2D> &points, double width, const CRGBA &c, const CLineDash &dash)
{
  renderer_->drawPath(points, width, c, dash);
}

void
CGnuPlot3DRenderer::
drawLine(const CPoint2D &point1, const CPoint2D &point2, double width, const CRGBA &c,
         const CLineDash &dash)
{
  renderer_->drawLine(transform(point1), transform(point2), width, c, dash);
}

void
CGnuPlot3DRenderer::
drawRect(const CBBox2D &rect, const CRGBA &c, double width)
{
  renderer_->drawRect(rect, c, width);
}

void
CGnuPlot3DRenderer::
patternRect(const CBBox2D &rect, CGnuPlotTypes::FillPattern pattern,
            const CRGBA &fg, const CRGBA &bg)
{
  renderer_->patternRect(rect, pattern, fg, bg);
}

void
CGnuPlot3DRenderer::
fillRect(const CBBox2D &rect, const CRGBA &c)
{
  renderer_->fillRect(rect, c);
}

void
CGnuPlot3DRenderer::
drawBezier(const CPoint2D &point1, const CPoint2D &point2,
           const CPoint2D &point3, const CPoint2D &point4,
           double width, const CRGBA &c)
{
  renderer_->drawBezier(point1, point2, point3, point4, width, c);
}

void
CGnuPlot3DRenderer::
drawPolygon(const std::vector<CPoint2D> &points, double width, const CRGBA &c)
{
  renderer_->drawPolygon(points, width, c);
}

void
CGnuPlot3DRenderer::
fillPolygon(const std::vector<CPoint2D> &points, const CRGBA &c)
{
  renderer_->fillPolygon(points, c);
}

void
CGnuPlot3DRenderer::
drawEllipse(const CPoint2D &center, double rx, double ry, double a, const CRGBA &c, double width)
{
  renderer_->drawEllipse(center, rx, ry, a, c, width);
}

void
CGnuPlot3DRenderer::
fillEllipse(const CPoint2D &center, double rx, double ry, double a, const CRGBA &c)
{
  renderer_->fillEllipse(center, rx, ry, a, c);
}

void
CGnuPlot3DRenderer::
drawText(const CPoint2D &point, const std::string &text, const CRGBA &c)
{
  renderer_->drawText(transform(point), text, c);
}

void
CGnuPlot3DRenderer::
drawPieSlice(const CPoint2D &pc, double r, double angle1, double angle2, const CRGBA &c)
{
  renderer_->drawPieSlice(pc, r, angle1, angle2, c);
}

CPoint2D
CGnuPlot3DRenderer::
transform(const CPoint2D &p) const
{
  CGnuPlotCamera *camera = group_->camera();

  CPoint3D p1;

  if      (axis_ == Axis::XY)
    p1 = camera->transform(CPoint3D(p.x, p.y, pos_));
  else if (axis_ == Axis::ZX)
    p1 = camera->transform(CPoint3D(p.y, pos_, p.x));
  else if (axis_ == Axis::XZ)
    p1 = camera->transform(CPoint3D(p.x, pos_, p.y));

  return CPoint2D(p1.x, p1.y);
}
