#include <CGnuPlotSVGRenderer.h>
#include <CGnuPlotSVGDevice.h>
#include <CGnuPlotUtil.h>
#include <CSymbol2D.h>

namespace {
  std::string toSVGColor(const CRGBA &c) {
    return c.getRGB().stringEncode();
  }

  std::string strokeNone() {
    std::stringstream ss;

    ss << "stroke: none;";

    return ss.str();
  }

  std::string strokeColor(const CRGBA &c) {
    std::stringstream ss;

    ss << "stroke:" << toSVGColor(c);

    if (c.getAlphaI() < 255)
      ss << "; stroke-opacity:" << c.getAlpha();

    ss << ";";

    return ss.str();
  }

  std::string fillNone() {
    std::stringstream ss;

    ss << "fill: none;";

    return ss.str();
  }

  std::string fillColor(const CRGBA &c) {
    std::stringstream ss;

    ss << "fill:" << toSVGColor(c);

    if (c.getAlphaI() < 255)
      ss << "; fill-opacity:" << c.getAlpha();

    ss << ";";

    return ss.str();
  }

  std::string strokeWidth(double w) {
    std::stringstream ss;

    ss << "stroke-width: " << std::max(w, 0.5) << ";";

    return ss.str();
  }
}

class CSymbol2DSVGRenderer : public CSymbol2DRenderer {
 public:
  typedef CGnuPlotTypes::SymbolType SymbolType;

 public:
  CSymbol2DSVGRenderer(CGnuPlotSVGRenderer *r, const CPoint2D &p, double size=1.0) :
   r_(r) {
    r_->windowToPixel(p, p_);

    ss_ = 4*size;
  }

  void drawSymbolType(SymbolType type) {
    if      (type == SymbolType::POINT) {
      r_->os() << "<ellipse cx=\"" << p_.x << "\" cy=\"" << p_.y << "\" rx=\"1\" ry=\"1\" " <<
                  "style=\"" << strokeColor(lc_) << " " << fillNone() << "\"/>\n";
    }
    else if (type == SymbolType::CIRCLE) {
      r_->os() << "<ellipse cx=\"" << p_.x << "\" cy=\"" << p_.y <<
                  "\" rx=\"" << ss_ << "\" ry=\"" << ss_ << "\" " <<
                  "style=\"" << strokeColor(lc_) << " " << fillNone() << "\"/>\n";
    }
    else if (type == SymbolType::FILLED_CIRCLE) {
      r_->os() << "<ellipse cx=\"" << p_.x << "\" cy=\"" << p_.y <<
                  "\" rx=\"" << ss_ << "\" ry=\"" << ss_ << "\" " <<
                  "style=\"" << strokeNone() << "; " << fillColor(fc_) << "\"/>\n";
    }
  }

  void moveTo(double x, double y) {
    str_ << "M " << p_.x + x << " " << p_.y + y;
  }

  void lineTo(double x, double y) {
    str_ << " L " << p_.x + x << " " << p_.y + y;
  }

  void closePath() {
    str_ << " z";
  }

  void setLineColor(const CRGBA &c) {
    lc_ = c; stroke_ = true;
  }

  void setFillColor(const CRGBA &c) {
    fc_ = c; fill_ = true;
  }

  void stroke() {
    if      (fill_ && stroke_) {
      r_->os() << "<path d=\"" << str_.str() << "\" style=\"";
      r_->os() << strokeColor(lc_) << " " << fillColor(fc_) << " ";
      r_->os() << strokeWidth(1) << "\"/>\n";
    }
    else if (fill_) {
      r_->os() << "<path d=\"" << str_.str() << "\" style=\"";
      r_->os() << strokeNone() << " " << fillColor(fc_) << "\"/>\n";
    }
    else if (stroke_) {
      r_->os() << "<path d=\"" << str_.str() << "\" style=\"";
      r_->os() << strokeColor(lc_) << " " << fillNone() << " ";
      r_->os() << strokeWidth(1) << "\"/>\n";
    }

    str_.str("");
  }

