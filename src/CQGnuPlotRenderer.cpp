#include <CQGnuPlotRenderer.h>
#include <CQGnuPlotWindow.h>
#include <CQGnuPlotPlot.h>
#include <CQGnuPlotCanvas.h>
#include <CQGnuPlotUtil.h>
#include <CQRotatedText.h>
#include <CGnuPlotUtil.h>
#include <CSymbol2D.h>

#include <CQFont.h>
#include <CQFontUtil.h>
#include <CQImageUtil.h>
#include <CQUtil.h>
#include <CQUtilLineDash.h>
#include <CQUtilAlignType.h>
#include <CFontMgr.h>
#include <CImageLib.h>

#include <QPainter>
#include <QPainterPath>

class CQSymbol2DRenderer : public CSymbol2DRenderer {
 public:
  typedef CGnuPlotTypes::SymbolType SymbolType;

 public:
  CQSymbol2DRenderer(CQGnuPlotRenderer *r, const CPoint2D &p,
                     double size=1.0, bool pixelSize=true) :
   r_(r) {
    r_->windowToPixel(p.x, p.y, &px_, &py_);

    if (! pixelSize) {
      double ss = sqrt(size);

      sw_ = r_->windowWidthToPixelWidth  (ss)/2.0;
      sh_ = r_->windowHeightToPixelHeight(ss)/2.0;
    }
    else {
      sw_ = 4*size;
      sh_ = sw_;
    }
  }

  void drawSymbolType(SymbolType type) {
    if      (type == SymbolType::POINT) {
      r_->painter()->setPen(pen_);
      r_->painter()->drawPoint(QPointF(px_, py_));
    }
    else if (type == SymbolType::CIRCLE) {
      path_.addEllipse(QRectF(px_ - sw_, py_ - sh_, 2*sw_, 2*sh_));
      r_->painter()->strokePath(path_, pen_);
    }
    else if (type == SymbolType::FILLED_CIRCLE) {
      path_.addEllipse(QRectF(px_ - sw_, py_ - sh_, 2*sw_, 2*sh_));
      r_->painter()->fillPath(path_, brush_);
    }
  }

  void setStroke(const QPen   &pen  ) { pen_   = pen  ; stroke_ = true; }
  void setFill  (const QBrush &brush) { brush_ = brush; fill_   = true; }

  void setLineWidth(double w) { pen_.setWidthF(w); }

  void moveTo(double x, double y) override {
    path_.moveTo(QPointF(px_ + sw_*x, py_ + sh_*y));
  }

  void lineTo(double x, double y) override {
    path_.lineTo(QPointF(px_ + sw_*x, py_ + sh_*y));
  }

  void closePath() override {
    path_.closeSubpath();
  }

  void stroke() override {
    //if (fill_)
    //  r_->painter()->fillPath(path_, brush_);

    if (stroke_)
      r_->painter()->strokePath(path_, pen_);
  }

  void fill() override {
    if (fill_)
      r_->painter()->fillPath(path_, brush_);
  }

  void strokeCircle(double x, double y, double r) override {
    QRectF rect(px_ + (x - r)*sw_, py_ + (y - r)*sw_, 2*r*sw_, 2*r*sw_);

    r_->painter()->save();

    r_->painter()->setBrush(Qt::NoBrush);
    r_->painter()->setPen  (pen_);

    r_->painter()->drawEllipse(rect);

    r_->painter()->restore();
  }

  void fillCircle(double x, double y, double r) override {
    QRectF rect(px_ + (x - r)*sw_, py_ + (y - r)*sw_, 2*r*sw_, 2*r*sw_);

    r_->painter()->save();

    r_->painter()->setBrush(brush_);
    r_->painter()->setPen  (Qt::NoPen);

    r_->painter()->drawEllipse(rect);

    r_->painter()->restore();
  }

