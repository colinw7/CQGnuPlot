#include <CGnuPlotLogRenderer.h>
#include <CGnuPlotLogDevice.h>
#include <CGnuPlotUtil.h>

CGnuPlotLogRenderer::
CGnuPlotLogRenderer(CGnuPlotLogDevice *device) :
 device_(device)
{
}

CGnuPlotLogRenderer::
~CGnuPlotLogRenderer()
{
}

std::ostream &
CGnuPlotLogRenderer::
os()
{
  return device_->os();
}

void
CGnuPlotLogRenderer::
clear(const CRGBA &c)
{
  log("clear " + logValue("color", c));
}

void
CGnuPlotLogRenderer::
drawPoint(const CPoint2D &point, const CRGBA &c)
{
  log("drawPoint " + logValue("point", point) + " " + logValue("color", c));
}

void
CGnuPlotLogRenderer::
drawSymbol(const CPoint2D &point, SymbolType type, double size, const CRGBA &c,
           double lw, bool pixelSize)
{
  log("drawSymbol " + logValue("point", point) + " " + logValue("type", int(type)) + " " +
      logValue("size", size) + " " + logValue("color", c) + " " +
      logValue("lw", lw) + " " + logValue("pixelSize", pixelSize));
}

void
CGnuPlotLogRenderer::
drawPath(const std::vector<CPoint2D> &points, double width, const CRGBA &c, const CLineDash &dash)
{
  log("drawPath " + logValues("points", points) + " " + logValue("width", width) + " " +
      logValue("color", c) + " " + logValue("dash", dash));
}

void
CGnuPlotLogRenderer::
drawLine(const CPoint2D &point1, const CPoint2D &point2, double width, const CRGBA &c,
         const CLineDash &dash)
{
  log("drawLine " + logValue("point1", point1) + " " + logValue("point2", point2) + " " +
      logValue("width", width) + " " + logValue("color", c) + " " + logValue("dash", dash));
}

void
CGnuPlotLogRenderer::
drawRect(const CBBox2D &rect, const CRGBA &c, double width)
{
  log("drawRect " + logValue("rect", rect) + " " + logValue("color", c) + " " +
      logValue("width", width));
}

void
CGnuPlotLogRenderer::
patternRect(const CBBox2D &rect, CGnuPlotTypes::FillPattern pattern,
            const CRGBA &fg, const CRGBA &bg)
{
  log("patternRect " + logValue("rect", rect) + " " + logValue("pattern", int(pattern)) + " " +
      logValue("fg", fg) + " " + logValue("bg", bg));
}

void
CGnuPlotLogRenderer::
fillRect(const CBBox2D &rect, const CRGBA &c)
{
  log("fillRect " + logValue("rect", rect) + " " + logValue("color", c));
}

void
CGnuPlotLogRenderer::
drawBezier(const CPoint2D &point1, const CPoint2D &point2,
           const CPoint2D &point3, const CPoint2D &point4,
           double width, const CRGBA &c)
{
  log("drawBezier " + logValue("point1", point1) + " " + logValue("point2", point2) + " " +
      logValue("point3", point3) + " " + logValue("point4", point4) + " " +
      logValue("width", width) + " " + logValue("color", c));
}

void
CGnuPlotLogRenderer::
drawPolygon(const std::vector<CPoint2D> &points, double width, const CRGBA &c,
            const CLineDash &dash)
{
  log("drawPolygon " + logValues("points", points) + " " + logValue("width", width) + " " +
      logValue("color", c) + " " + logValue("dash", dash));
}

void
CGnuPlotLogRenderer::
fillPolygon(const std::vector<CPoint2D> &points, const CRGBA &c)
{
  log("fillPolygon " + logValues("points", points) + " " + logValue("color", c));
}

void
CGnuPlotLogRenderer::
patternPolygon(const std::vector<CPoint2D> &points, CGnuPlotTypes::FillPattern pattern,
               const CRGBA &fg, const CRGBA &bg)
{
  log("patternPolygon " + logValues("points", points) + " " +
      logValue("pattern", int(pattern)) + " " + logValue("fg", fg) + " " + logValue("bg", bg));
}