 private:
  CGnuPlotSVGRenderer *r_      { 0 };
  CPoint2D             p_;
  std::stringstream    str_;
  CRGBA                lc_, fc_;
  bool                 stroke_ { false };
  bool                 fill_   { false };
  double               ss_     { 1.0 };
};

//-----

CGnuPlotSVGRenderer::
CGnuPlotSVGRenderer(CGnuPlotSVGDevice *device) :
 device_(device)
{
}

CGnuPlotSVGRenderer::
~CGnuPlotSVGRenderer()
{
}

std::ostream &
CGnuPlotSVGRenderer::
os()
{
  return device_->os();
}

void
CGnuPlotSVGRenderer::
clear(const CRGBA &)
{
  // TODO
}

void
CGnuPlotSVGRenderer::
drawPoint(const CPoint2D &point, const CRGBA &c)
{
  CPoint2D p;

  windowToPixel(point, p);

  os() << "<ellipse cx=\"" << p.x << "\" cy=\"" << p.y << "\" rx=\"1\" ry=\"1\"" <<
               "style=\"" << strokeColor(c) << " " << fillNone() << "\"/>\n";
}

void
CGnuPlotSVGRenderer::
drawSymbol(const CPoint2D &point, SymbolType type, double size, const CRGBA &c)
{
  CSymbol2DSVGRenderer r(this, point, size);

  switch (type) {
    case SymbolType::POINT: { // point
      r.setLineColor(c); r.drawSymbolType(type);
      break;
    }
    case SymbolType::PLUS: { // plus
      r.setLineColor(c); r.drawSymbol(CSYMBOL_PLUS);
      break;
    }
    case SymbolType::CROSS: { // cross
      r.setLineColor(c); r.drawSymbol(CSYMBOL_CROSS);
      break;
    }
    case SymbolType::STAR: { // star
      r.setLineColor(c); r.drawSymbol(CSYMBOL_STAR1);
      break;
    }
    case SymbolType::BOX: { // box
      r.setLineColor(c); r.drawSymbol(CSYMBOL_BOX);
      break;
    }
    case SymbolType::FILLED_BOX: { // filled box
      r.setFillColor(c); r.drawSymbol(CSYMBOL_BOX);
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
      r.setLineColor(c); r.drawSymbol(CSYMBOL_TRIANGLE);
      break;
    }
    case SymbolType::FILLED_TRIANGLE: { // filled triangle
      r.setFillColor(c); r.drawSymbol(CSYMBOL_TRIANGLE);
      break;
    }
    case SymbolType::INV_TRIANGLE: { // inv triangle
      r.setLineColor(c); r.drawSymbol(CSYMBOL_ITRIANGLE);
      break;
    }
    case SymbolType::FILLED_INV_TRIANGLE: { // filled inv triangle
      r.setFillColor(c); r.drawSymbol(CSYMBOL_ITRIANGLE);
      break;
    }
    case SymbolType::DIAMOND: { // diamond
      r.setLineColor(c); r.drawSymbol(CSYMBOL_DIAMOND);
      break;
    }
    case SymbolType::FILLED_DIAMOND: { // filled diamond
      r.setFillColor(c); r.drawSymbol(CSYMBOL_DIAMOND);
      break;
    }
    default:
      break;
  }
}

void
CGnuPlotSVGRenderer::
drawPath(const std::vector<CPoint2D> &points, double width, const CRGBA &c,
         const CLineDash & /*dash*/)
{
  os() << "<path d=\"";

  for (uint i = 0; i < points.size(); ++i) {
    CPoint2D p;

    windowToPixel(points[i], p);

    if (i == 0)
      os() << "M " << p.x << " " << p.y;
    else
      os() << " L " << p.x << " " << p.y;
  }

  os() << "\" " << "style=\"" << fillNone() << " " << strokeColor(c) << " " <<
          strokeWidth(width) << "\"/>\n";
}

