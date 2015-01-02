#include <CQGnuPlotRenderer.h>
#include <CQGnuPlotWindow.h>
#include <CQGnuPlotPlot.h>
#include <CQGnuPlotCanvas.h>
#include <CQGnuPlotUtil.h>
#include <CGnuPlotUtil.h>
#include <CSymbol2D.h>

#include <CQFont.h>
#include <CQUtil.h>
#include <CFontMgr.h>
#include <QPainter>

class CQSymbol2DRenderer : public CSymbol2DRenderer {
 public:
  typedef CGnuPlotTypes::SymbolType SymbolType;

 public:
  CQSymbol2DRenderer(CQGnuPlotRenderer *r, const CPoint2D &p, double size=1.0) :
   r_(r), stroke_(false), fill_(false) {
    r_->windowToPixel(p.x, p.y, &px_, &py_);

    ss_ = 4*size;
  }

  void drawSymbolType(SymbolType type) {
    if      (type == SymbolType::POINT) {
      r_->painter()->setPen(pen_);
      r_->painter()->drawPoint(QPointF(px_, py_));
    }
    else if (type == SymbolType::CIRCLE) {
      path_.addEllipse(QRectF(px_ - ss_, py_ - ss_, 2*ss_, 2*ss_));
      r_->painter()->strokePath(path_, pen_);
    }
    else if (type == SymbolType::FILLED_CIRCLE) {
      path_.addEllipse(QRectF(px_ - ss_, py_ - ss_, 2*ss_, 2*ss_));
      r_->painter()->fillPath(path_, brush_);
    }
  }

  void setStroke(const QPen   &pen  ) { pen_   = pen  ; stroke_ = true; }
  void setFill  (const QBrush &brush) { brush_ = brush; fill_   = true; }

  void moveTo(double x, double y) {
    path_.moveTo(QPointF(px_ + ss_*x, py_ - ss_*y));
  }

  void lineTo(double x, double y) {
    path_.lineTo(QPointF(px_ + ss_*x, py_ - ss_*y));
  }

  void closePath() {
    path_.closeSubpath();
  }

  void stroke() {
    if (fill_)
      r_->painter()->fillPath(path_, brush_);

    if (stroke_)
      r_->painter()->strokePath(path_, pen_);
  }

  void setPen  (const QPen   &p) { pen_   = p; stroke_ = true; }
  void setBrush(const QBrush &b) { brush_ = b; fill_   = true; }

 private:
  CQGnuPlotRenderer *r_;
  double             px_, py_;
  double             ss_;
  QPainterPath       path_;
  QPen               pen_;
  QBrush             brush_;
  bool               stroke_;
  bool               fill_;
};

CQGnuPlotRenderer::
CQGnuPlotRenderer(CQGnuPlotCanvas *canvas) :
 canvas_(canvas), pw_(1), ph_(1), mapping_(true)
{
  pw_ = canvas_->width ();
  ph_ = canvas_->height();

  font_ = CFontMgrInst->lookupFont("helvetica", CFONT_STYLE_NORMAL, 12);
}

CQGnuPlotRenderer::
~CQGnuPlotRenderer()
{
}

void
CQGnuPlotRenderer::
setPainter(QPainter *painter)
{
  painter_ = painter;

  pw_ = canvas_->width ();
  ph_ = canvas_->height();
}

void
CQGnuPlotRenderer::
drawPoint(const CPoint2D &point, const CRGBA &c)
{
  double px, py;

  windowToPixel(point.x, point.y, &px, &py);

  painter_->setPen(toQColor(c));

  painter_->drawPoint(QPointF(px, py));
}

