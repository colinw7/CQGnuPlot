#include <CGnuPlotPSRenderer.h>
#include <CGnuPlotPSDevice.h>
#include <CGnuPlotUtil.h>
#include <CXMLNamedChar.h>
#include <CSymbol2D.h>

class CSymbol2DPSRenderer : public CSymbol2DRenderer {
 public:
  typedef CGnuPlotTypes::SymbolType SymbolType;

 public:
  CSymbol2DPSRenderer(CGnuPlotPSRenderer *r, const CPoint2D &p,
                      double size=1.0, bool pixelSize=true) :
   r_(r), p_(p) {
    if (! pixelSize) {
      double ss = sqrt(size);

      sw_ = r_->windowWidthToPixelWidth  (ss)/2.0;
      sh_ = r_->windowHeightToPixelHeight(ss)/2.0;
    }
    else {
      sw_ = 4*size;
      sh_ = sw_;
    }

    sw_ = r_->pixelWidthToWindowWidth  (sw_);
    sh_ = r_->pixelHeightToWindowHeight(sh_);
  }

  void drawSymbolType(SymbolType type) {
    if      (type == SymbolType::POINT) {
      r_->drawPoint(p_, lc_);
    }
    else if (type == SymbolType::CIRCLE) {
      r_->drawEllipse(p_, sw_, sh_, 0, lc_, lw_, CLineDash());
    }
    else if (type == SymbolType::FILLED_CIRCLE) {
      r_->fillEllipse(p_, sw_, sh_, 0, fc_);
    }
  }

  void moveTo(double x, double y) {
    points_.clear();

    double px = r_->pixelWidthToWindowWidth  (x);
    double py = r_->pixelHeightToWindowHeight(y);

    points_.push_back(p_ + CPoint2D(px, py));
  }

  void lineTo(double x, double y) {
    double px = r_->pixelWidthToWindowWidth  (x);
    double py = r_->pixelHeightToWindowHeight(y);

    points_.push_back(p_ + CPoint2D(px, py));
  }

  void closePath() {
  }

  void setLineColor(const CRGBA &c) {
    lc_ = c; stroke_ = true;
  }

  void setLineWidth(double w) {
    lw_ = w;
  }

  void setFillColor(const CRGBA &c) {
    fc_ = c; fill_ = true;
  }

  void stroke() {
    if      (fill_ && stroke_) {
      r_->fillPolygon(points_, fc_);
      r_->drawPolygon(points_, lc_, lw_, CLineDash());
    }
    else if (fill_) {
      r_->fillPolygon(points_, fc_);
    }
    else if (stroke_) {
      r_->drawPolygon(points_, lc_, lw_, CLineDash());
    }

    str_.str("");
  }

 private:
  typedef std::vector<CPoint2D> Points;

  CGnuPlotPSRenderer *r_      { 0 };
  CPoint2D            p_;
  std::stringstream   str_;
  CRGBA               lc_, fc_;
  double              lw_     { 0 };
  bool                stroke_ { false };
  bool                fill_   { false };
  double              sw_     { 1.0 };
  double              sh_     { 1.0 };
  Points              points_;
};

CGnuPlotPSRenderer::
CGnuPlotPSRenderer(CGnuPlotPSDevice *device) :
 device_(device)
{
}

CGnuPlotPSRenderer::
~CGnuPlotPSRenderer()
{
}

void
CGnuPlotPSRenderer::
setFont(const CFontPtr &font)
{
  device_->print().setFont(font);

  CGnuPlotRenderer::setFont(font);
}

void
CGnuPlotPSRenderer::
clear(const CRGBA &)
{
  // TODO
}

void
CGnuPlotPSRenderer::
drawPoint(const CPoint2D &point, const CRGBA &c)
{
  CPoint2D ppoint;

  windowToPixel(point, ppoint);

  CPoint2D p;

  windowToPixel(point, p);

  device_->print().setForeground(c);

  device_->print().drawPoint(ppoint.x, ppoint.y);
}