  void setPen  (const QPen   &p) { pen_   = p; stroke_ = true; }
  void setBrush(const QBrush &b) { brush_ = b; fill_   = true; }

 private:
  CQGnuPlotRenderer *r_;
  double             px_     { 0.0 };
  double             py_     { 0.0 };
  double             sw_     { 1.0 };
  double             sh_     { 1.0 };
  QPainterPath       path_;
  QPen               pen_;
  QBrush             brush_;
  bool               stroke_ { false };
  bool               fill_   { false };
};

//-----

CQGnuPlotRenderer::
CQGnuPlotRenderer(CQGnuPlotCanvas *canvas) :
 canvas_(canvas)
{
}

CQGnuPlotRenderer::
~CQGnuPlotRenderer()
{
}

void
CQGnuPlotRenderer::
setCanvas(CQGnuPlotCanvas *canvas)
{
  canvas_ = canvas;
}

void
CQGnuPlotRenderer::
setPainter(QPainter *painter)
{
  painter_ = painter;

  setAntiAlias(true);

  if (canvas_)
    window()->setSize(CISize2D(canvas_->width(), canvas_->height()));
}

void
CQGnuPlotRenderer::
setAntiAlias(bool b)
{
  painter_->setRenderHint(QPainter::Antialiasing, b);
}