void
CGnuPlotLogRenderer::
drawEllipse(const CPoint2D &center, double rx, double ry, double a, const CRGBA &c,
            double width, const CLineDash &dash)
{
  log("drawEllipse " + logValue("center", center) + " " + logValue("rx", rx) + " " +
      logValue("ry", ry) + " " + logValue("a", a) + " " + logValue("color", c) + " " +
      logValue("width", width) + " " + logValue("color", c) + " " + logValue("dash", dash));
}

void
CGnuPlotLogRenderer::
fillEllipse(const CPoint2D &center, double rx, double ry, double a, const CRGBA &c)
{
  log("fillEllipse " + logValue("center", center) + " " + logValue("rx", rx) + " " +
      logValue("ry", ry) + " " + logValue("a", a) + " " + logValue("color", c));
}

void
CGnuPlotLogRenderer::
patternEllipse(const CPoint2D &center, double rx, double ry, double a,
               CGnuPlotTypes::FillPattern pattern, const CRGBA &fg, const CRGBA &bg)
{
  log("patternEllipse " + logValue("center", center) + " " + logValue("rx", rx) + " " +
      logValue("ry", ry) + " " + logValue("a", a) + " " +
      logValue("pattern", int(pattern)) + " " + logValue("fg", fg) + " " + logValue("bg", bg));
}

void
CGnuPlotLogRenderer::
drawText(const CPoint2D &point, const std::string &text, const CRGBA &c)
{
  log("drawText " + logValue("point", point) + " " + logValue("text", text) + " " +
      logValue("color", c));
}

void
CGnuPlotLogRenderer::
drawRotatedText(const CPoint2D &point, const std::string &text, double angle,
                CHAlignType halign, CVAlignType valign, const CRGBA &c)
{
  log("drawRotatedText " + logValue("point", point) + " " + logValue("text", text) + " " +
      logValue("angle", angle) + " " + logValue("halign", halign) + " " +
      logValue("valign", valign) + " " + logValue("color", c));
}

void
CGnuPlotLogRenderer::
drawPieSlice(const CPoint2D &center, double ri, double ro, double angle1, double angle2,
             double width, const CRGBA &c)
{
  log("drawPieSlice " + logValue("center", center) + " " + logValue("ri", ri) + " " +
      logValue("ro", ro) + " " + logValue("angle1", angle1) + " " +
      logValue("angle2", angle2) + " " + logValue("width", width) + " " + logValue("color", c));
}

void
CGnuPlotLogRenderer::
fillPieSlice(const CPoint2D &center, double ri, double ro, double angle1, double angle2,
             const CRGBA &c)
{
  log("fillPieSlice " + logValue("center", center) + " " + logValue("ri", ri) + " " +
      logValue("ro", ro) + " " + logValue("angle1", angle1) + " " +
      logValue("angle2", angle2) + " " + logValue("color", c));
}

void
CGnuPlotLogRenderer::
drawArc(const CPoint2D &center, double r1, double r2, double angle1, double angle2, const CRGBA &c)
{
  log("drawArc " + logValue("center", center) + " " + logValue("r1", r1) + " " +
      logValue("r2", r2) + " " + logValue("angle1", angle1) + " " +
      logValue("angle2", angle2) + " " + logValue("color", c));
}

void
CGnuPlotLogRenderer::
drawChord(const CPoint2D &center, double r, double angle1, double angle2, const CRGBA &fc,
          const CRGBA &lc)
{
  log("drawChord " + logValue("center", center) + " " + logValue("r", r) + " " +
      logValue("angle1", angle1) + " " + logValue("angle2", angle2) + " " +
      logValue("fillColor", fc) + " " + logValue("lineColor", lc));
}

void
CGnuPlotLogRenderer::
drawComplexChord(const CPoint2D &center, double r, double a11, double a12, double a21, double a22,
                 const CRGBA &fc, const CRGBA &lc)
{
  log("drawComplexChord " + logValue("center", center) + " " + logValue("r", r) + " " +
      logValue("a11", a11) + " " + logValue("a12", a12) + " " +
      logValue("a21", a21) + " " + logValue("a22", a22) + " " +
      logValue("fillColor", fc) + " " + logValue("lineColor", lc));
}

void
CGnuPlotLogRenderer::
log(const std::string &str)
{
  os() << str << std::endl;
}