void
CGnuPlotPSRenderer::
drawSymbol(const CPoint2D &point, SymbolType type, double size, const CRGBA &c,
           double lw, bool pixelSize)
{
  CSymbol2DPSRenderer r(this, point, size, pixelSize);

  r.setLineWidth(lw);

  switch (type) {
    case SymbolType::POINT: { // point
      r.setLineColor(c); r.drawSymbolType(type);
      break;
    }
    case SymbolType::PLUS: { // plus
      r.setLineColor(c); r.drawSymbol(CSymbol2D::Type::PLUS);
      break;
    }
    case SymbolType::CROSS: { // cross
      r.setLineColor(c); r.drawSymbol(CSymbol2D::Type::CROSS);
      break;
    }
    case SymbolType::STAR: { // star
      r.setLineColor(c); r.drawSymbol(CSymbol2D::Type::STAR1);
      break;
    }
    case SymbolType::BOX: { // box
      r.setLineColor(c); r.drawSymbol(CSymbol2D::Type::BOX);
      break;
    }
    case SymbolType::FILLED_BOX: { // filled box
      r.setFillColor(c); r.drawSymbol(CSymbol2D::Type::BOX);
      break;
    }
    case SymbolType::CIRCLE: { // circle
      r.setLineColor(c); r.drawSymbolType(type);
      break;
    }
    case SymbolType::FILLED_CIRCLE: { // fill circle
      r.setFillColor(c); r.drawSymbolType(type);
      break;
    }
    case SymbolType::TRIANGLE: { // triangle
      r.setLineColor(c); r.drawSymbol(CSymbol2D::Type::TRIANGLE);
      break;
    }
    case SymbolType::FILLED_TRIANGLE: { // filled triangle
      r.setFillColor(c); r.drawSymbol(CSymbol2D::Type::TRIANGLE);
      break;
    }
    case SymbolType::INV_TRIANGLE: { // inv triangle
      r.setLineColor(c); r.drawSymbol(CSymbol2D::Type::ITRIANGLE);
      break;
    }
    case SymbolType::FILLED_INV_TRIANGLE: { // filled inv triangle
      r.setFillColor(c); r.drawSymbol(CSymbol2D::Type::ITRIANGLE);
      break;
    }
    case SymbolType::DIAMOND: { // diamond
      r.setLineColor(c); r.drawSymbol(CSymbol2D::Type::DIAMOND);
      break;
    }
    case SymbolType::FILLED_DIAMOND: { // filled diamond
      r.setFillColor(c); r.drawSymbol(CSymbol2D::Type::DIAMOND);
      break;
    }
    case SymbolType::PENTAGON: { // pentagon
      r.setLineColor(c); r.drawSymbol(CSymbol2D::Type::PENTAGON);
      break;
    }
    case SymbolType::FILLED_PENTAGON: { // filled pentagon
      r.setFillColor(c); r.drawSymbol(CSymbol2D::Type::PENTAGON);
      break;
    }
    default:
      break;
  }
}

void
CGnuPlotPSRenderer::
drawPath(const std::vector<CPoint2D> &points, const CGnuPlotStroke &stroke)
{
  if (points.empty())
    return;

  std::vector<double> x, y;

  pointsToXY(points, x, y);

  device_->print().setForeground(stroke.color());

  device_->print().drawPath(&x[0], &y[0], x.size());
}

void
CGnuPlotPSRenderer::
drawLine(const CPoint2D &point1, const CPoint2D &point2, const CRGBA &c, double width,
         const CLineDash & /*dash*/)
{
  CPoint2D ppoint1, ppoint2;

  windowToPixel(point1, ppoint1);
  windowToPixel(point2, ppoint2);

  device_->print().setForeground(c);
  device_->print().setLineWidth(width);

  device_->print().drawLine(ppoint1.x, ppoint1.y, ppoint2.x, ppoint2.y);
}

void
CGnuPlotPSRenderer::
drawRect(const CBBox2D &rect, const CRGBA &c, double width, const CLineDash & /*dash*/)
{
  CBBox2D prect;

  windowToPixel(rect, prect);

  device_->print().setForeground(c);
  device_->print().setLineWidth(width);

  device_->print().drawRectangle(prect.getLL().x, prect.getLL().y,
                                 prect.getUR().x, prect.getUR().y);
}

void
CGnuPlotPSRenderer::
patternRect(const CBBox2D &rect, CGnuPlotTypes::FillPattern /*pattern*/,
            const CRGBA &fg, const CRGBA & /*bg*/)
{
  CBBox2D prect;

  windowToPixel(rect, prect);

  device_->print().setForeground(fg);

  device_->print().fillRectangle(prect.getLL().x, prect.getLL().y,
                                 prect.getUR().x, prect.getUR().y);
}

void
CGnuPlotPSRenderer::
fillRect(const CBBox2D &rect, const CRGBA &c)
{
  CBBox2D prect;

  windowToPixel(rect, prect);

  device_->print().setForeground(c);

  device_->print().fillRectangle(prect.getLL().x, prect.getLL().y,
                                 prect.getUR().x, prect.getUR().y);
}

void
CGnuPlotPSRenderer::
drawBezier(const CPoint2D &point1, const CPoint2D &point2,
           const CPoint2D &point3, const CPoint2D &point4,
           const CRGBA &c, double width, const CLineDash &dash)
{
  drawLine(point1, point2, c, width, dash);
  drawLine(point2, point3, c, width, dash);
  drawLine(point3, point4, c, width, dash);
}

void
CGnuPlotPSRenderer::
drawPolygon(const std::vector<CPoint2D> &points, const CRGBA &c, double width, const CLineDash &)
{
  if (points.empty()) return;

  device_->print().setForeground(c);
  device_->print().setLineWidth(width);

  std::vector<double> x, y;

  pointsToXY(points, x, y);

  device_->print().drawPolygon(&x[0], &y[0], x.size());
}

void
CGnuPlotPSRenderer::
fillPolygon(const std::vector<CPoint2D> &points, const CRGBA &c)
{
  if (points.empty()) return;

  device_->print().setForeground(c);

  std::vector<double> x, y;

  pointsToXY(points, x, y);

  device_->print().fillPolygon(&x[0], &y[0], x.size());
}