void
CGnuPlotSVGRenderer::
drawLine(const CPoint2D &point1, const CPoint2D &point2, double width, const CRGBA &c,
         const CLineDash & /*dash*/)
{
  CPoint2D p1, p2;

  windowToPixel(point1, p1);
  windowToPixel(point2, p2);

  os() << "<path d=\"M " << p1.x << " " << p1.y << " L " << p2.x << " " << p2.y << "\" " <<
          "style=\"" << fillNone() << " " << strokeColor(c) << " " <<
          strokeWidth(width) << "\"/>\n";
}

void
CGnuPlotSVGRenderer::
drawRect(const CBBox2D &rect, const CRGBA &c, double width)
{
  double px1, py1, px2, py2;

  windowToPixel(rect.getXMin(), rect.getYMin(), &px1, &py1);
  windowToPixel(rect.getXMax(), rect.getYMax(), &px2, &py2);

  CBBox2D rect1(std::min(px1, px2), std::min(py1, py2), std::max(px1, px2), std::max(py1, py2));

  os() << "<rect x=\"" << rect1.getXMin() << "\" y=\"" << rect1.getYMin() << "\" " <<
          "width=\"" << rect1.getWidth() << "\" height=\"" << rect1.getHeight() << "\" " <<
          "style=\"" << fillNone() << " " << strokeColor(c) << " " <<
          strokeWidth(width) << "\"/>\n";
}

void
CGnuPlotSVGRenderer::
patternRect(const CBBox2D &rect, CGnuPlotTypes::FillPattern /*pattern*/,
            const CRGBA &fg, const CRGBA & /*bg*/)
{
  double px1, py1, px2, py2;

  windowToPixel(rect.getXMin(), rect.getYMin(), &px1, &py1);
  windowToPixel(rect.getXMax(), rect.getYMax(), &px2, &py2);

  CBBox2D rect1(std::min(px1, px2), std::min(py1, py2), std::max(px1, px2), std::max(py1, py2));

  os() << "<rect x=\"" << rect1.getXMin() << "\" y=\"" << rect1.getYMin() << "\" " <<
          "width=\"" << rect1.getWidth() << "\" height=\"" << rect1.getHeight() << "\" " <<
          "style=\"" << fillNone() << " " << strokeColor(fg) << "\"/>\n";
}

void
CGnuPlotSVGRenderer::
fillRect(const CBBox2D &rect, const CRGBA &c)
{
  double px1, py1, px2, py2;

  windowToPixel(rect.getXMin(), rect.getYMin(), &px1, &py1);
  windowToPixel(rect.getXMax(), rect.getYMax(), &px2, &py2);

  CBBox2D rect1(std::min(px1, px2), std::min(py1, py2), std::max(px1, px2), std::max(py1, py2));

  os() << "<rect x=\"" << rect1.getXMin() << "\" y=\"" << rect1.getYMin() << "\" " <<
          "width=\"" << rect1.getWidth() << "\" height=\"" << rect1.getHeight() << "\" " <<
          "style=\"" << fillColor(c) << " " << strokeNone() << "\"/>\n";
}

void
CGnuPlotSVGRenderer::
drawBezier(const CPoint2D &point1, const CPoint2D &point2,
           const CPoint2D &point3, const CPoint2D &point4,
           double width, const CRGBA &c)
{
  CPoint2D p1, p2, p3, p4;

  windowToPixel(point1, p1);
  windowToPixel(point2, p2);
  windowToPixel(point3, p3);
  windowToPixel(point4, p4);

  os() << "<path d=\"M " << p1.x << " " << p1.y << "C " << p2.x << " " << p2.y << " " <<
          p3.x << " " << p3.y << " " << p4.x << " " << p4.y << "\" " <<
          "style=\"" << fillNone() << " " << strokeColor(c) << " " <<
          strokeWidth(width) << "\"/>\n";
}

void
CGnuPlotSVGRenderer::
drawPolygon(const std::vector<CPoint2D> &points, double width, const CRGBA &c)
{
  if (points.empty()) return;

  CPoint2D p;

  windowToPixel(points[0], p);

  os() << "<path d=\"M " << p.x << " " << p.y;

  for (uint i = 1; i < points.size(); ++i) {
    windowToPixel(points[i], p);

    os() << " L " << p.x << " " << p.y;
  }

  os() << "z\" style=\"" << fillNone() << " " << strokeColor(c) << " " <<
          strokeWidth(width) << "\"/>\n";
}