void
CQGnuPlotRenderer::
drawSymbol(const CPoint2D &point, SymbolType type, double size, const CRGBA &c)
{
  CQSymbol2DRenderer r(this, point, size);

  QPen   pen  (toQColor(c));
  QBrush brush(toQColor(c));

  switch (type) {
    case SymbolType::POINT: { // point
      r.setPen(pen); r.drawSymbolType(type);
      break;
    }
    case SymbolType::PLUS: { // plus
      r.setPen(pen); r.drawSymbol(CSYMBOL_PLUS);
      break;
    }
    case SymbolType::CROSS: { // cross
      r.setPen(pen); r.drawSymbol(CSYMBOL_CROSS);
      break;
    }
    case SymbolType::STAR: { // star
      r.setPen(pen); r.drawSymbol(CSYMBOL_STAR1);
      break;
    }
    case SymbolType::BOX: { // box
      r.setPen(pen); r.drawSymbol(CSYMBOL_BOX);
      break;
    }
    case SymbolType::FILLED_BOX: { // filled box
      r.setBrush(brush); r.drawSymbol(CSYMBOL_BOX);
      break;
    }
    case SymbolType::CIRCLE: { // circle
      r.setPen(pen); r.drawSymbolType(type);
      break;
    }
    case SymbolType::FILLED_CIRCLE: { // fill circle
      r.setBrush(brush); r.drawSymbolType(type);
      break;
    }
    case SymbolType::TRIANGLE: { // triangle
      r.setPen(pen); r.drawSymbol(CSYMBOL_TRIANGLE);
      break;
    }
    case SymbolType::FILLED_TRIANGLE: { // filled triangle
      r.setBrush(brush); r.drawSymbol(CSYMBOL_TRIANGLE);
      break;
    }
    case SymbolType::INV_TRIANGLE: { // inv triangle
      r.setPen(pen); r.drawSymbol(CSYMBOL_ITRIANGLE);
      break;
    }
    case SymbolType::FILLED_INV_TRIANGLE: { // filled inv triangle
      r.setBrush(brush); r.drawSymbol(CSYMBOL_ITRIANGLE);
      break;
    }
    case SymbolType::DIAMOND: { // diamond
      r.setPen(pen); r.drawSymbol(CSYMBOL_DIAMOND);
      break;
    }
    case SymbolType::FILLED_DIAMOND: { // filled diamond
      r.setBrush(brush); r.drawSymbol(CSYMBOL_DIAMOND);
      break;
    }
    default:
      break;
  }
}

void
CQGnuPlotRenderer::
drawLine(const CPoint2D &point1, const CPoint2D &point2, double width, const CRGBA &c)
{
  QPen p = painter_->pen();

  p.setWidthF(width);
  p.setColor(toQColor(c));

  painter_->setPen(p);

  double px1, py1, px2, py2;

  windowToPixel(point1.x, point1.y, &px1, &py1);
  windowToPixel(point2.x, point2.y, &px2, &py2);

  painter_->drawLine(px1, py1, px2, py2);
}

void
CQGnuPlotRenderer::
drawRect(const CBBox2D &rect, const CRGBA &c, double width)
{
  QPen p = painter_->pen();

  p.setWidthF(width);
  p.setColor(toQColor(c));

  painter_->setPen(p);

  double px1, py1, px2, py2;

  windowToPixel(rect.getXMin(), rect.getYMin(), &px1, &py1);
  windowToPixel(rect.getXMax(), rect.getYMax(), &px2, &py2);

  QRectF qrect(std::min(px1, px2), std::min(py1, py2), abs(px2 - px1), abs(py2 - py1));

  painter_->drawRect(qrect);
}

void
CQGnuPlotRenderer::
patternRect(const CBBox2D &rect, CGnuPlot::FillPattern pattern, const CRGBA &fg, const CRGBA &bg)
{
  CRGBA c = (pattern == CGnuPlot::FillPattern::BG ? bg : fg);

  Qt::BrushStyle qpattern = CQGnuPlotUtil::fillPatternQtConv(pattern);

  double px1, py1, px2, py2;

  windowToPixel(rect.getXMin(), rect.getYMin(), &px1, &py1);
  windowToPixel(rect.getXMax(), rect.getYMax(), &px2, &py2);

  QBrush b(toQColor(c), qpattern);

  QRectF qrect(std::min(px1, px2), std::min(py1, py2), abs(px2 - px1), abs(py2 - py1));

  painter_->fillRect(qrect, b);
}

void
CQGnuPlotRenderer::
fillRect(const CBBox2D &rect, const CRGBA &c)
{
  double px1, py1, px2, py2;

  windowToPixel(rect.getXMin(), rect.getYMin(), &px1, &py1);
  windowToPixel(rect.getXMax(), rect.getYMax(), &px2, &py2);

  QBrush b(toQColor(c));

  QRectF qrect(std::min(px1, px2), std::min(py1, py2), abs(px2 - px1), abs(py2 - py1));

  painter_->fillRect(qrect, b);
}

void
CQGnuPlotRenderer::
drawBezier(const CPoint2D &point1, const CPoint2D &point2,
           const CPoint2D &point3, const CPoint2D &point4)
{
  double px1, py1, px2, py2, px3, py3, px4, py4;

  windowToPixel(point1.x, point1.y, &px1, &py1);
  windowToPixel(point2.x, point2.y, &px2, &py2);
  windowToPixel(point3.x, point3.y, &px3, &py3);
  windowToPixel(point4.x, point4.y, &px4, &py4);

  QPainterPath path;

  path.moveTo(px1, py1);
  path.cubicTo(px2, py2, px3, py3, px4, py4);

  painter_->drawPath(path);
}