void
CQGnuPlotRenderer::
clear(const CRGBA &c)
{
  if (canvas_)
    painter_->fillRect(canvas_->rect(), QBrush(toQColor(c)));
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
drawSymbol(const CPoint2D &point, SymbolType type, double size, const CRGBA &c,
           double lw, bool pixelSize)
{
  double size1 = (size > 0 ? size : 1);

  CQSymbol2DRenderer r(this, point, size1, pixelSize);

  QPen   pen  (toQColor(c));
  QBrush brush(toQColor(c));

  pen.setStyle(Qt::SolidLine);

  pen.setWidthF(lw);

  switch (type) {
    case SymbolType::POINT: { // point
      r.setPen(pen); r.drawSymbolType(type);
      break;
    }
    case SymbolType::PLUS: { // plus
      r.setPen(pen); r.drawSymbol(CSymbol2D::Type::PLUS);
      break;
    }
    case SymbolType::CROSS: { // cross
      r.setPen(pen); r.drawSymbol(CSymbol2D::Type::CROSS);
      break;
    }
    case SymbolType::STAR: { // star
      r.setPen(pen); r.drawSymbol(CSymbol2D::Type::STAR1);
      break;
    }
    case SymbolType::BOX: { // box
      r.setPen(pen); r.drawSymbol(CSymbol2D::Type::BOX);
      break;
    }
    case SymbolType::FILLED_BOX: { // filled box
      r.setBrush(brush); r.drawSymbol(CSymbol2D::Type::BOX);
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
      r.setPen(pen); r.drawSymbol(CSymbol2D::Type::TRIANGLE);
      break;
    }
    case SymbolType::FILLED_TRIANGLE: { // filled triangle
      r.setBrush(brush); r.drawSymbol(CSymbol2D::Type::TRIANGLE);
      break;
    }
    case SymbolType::INV_TRIANGLE: { // inv triangle
      r.setPen(pen); r.drawSymbol(CSymbol2D::Type::ITRIANGLE);
      break;
    }
    case SymbolType::FILLED_INV_TRIANGLE: { // filled inv triangle
      r.setBrush(brush); r.drawSymbol(CSymbol2D::Type::ITRIANGLE);
      break;
    }
    case SymbolType::DIAMOND: { // diamond
      r.setPen(pen); r.drawSymbol(CSymbol2D::Type::DIAMOND);
      break;
    }
    case SymbolType::FILLED_DIAMOND: { // filled diamond
      r.setBrush(brush); r.drawSymbol(CSymbol2D::Type::DIAMOND);
      break;
    }
    case SymbolType::PENTAGON: { // pentagon
      r.setPen(pen); r.drawSymbol(CSymbol2D::Type::PENTAGON);
      break;
    }
    case SymbolType::FILLED_PENTAGON: { // filled pentagon
      r.setBrush(brush); r.drawSymbol(CSymbol2D::Type::PENTAGON);
      break;
    }
    default:
      break;
  }
}

void
CQGnuPlotRenderer::
drawPath(const std::vector<CPoint2D> &points, const CGnuPlotStroke &stroke)
{
  if (points.empty()) return;

  QPainterPath path;

  double px, py;

  windowToPixel(points[0].x, points[0].y, &px, &py);

  path.moveTo(px, py);

  for (uint i = 1; i < points.size(); ++i) {
    windowToPixel(points[i].x, points[i].y, &px, &py);

    path.lineTo(px, py);
  }

  QPen pen = painter_->pen();

  CQUtil::penSetLineDash(pen, stroke.lineDash());

  pen.setWidthF   (stroke.width());
  pen.setColor    (toQColor(stroke.color()));
  pen.setCapStyle (toPenCapStyle(stroke.lineCap()));
  pen.setJoinStyle(toPenJoinStyle(stroke.lineJoin()));

  painter_->strokePath(path, pen);
}

void
CQGnuPlotRenderer::
drawPixelLine(const CPoint2D &point1, const CPoint2D &point2, const CRGBA &c, double width,
              const CLineDash &dash)
{
  QPen pen = painter_->pen();

  pen.setWidthF(width);
  pen.setColor(toQColor(c));

  CQUtil::penSetLineDash(pen, dash);

  painter_->setPen(pen);

  painter_->drawLine(point1.x, point1.y, point2.x, point2.y);
}

void
CQGnuPlotRenderer::
drawLine(const CPoint2D &point1, const CPoint2D &point2, const CRGBA &c, double width,
         const CLineDash &dash)
{
  QPen pen = painter_->pen();

  pen.setWidthF(width);
  pen.setColor(toQColor(c));

  CQUtil::penSetLineDash(pen, dash);

  painter_->setPen(pen);

  double px1, py1, px2, py2;

  windowToPixel(point1.x, point1.y, &px1, &py1);
  windowToPixel(point2.x, point2.y, &px2, &py2);

  painter_->drawLine(px1, py1, px2, py2);
}

void
CQGnuPlotRenderer::
drawRect(const CBBox2D &rect, const CRGBA &c, double width, const CLineDash &dash)
{
  QPen pen = painter_->pen();

  pen.setWidthF(width);
  pen.setColor(toQColor(c));

  CQUtil::penSetLineDash(pen, dash);

  painter_->setPen(pen);

  double px1, py1, px2, py2;

  windowToPixel(rect.getXMin(), rect.getYMin(), &px1, &py1);
  windowToPixel(rect.getXMax(), rect.getYMax(), &px2, &py2);

  QRectF qrect(std::min(px1, px2), std::min(py1, py2), fabs(px2 - px1), fabs(py2 - py1));

  painter_->drawRect(qrect);
}

void
CQGnuPlotRenderer::
patternRect(const CBBox2D &rect, CGnuPlotTypes::FillPattern pattern,
            const CRGBA &fg, const CRGBA &bg)
{
  double px1, py1, px2, py2;

  windowToPixel(rect.getXMin(), rect.getYMin(), &px1, &py1);
  windowToPixel(rect.getXMax(), rect.getYMax(), &px2, &py2);

  CRGBA c = (pattern == CGnuPlotTypes::FillPattern::BG ? bg : fg);

  Qt::BrushStyle qpattern = CQGnuPlotUtil::fillPatternQtConv(pattern);

  QBrush b(toQColor(c), qpattern);

  QRectF qrect(std::min(px1, px2), std::min(py1, py2), fabs(px2 - px1), fabs(py2 - py1));

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

  QRectF qrect(std::min(px1, px2), std::min(py1, py2), fabs(px2 - px1), fabs(py2 - py1));

  painter_->fillRect(qrect, b);
}

void
CQGnuPlotRenderer::
drawBezier(const CPoint2D &point1, const CPoint2D &point2,
           const CPoint2D &point3, const CPoint2D &point4,
           const CRGBA &c, double width, const CLineDash &dash)
{
  double px1, py1, px2, py2, px3, py3, px4, py4;

  windowToPixel(point1.x, point1.y, &px1, &py1);
  windowToPixel(point2.x, point2.y, &px2, &py2);
  windowToPixel(point3.x, point3.y, &px3, &py3);
  windowToPixel(point4.x, point4.y, &px4, &py4);

  QPainterPath path;

  path.moveTo(px1, py1);
  path.cubicTo(px2, py2, px3, py3, px4, py4);

  QPen pen(toQColor(c));

  pen.setWidthF(width);

  CQUtil::penSetLineDash(pen, dash);

  painter_->strokePath(path, pen);
}

void
CQGnuPlotRenderer::
drawPolygon(const std::vector<CPoint2D> &points, const CRGBA &c, double w, const CLineDash &dash)
{
  if (points.empty()) return;

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

  CQUtil::penSetLineDash(pen, dash);

  painter_->strokePath(path, pen);
}

void
CQGnuPlotRenderer::
fillPolygon(const std::vector<CPoint2D> &points, const CRGBA &c)
{
  if (points.empty()) return;

  QPainterPath path;

  double px, py;

  windowToPixel(points[0].x, points[0].y, &px, &py);

  path.moveTo(QPointF(px, py));

  for (uint i = 1; i < points.size(); ++i) {
    windowToPixel(points[i].x, points[i].y, &px, &py);

    path.lineTo(QPointF(px, py));
  }

  path.closeSubpath();

  QBrush brush(toQColor(c));

  painter_->fillPath(path, brush);

  // TODO: why need extra border to overwrite aliased edge
  QPen pen(toQColor(c));

  painter_->strokePath(path, pen);
}

void
CQGnuPlotRenderer::
patternPolygon(const std::vector<CPoint2D> &points, CGnuPlotTypes::FillPattern pattern,
               const CRGBA &fg, const CRGBA &bg)
{
  if (points.empty()) return;

  QPainterPath path;

  double px, py;

  windowToPixel(points[0].x, points[0].y, &px, &py);

  path.moveTo(px, py);

  for (uint i = 1; i < points.size(); ++i) {
    windowToPixel(points[i].x, points[i].y, &px, &py);

    path.lineTo(px, py);
  }

  path.closeSubpath();

  CRGBA c = (pattern == CGnuPlotTypes::FillPattern::BG ? bg : fg);

  Qt::BrushStyle qpattern = CQGnuPlotUtil::fillPatternQtConv(pattern);

  QBrush b(toQColor(c), qpattern);

  painter_->fillPath(path, b);
}

void
CQGnuPlotRenderer::
drawEllipse(const CPoint2D &center, double rx, double ry, double angle, const CRGBA &c,
            double width, const CLineDash &dash)
{
  double px1, py1, px2, py2;

  windowToPixel(center.x - rx, center.y - ry, &px1, &py1);
  windowToPixel(center.x + rx, center.y + ry, &px2, &py2);

  QRectF rect(std::min(px1, px2), std::min(py1, py2), fabs(px2 - px1), fabs(py2 - py1));

  QPainterPath path;

  path.addEllipse(rect);

  auto m = painter_->worldTransform();

  painter_->setWorldTransform(QTransform());

  QTransform t1, t2, t3;

  double px = (px1 + px2)/2.0;
  double py = (py1 + py2)/2.0;

  t1.translate(px, py); t2.rotate(-angle); t3.translate(-px, -py);

  painter_->setWorldTransform(t3*t2*t1);

  QPen pen(toQColor(c));

  pen.setWidthF(width);

  CQUtil::penSetLineDash(pen, dash);

  painter_->strokePath(path, pen);

  painter_->setWorldTransform(m);
}

void
CQGnuPlotRenderer::
fillEllipse(const CPoint2D &center, double rx, double ry, double angle, const CRGBA &c)
{
  double px1, py1, px2, py2;

  windowToPixel(center.x - rx, center.y - ry, &px1, &py1);
  windowToPixel(center.x + rx, center.y + ry, &px2, &py2);

  QRectF rect(std::min(px1, px2), std::min(py1, py2), fabs(px2 - px1), fabs(py2 - py1));

  QPainterPath path;

  path.addEllipse(rect);

  QTransform m = painter_->worldTransform();

  painter_->setWorldTransform(QTransform());

  QTransform t1, t2, t3;

  double px = (px1 + px2)/2.0;
  double py = (py1 + py2)/2.0;

  t1.translate(px, py); t2.rotate(-angle); t3.translate(-px, -py);

  painter_->setWorldTransform(t3*t2*t1);

  painter_->fillPath(path, QBrush(toQColor(c)));

  painter_->setWorldTransform(m);
}

void
CQGnuPlotRenderer::
patternEllipse(const CPoint2D &center, double rx, double ry, double angle,
               CGnuPlotTypes::FillPattern pattern, const CRGBA &fg, const CRGBA &bg)
{
  double px1, py1, px2, py2;

  windowToPixel(center.x - rx, center.y - ry, &px1, &py1);
  windowToPixel(center.x + rx, center.y + ry, &px2, &py2);

  QRectF rect(std::min(px1, px2), std::min(py1, py2), fabs(px2 - px1), fabs(py2 - py1));

  QPainterPath path;

  path.addEllipse(rect);

  QTransform m = painter_->worldTransform();

  painter_->setWorldTransform(QTransform());

  QTransform t1, t2, t3;

  double px = (px1 + px2)/2.0;
  double py = (py1 + py2)/2.0;

  t1.translate(px, py); t2.rotate(-angle); t3.translate(-px, -py);

  painter_->setWorldTransform(t3*t2*t1);

  CRGBA c = (pattern == CGnuPlotTypes::FillPattern::BG ? bg : fg);

  Qt::BrushStyle qpattern = CQGnuPlotUtil::fillPatternQtConv(pattern);

  QBrush b(toQColor(c), qpattern);

  painter_->fillPath(path, b);

  painter_->setWorldTransform(m);
}

void
CQGnuPlotRenderer::
drawText(const CPoint2D &point, const std::string &text, const CRGBA &c)
{
  auto *qfont = dynamic_cast<CQFont *>(getFont().get());

  painter_->setFont(qfont->getQFont());

  double px, py;

  windowToPixel(point.x, point.y, &px, &py);

  //painter_->drawText(px, py, text.c_str());

  QTransform m = painter_->worldTransform();

  painter_->setWorldTransform(QTransform());

  QTransform t1, t2, t3;

  t1.translate(px, py); t2.rotate(-qfont->getAngle()); t3.translate(-px, -py);

  painter_->setWorldTransform(t3*t2*t1);

  painter_->setPen(toQColor(c));

  painter_->drawText(px, py, text.c_str());

  painter_->setWorldTransform(m);
}

void
CQGnuPlotRenderer::
drawRotatedText(const CPoint2D &p, const std::string &text, double ta,
                const HAlignPos &halignPos, const VAlignPos &valignPos, const CRGBA &tc)
{
  auto *qfont = dynamic_cast<CQFont *>(getFont().get());

  painter_->setFont(qfont->getQFont());

  double px, py;

  windowToPixel(p.x, p.y, &px, &py);

  Qt::Alignment qalign = CQUtil::toQAlign(halignPos.first) | CQUtil::toQAlign(valignPos.first);

  painter_->setPen(toQColor(tc));

  //CQRotatedText::drawRotatedText(painter_, px, py, text.c_str(), -ta, qalign);

  //---

  painter_->save();

  QFontMetrics fm(painter_->font());

  int th = fm.height();
  int tw = fm.horizontalAdvance(text.c_str());

  double dx { 0.0 };

  if      (qalign & Qt::AlignLeft)
    dx = 0;
  else if (qalign & Qt::AlignRight)
    dx = -tw;
  else if (qalign & Qt::AlignHCenter)
    dx = -tw/2.0;

  double dy { 0.0 };

  if      (qalign & Qt::AlignBottom)
    dy = 0;
  else if (qalign & Qt::AlignTop)
    dy = th;
  else if (qalign & Qt::AlignVCenter)
    dy = th/2.0;

  double angle = -ta;

  double a1 = M_PI*angle/180.0;

  double c = cos(a1);
  double s = sin(a1);

  double tx = c*dx - s*dy;
  double ty = s*dx + c*dy;

  QTransform t;

  t.translate(px + tx, py + ty);
  t.rotate(angle);
  //t.translate(0, -fm.descent());

  painter_->setTransform(t);

  painter_->drawText(0, 0, text.c_str());

  painter_->restore();
}

void
CQGnuPlotRenderer::
fillPieSlice(const CPoint2D &pc, double ri, double ro, double angle1, double angle2,
             const CRGBA &c)
{
  QPainterPath path;

  double x1, y1, x2, y2;

  windowToPixel(pc.x - ro, pc.y + ro, &x1, &y1);
  windowToPixel(pc.x + ro, pc.y - ro, &x2, &y2);

  QRectF orect(x1, y1, x2 - x1, y2 - y1);

  double dangle = angle2 - angle1;

  if (ri <= 0) {
    path.moveTo(QPointF((x1 + x2)/2, (y1 + y2)/2));

    path.arcTo(orect, angle1, dangle);

    path.closeSubpath();
  }
  else {
    windowToPixel(pc.x - ri, pc.y + ri, &x1, &y1);
    windowToPixel(pc.x + ri, pc.y - ri, &x2, &y2);

    QRectF irect(x1, y1, x2 - x1, y2 - y1);

    path.arcMoveTo(irect, angle1);

    path.arcTo(irect, angle1, dangle);
    path.arcTo(orect, angle2, -dangle);

    path.closeSubpath();
  }

  painter_->fillPath(path, QBrush(toQColor(c)));
}

void
CQGnuPlotRenderer::
drawPieSlice(const CPoint2D &pc, double ri, double ro, double angle1, double angle2,
             bool wedge, const CRGBA &c, double w, const CLineDash &dash)
{
  QPainterPath path;

  double x1, y1, x2, y2;

  windowToPixel(pc.x - ro, pc.y + ro, &x1, &y1);
  windowToPixel(pc.x + ro, pc.y - ro, &x2, &y2);

  QRectF orect(x1, y1, x2 - x1, y2 - y1);

  double dangle = angle2 - angle1;

  if (ri <= 0) {
    if (wedge) {
      path.moveTo(QPointF((x1 + x2)/2, (y1 + y2)/2));

      path.arcTo(orect, angle1, dangle);

      path.closeSubpath();
    }
    else {
      path.arcMoveTo(orect, angle1);
      path.arcTo(orect, angle1, dangle);
    }
  }
  else {
    windowToPixel(pc.x - ri, pc.y + ri, &x1, &y1);
    windowToPixel(pc.x + ri, pc.y - ri, &x2, &y2);

    QRectF irect(x1, y1, x2 - x1, y2 - y1);

    path.arcMoveTo(irect, angle1);

    path.arcTo(irect, angle1, dangle);
    path.arcTo(orect, angle2, -dangle);

    path.closeSubpath();
  }

  QPen pen(toQColor(c));

  pen.setWidthF(w);

  CQUtil::penSetLineDash(pen, dash);

  painter_->strokePath(path, pen);
}

void
CQGnuPlotRenderer::
drawArc(const CPoint2D &p, double r1, double r2, double a1, double a2, const CRGBA &c)
{
  double px11, py11, px21, py21;
  double px12, py12, px22, py22;

  windowToPixel(p.x - r1, p.y - r1, &px11, &py11);
  windowToPixel(p.x + r1, p.y + r1, &px21, &py21);
  windowToPixel(p.x - r2, p.y - r2, &px12, &py12);
  windowToPixel(p.x + r2, p.y + r2, &px22, &py22);

  QRectF qr1(px11, py21, px21 - px11, py11 - py21);
  QRectF qr2(px12, py22, px22 - px12, py12 - py22);

  QPainterPath path;

  path.arcMoveTo(qr1, a1);

  double da = a2 - a1;

  path.arcTo(qr1, a1,  da);
  path.arcTo(qr2, a2, -da);

  path.closeSubpath();

  painter_->setPen  (QColor(0, 0, 0));
  painter_->setBrush(toQColor(c));

  painter_->drawPath(path);
}

void
CQGnuPlotRenderer::
drawChord(const CPoint2D &p, double r, double a1, double a2, const CRGBA &fc, const CRGBA &lc)
{
  double px1, py1, px2, py2;

  windowToPixel(p.x - r, p.y - r, &px1, &py1);
  windowToPixel(p.x + r, p.y + r, &px2, &py2);

  QRectF qr(px1, py2, px2 - px1, py1 - py2);

  QPainterPath path;

  path.arcMoveTo(qr, a1); QPointF p1 = path.currentPosition();
  path.arcMoveTo(qr, a2); QPointF p2 = path.currentPosition();

  QPointF p3 = qr.center();

  path.moveTo(p1);
  path.quadTo(p3, p2);
  path.arcTo(qr, a2, a1 - a2);

  path.closeSubpath();

  painter_->setPen  (toQColor(lc));
  painter_->setBrush(toQColor(fc));

  painter_->drawPath(path);
}

void
CQGnuPlotRenderer::
drawComplexChord(const CPoint2D &p, double r, double a11, double a12,
                 double a21, double a22, const CRGBA &fc, const CRGBA &lc)
{
  double px1, py1, px2, py2;

  windowToPixel(p.x - r, p.y - r, &px1, &py1);
  windowToPixel(p.x + r, p.y + r, &px2, &py2);

  QRectF qr(px1, py2, px2 - px1, py1 - py2);

  QPainterPath path;

  path.arcMoveTo(qr, a11); QPointF p1 = path.currentPosition();
  path.arcMoveTo(qr, a12); QPointF p2 = path.currentPosition();
  path.arcMoveTo(qr, a21); //QPointF p3 = path.currentPosition();
  path.arcMoveTo(qr, a22); QPointF p4 = path.currentPosition();

  QPointF p5 = qr.center();

  path.moveTo(p1);
  path.quadTo(p5, p4);
  path.arcTo(qr, a22, a21 - a22);
  path.quadTo(p5, p2);
  path.arcTo(qr, a12, a11 - a12);

  path.closeSubpath();

  painter_->setPen  (toQColor(lc));
  painter_->setBrush(toQColor(fc));

  painter_->drawPath(path);
}

void
CQGnuPlotRenderer::
drawImage(const CPoint2D &p, const CImagePtr &image)
{
  double px, py;

  windowToPixel(p.x, p.y, &px, &py);

  painter_->drawImage(px, py, CQImageUtil::toQImage(image));
}

void
CQGnuPlotRenderer::
setFont(const CFontPtr &font)
{
  CGnuPlotRenderer::setFont(font);

  painter_->setFont(CQFontUtil::toQFont(getFont()));
}