void
CGnuPlotSVGRenderer::
fillPolygon(const std::vector<CPoint2D> &points, const CRGBA &c)
{
  if (points.empty()) return;

  CPoint2D p;

  windowToPixel(points[0], p);

  os() << "<path d=\"M " << p.x << " " << p.y;

  for (uint i = 1; i < points.size(); ++i) {
    windowToPixel(points[i], p);

    os() << " L " << p.x << " " << p.y;
  }

  os() << "z\" style=\"" << fillColor(c) << " " << strokeNone() << "\"/>\n";
}

void
CGnuPlotSVGRenderer::
drawEllipse(const CPoint2D &center, double rx, double ry, double a, const CRGBA &c, double width)
{
  double px1, py1, px2, py2;

  windowToPixel(center.x - rx, center.y - ry, &px1, &py1);
  windowToPixel(center.x + rx, center.y + ry, &px2, &py2);

  CBBox2D rect1(std::min(px1, px2), std::min(py1, py2), std::max(px1, px2), std::max(py1, py2));

  os() << "<ellipse cx=\"" << rect1.getCenter().x << "\" cy=\"" << rect1.getCenter().y << "\" " <<
          "rx=\"" << rect1.getWidth()/2 << "\" ry=\"" << rect1.getHeight()/2 << "\" " <<
          "transform=\"rotate(" << a << ")\" " <<
          "style=\"" << strokeColor(c) << " " << strokeWidth(width) <<
          " " << fillNone() << "\"/>\n";
}

void
CGnuPlotSVGRenderer::
fillEllipse(const CPoint2D &center, double rx, double ry, double a, const CRGBA &c)
{
  double px1, py1, px2, py2;

  windowToPixel(center.x - rx, center.y - ry, &px1, &py1);
  windowToPixel(center.x + rx, center.y + ry, &px2, &py2);

  CBBox2D rect1(std::min(px1, px2), std::min(py1, py2), std::max(px1, px2), std::max(py1, py2));

  os() << "<ellipse cx=\"" << rect1.getCenter().x << "\" cy=\"" << rect1.getCenter().y << "\" " <<
          "rx=\"" << rect1.getWidth()/2 << "\" ry=\"" << rect1.getHeight()/2 << "\" " <<
          "transform=\"rotate(" << a << ")\" " <<
          "style=\"" << strokeNone() << " " << fillColor(c) << "\"/>\n";
}

void
CGnuPlotSVGRenderer::
drawText(const CPoint2D &point, const std::string &text, const CRGBA &c)
{
  double a = getFont()->getAngle();

  CPoint2D p;

  windowToPixel(point, p);

  os() << "<g transform=\"translate(" << p.x << "," << p.y << ") rotate(" << -a << ")\" "
           "style=\"stroke:none; " << fillColor(c) << " " <<
           "font-family:" << getFont()->getFamily() << "; " <<
           "font-size:" << getFont()->getSize() << "; " <<
           "text-anchor:start\">\n";
  os() << "<text>" << text << "</text>\n";
  os() << "</g>\n";
}

void
CGnuPlotSVGRenderer::
drawPieSlice(const CPoint2D &pc, double r, double angle1, double angle2, const CRGBA &c)
{
  double x1, y1, x2, y2;

  windowToPixel(pc.x - r, pc.y - r, &x1, &y1);
  windowToPixel(pc.x + r, pc.y + r, &x2, &y2);

  double dangle = angle2 - angle1;

  // TODO: !!!!
  os() << "<path d=\"M " << x1 << " " << y1;
  os() << " L " << dangle << " " << dangle;
  os() << " L " << x2 << " " << y2;

  os() << "z\" style=\"" << fillColor(c) << " " << strokeNone() << "\"/>\n";
}