void
CQGnuPlotRenderer::
drawPolygon(const std::vector<CPoint2D> &points, double w, const CRGBA &c)
{
  QPainterPath path;

  double px, py;

  windowToPixel(points[0].x, points[0].y, &px, &py);

  path.moveTo(px, py);

  for (uint i = 1; i < points.size(); ++i) {
    windowToPixel(points[i].x, points[i].y, &px, &py);

    path.lineTo(px, py);
  }

  path.closeSubpath();

  QPen pen(toQColor(c));

  pen.setWidthF(w);

  painter_->strokePath(path, pen);
}

void
CQGnuPlotRenderer::
fillPolygon(const std::vector<CPoint2D> &points, const CRGBA &c)
{
  QPainterPath path;

  double px, py;

  windowToPixel(points[0].x, points[0].y, &px, &py);

  path.moveTo(px, py);

  for (uint i = 1; i < points.size(); ++i) {
    windowToPixel(points[i].x, points[i].y, &px, &py);

    path.lineTo(px, py);
  }

  path.closeSubpath();

  QBrush brush(toQColor(c));

  painter_->fillPath(path, brush);
}

void
CQGnuPlotRenderer::
drawEllipse(const CPoint2D &center, double rx, double ry, const CRGBA &c)
{
  double px1, py1, px2, py2;

  windowToPixel(center.x - rx, center.y - ry, &px1, &py1);
  windowToPixel(center.x + rx, center.y + ry, &px2, &py2);

  QRectF rect(std::min(px1, px2), std::min(py1, py2), abs(px2 - px1), abs(py2 - py1));

  QPainterPath path;

  path.addEllipse(rect);

  painter_->strokePath(path, QPen(toQColor(c)));
}

void
CQGnuPlotRenderer::
fillEllipse(const CPoint2D &center, double rx, double ry, const CRGBA &c)
{
  double px1, py1, px2, py2;

  windowToPixel(center.x - rx, center.y - ry, &px1, &py1);
  windowToPixel(center.x + rx, center.y + ry, &px2, &py2);

  QRectF rect(std::min(px1, px2), std::min(py1, py2), abs(px2 - px1), abs(py2 - py1));

  QPainterPath path;

  path.addEllipse(rect);

  painter_->fillPath(path, QBrush(toQColor(c)));
}

void
CQGnuPlotRenderer::
drawText(const CPoint2D &point, const std::string &text, const CRGBA &c)
{
  CQFont *qfont = font_.cast<CQFont>();

  painter_->setFont(qfont->getQFont());

  double px, py;

  windowToPixel(point.x, point.y, &px, &py);

  //painter_->drawText(px, py, text.c_str());

  QMatrix m = painter_->worldMatrix();

  painter_->setWorldMatrix(QMatrix());

  QTransform t1, t2, t3;

  t1.translate(px, py);
  t2.rotate   (-qfont->getAngle());
  t3.translate(-px, -py);

  painter_->setWorldTransform(t3*t2*t1);

  painter_->setPen(toQColor(c));

  painter_->drawText(px, py, text.c_str());

  painter_->setWorldMatrix(m);
}

void
CQGnuPlotRenderer::
setFont(CFontPtr font)
{
  font_ = font;

  painter_->setFont(CQUtil::toQFont(font_));
}

void
CQGnuPlotRenderer::
setLineDash(const CLineDash &dash)
{
  QPen pen = painter_->pen();

  double width = pen.widthF();

  if (width <= 0) width = 1.0;

  int num = dash.getNumLengths();

  if (num > 0) {
    pen.setStyle(Qt::CustomDashLine);

    pen.setDashOffset(dash.getOffset());

    QVector<qreal> dashes;

    for (int i = 0; i < num; ++i)
      dashes << dash.getLength(i)/width;

    if (num & 1)
      dashes << dash.getLength(0)/width;

    pen.setDashPattern(dashes);
  }
  else
    pen.setStyle(Qt::SolidLine);

  painter_->setPen(pen);
}