void
CGnuPlotPSRenderer::
patternPolygon(const std::vector<CPoint2D> &points, CGnuPlotTypes::FillPattern,
               const CRGBA &fg, const CRGBA &)
{
  // TODO: fill pattern
  if (points.empty()) return;

  device_->print().setForeground(fg);

  std::vector<double> x, y;

  pointsToXY(points, x, y);

  device_->print().fillPolygon(&x[0], &y[0], x.size());
}

void
CGnuPlotPSRenderer::
drawEllipse(const CPoint2D &center, double rx, double ry, double /*a*/, const CRGBA &c,
            double width, const CLineDash & /*dash*/)
{
  CPoint2D pcenter;

  windowToPixel(center, pcenter);

  device_->print().setForeground(c);
  device_->print().setLineWidth(width);

  device_->print().drawEllipse(pcenter.x, pcenter.y, rx, ry);
}

void
CGnuPlotPSRenderer::
fillEllipse(const CPoint2D &center, double rx, double ry, double /*a*/, const CRGBA &c)
{
  CPoint2D pcenter;

  windowToPixel(center, pcenter);

  device_->print().setForeground(c);

  device_->print().fillEllipse(pcenter.x, pcenter.y, rx, ry);
}

void
CGnuPlotPSRenderer::
patternEllipse(const CPoint2D &center, double rx, double ry, double /*a*/,
               CGnuPlotTypes::FillPattern, const CRGBA &fg, const CRGBA &)
{
  CPoint2D pcenter;

  windowToPixel(center, pcenter);

  device_->print().setForeground(fg);

  device_->print().fillEllipse(pcenter.x, pcenter.y, rx, ry);
}

void
CGnuPlotPSRenderer::
drawText(const CPoint2D &point, const std::string &text, const CRGBA &c)
{
  CPoint2D ppoint;

  windowToPixel(point, ppoint);

  device_->print().setForeground(c);

  device_->print().drawText(ppoint.x, ppoint.y, text);
}

void
CGnuPlotPSRenderer::
drawRotatedText(const CPoint2D &p, const std::string &text, double /*ta*/,
                const HAlignPos &, const VAlignPos &, const CRGBA &c)
{
  drawText(p, text, c);
}

void
CGnuPlotPSRenderer::
drawPieSlice(const CPoint2D &pc, double ri, double ro, double angle1, double angle2,
             bool /*wedge*/, const CRGBA &c, double width, const CLineDash &)
{
  CPoint2D ppc;

  windowToPixel(pc, ppc);

  // TODO: !!!!
  device_->print().setForeground(c);
  device_->print().setLineWidth(width);

  device_->print().drawArc(ppc.x, ppc.y, ro, ro, angle1, angle2);
  device_->print().drawArc(ppc.x, ppc.y, ri, ri, angle1, angle2);
}

void
CGnuPlotPSRenderer::
fillPieSlice(const CPoint2D &pc, double ri, double ro, double angle1, double angle2,
             const CRGBA &c)
{
  CPoint2D ppc;

  windowToPixel(pc, ppc);

  // TODO: !!!!
  device_->print().setForeground(c);

  device_->print().drawArc(ppc.x, ppc.y, ro, ro, angle1, angle2);
  device_->print().drawArc(ppc.x, ppc.y, ri, ri, angle1, angle2);
}

void
CGnuPlotPSRenderer::
drawArc(const CPoint2D &p, double r1, double r2, double a1, double a2, const CRGBA &c)
{
  CPoint2D pp;

  windowToPixel(p, pp);

  device_->print().setForeground(c);

  device_->print().drawArc(pp.x, pp.y, r1, r2, a1, a2);
}

void
CGnuPlotPSRenderer::
drawChord(const CPoint2D &, double, double, double, const CRGBA &, const CRGBA &)
{
  // TODO
}

void
CGnuPlotPSRenderer::
drawComplexChord(const CPoint2D &, double, double, double, double, double,
                 const CRGBA &, const CRGBA &)
{
  // TODO
}

void
CGnuPlotPSRenderer::
drawImage(const CPoint2D &, const CImagePtr &)
{
}

void
CGnuPlotPSRenderer::
pointsToXY(const std::vector<CPoint2D> &points, std::vector<double> &x, std::vector<double> &y)
{
  x.resize(points.size());
  y.resize(points.size());

  int i = 0;

  for (const auto &p : points) {
    CPoint2D pp;

    windowToPixel(p, pp);

    x[i] = pp.x;
    y[i] = pp.y;

    ++i;
  }
}

void
CGnuPlotPSRenderer::
windowToPixel(const CBBox2D &w, CBBox2D &p)
{
  CPoint2D p1, p2;

  windowToPixel(w.getLL(), p1);
  windowToPixel(w.getUR(), p2);

  p = CBBox2D(p1, p2);
}

void
CGnuPlotPSRenderer::
windowToPixel(const CPoint2D &w, CPoint2D &p)
{
  CGnuPlotRenderer::windowToPixel(w, p);

  p.y = device_->size().height - p.y;
}