void
CQGnuPlotRenderer::
windowToPixel(double wx, double wy, double *px, double *py)
{
  if (! mapping_) {
    *px = wx;
    *py = wy;
    return;
  }

  // place on screen
  double pxmin = region_.getLeft  ()*pw_;
  double pymin = region_.getBottom()*ph_;
  double pxmax = region_.getRight ()*pw_;
  double pymax = region_.getTop   ()*ph_;

  double pw = pxmax - pxmin;
  double ph = pymax - pymin;

  // add margin
  double ps = std::min(pw, ph);

  double lmargin = ps*margin_.left  ()/100.0;
  double rmargin = ps*margin_.right ()/100.0;
  double tmargin = ps*margin_.top   ()/100.0;
  double bmargin = ps*margin_.bottom()/100.0;

  pxmin += lmargin; pxmax -= rmargin;
  pymin += tmargin; pymax -= bmargin;

  // adjust for ratio
  if (ratio_.isValid() && ratio_.getValue() > 0.0) {
    double ratio = ratio_.getValue();

    pw = pxmax - pxmin;
    ph = pymax - pymin;

    double pr = (1.0*pw)/ph;

    double pw1 = pw*ratio/pr;
    double ph1 = ph;

    if (pw1 > pw) {
      ph1 *= pw/pw1;
      pw1  = pw;
    }

    double pxc = (pxmin + pxmax)/2.0;
    double pyc = (pymin + pymax)/2.0;

    pxmin = pxc - pw1/2.0; pxmax = pxc + pw1/2.0;
    pymin = pyc - ph1/2.0; pymax = pyc + ph1/2.0;
  }

  // map
  *px = CGnuPlotUtil::map(wx, range_.getXMin(), range_.getXMax(), pxmin, pxmax);
  *py = CGnuPlotUtil::map(wy, range_.getYMin(), range_.getYMax(), pymax, pymin);
}

void
CQGnuPlotRenderer::
pixelToWindow(double px, double py, double *wx, double *wy)
{
  if (! mapping_) {
    *wx = px;
    *wy = py;
    return;
  }

  // place on screen
  double pxmin = region_.getLeft  ()*pw_;
  double pymin = region_.getBottom()*ph_;
  double pxmax = region_.getRight ()*pw_;
  double pymax = region_.getTop   ()*ph_;

  double pw = pxmax - pxmin;
  double ph = pymax - pymin;

  // add margin
  double ps = std::min(pw, ph);

  double lmargin = ps*margin_.left  ()/100.0;
  double rmargin = ps*margin_.right ()/100.0;
  double tmargin = ps*margin_.top   ()/100.0;
  double bmargin = ps*margin_.bottom()/100.0;

  pxmin += lmargin; pxmax -= rmargin;
  pymin += tmargin; pymax -= bmargin;

  // adjust for ratio
  if (ratio_.isValid() && ratio_.getValue() > 0.0) {
    double ratio = ratio_.getValue();

    pw = pxmax - pxmin;
    ph = pymax - pymin;

    double pr = (1.0*pw)/ph;

    double pw1 = pw*ratio/pr;
    double ph1 = ph;

    if (pw1 > pw) {
      ph1 *= pw/pw1;
      pw1  = pw;
    }

    double pxc = (pxmin + pxmax)/2.0;
    double pyc = (pymin + pymax)/2.0;

    pxmin = pxc - pw1/2.0; pxmax = pxc + pw1/2.0;
    pymin = pyc - ph1/2.0; pymax = pyc + ph1/2.0;
  }

  // map
  *wx = CGnuPlotUtil::map(px, pxmin, pxmax, range_.getXMin(), range_.getXMax());
  *wy = CGnuPlotUtil::map(py, pymax, pymin, range_.getYMin(), range_.getYMax());
}

double
CQGnuPlotRenderer::
pixelWidthToWindowWidth(double w)
{
  double wx1, wy1, wx2, wy2;

  pixelToWindow(0, 0, &wx1, &wy1);
  pixelToWindow(w, w, &wx2, &wy2);

  return wx2 - wx1;
}

double
CQGnuPlotRenderer::
pixelHeightToWindowHeight(double h)
{
  double wx1, wy1, wx2, wy2;

  pixelToWindow(0, 0, &wx1, &wy1);
  pixelToWindow(h, h, &wx2, &wy2);

  return wy1 - wy2;
}

double
CQGnuPlotRenderer::
windowWidthToPixelWidth(double w)
{
  double wx1, wy1, wx2, wy2;

  windowToPixel(0, 0, &wx1, &wy1);
  windowToPixel(w, w, &wx2, &wy2);

  return fabs(wx2 - wx1);
}

double
CQGnuPlotRenderer::
windowHeightToPixelHeight(double h)
{
  double wx1, wy1, wx2, wy2;

  windowToPixel(0, 0, &wx1, &wy1);
  windowToPixel(h, h, &wx2, &wy2);

  return fabs(wy2 - wy1);
}
