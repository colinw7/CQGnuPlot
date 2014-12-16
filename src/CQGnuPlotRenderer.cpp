#include <CQGnuPlotRenderer.h>
#include <CQGnuPlotWindow.h>
#include <CQGnuPlotPlot.h>
#include <CQGnuPlotCanvas.h>

#include <CQFont.h>
#include <CQUtil.h>
#include <CFontMgr.h>
#include <QPainter>

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

  painter_->drawPoint(QPoint(px, py));
}

void
CQGnuPlotRenderer::
drawSymbol(const CPoint2D &point, CGnuPlot::SymbolType type, double size, const CRGBA &c)
{
  QPen   pen  (toQColor(c));
  QBrush brush(toQColor(c));

  painter_->setPen(pen);

  double px, py;

  windowToPixel(point.x, point.y, &px, &py);

  double ss = 4*size;

  switch (type) {
    case CGnuPlot::SYMBOL_POINT: { // point
      painter_->drawPoint(QPointF(px, py));
      break;
    }
    case CGnuPlot::SYMBOL_PLUS: { // plus
      painter_->drawLine(QPointF(px - ss, py     ), QPointF(px + ss, py     ));
      painter_->drawLine(QPointF(px     , py - ss), QPointF(px     , py + ss));
      break;
    }
    case CGnuPlot::SYMBOL_CROSS: { // cross
      painter_->drawLine(QPointF(px - ss, py - ss), QPointF(px + ss, py + ss));
      painter_->drawLine(QPointF(px - ss, py + ss), QPointF(px + ss, py - ss));
      break;
    }
    case CGnuPlot::SYMBOL_STAR: { // star
      painter_->drawLine(QPointF(px - ss, py     ), QPointF(px + ss, py     ));
      painter_->drawLine(QPointF(px     , py - ss), QPointF(px     , py + ss));
      painter_->drawLine(QPointF(px - ss, py - ss), QPointF(px + ss, py + ss));
      painter_->drawLine(QPointF(px - ss, py + ss), QPointF(px + ss, py - ss));
      break;
    }
    case CGnuPlot::SYMBOL_BOX: { // box
      QPainterPath path;
      path.moveTo(QPointF(px - ss, py - ss));
      path.lineTo(QPointF(px + ss, py - ss));
      path.lineTo(QPointF(px + ss, py + ss));
      path.lineTo(QPointF(px - ss, py + ss));
      path.closeSubpath();
      painter_->strokePath(path, pen);
      break;
    }
    case CGnuPlot::SYMBOL_FILLED_BOX: { // filled box
      QPainterPath path;
      path.moveTo(QPointF(px - ss, py - ss));
      path.lineTo(QPointF(px + ss, py - ss));
      path.lineTo(QPointF(px + ss, py + ss));
      path.lineTo(QPointF(px - ss, py + ss));
      path.closeSubpath();
      painter_->fillPath(path, brush);
      break;
    }
    case CGnuPlot::SYMBOL_CIRCLE: { // circle
      QPainterPath path;
      path.addEllipse(QRectF(px - ss, py - ss, 2*ss, 2*ss));
      painter_->strokePath(path, pen);
      break;
    }
    case CGnuPlot::SYMBOL_FILLED_CIRCLE: { // fill circle
      QPainterPath path;
      path.addEllipse(QRectF(px - ss, py - ss, 2*ss, 2*ss));
      painter_->fillPath(path, brush);
      break;
    }
    case CGnuPlot::SYMBOL_TRIANGLE: { // triangle
      QPainterPath path;
      path.moveTo(QPointF(px     , py - ss));
      path.lineTo(QPointF(px + ss, py + ss));
      path.lineTo(QPointF(px - ss, py + ss));
      path.closeSubpath();
      painter_->strokePath(path, pen);
      break;
    }
    case CGnuPlot::SYMBOL_FILLED_TRIANGLE: { // filled triangle
      QPainterPath path;
      path.moveTo(QPointF(px     , py - ss));
      path.lineTo(QPointF(px + ss, py + ss));
      path.lineTo(QPointF(px - ss, py + ss));
      path.closeSubpath();
      painter_->fillPath(path, brush);
      break;
    }
    case CGnuPlot::SYMBOL_INV_TRIANGLE: { // inv triangle
      QPainterPath path;
      path.moveTo(QPointF(px     , py + ss));
      path.lineTo(QPointF(px + ss, py - ss));
      path.lineTo(QPointF(px - ss, py - ss));
      path.closeSubpath();
      painter_->strokePath(path, pen);
      break;
    }
    case CGnuPlot::SYMBOL_FILLED_INV_TRIANGLE: { // filled inv triangle
      QPainterPath path;
      path.moveTo(QPointF(px     , py + ss));
      path.lineTo(QPointF(px + ss, py - ss));
      path.lineTo(QPointF(px - ss, py - ss));
      path.closeSubpath();
      painter_->fillPath(path, brush);
      break;
    }
    case CGnuPlot::SYMBOL_DIAMOND: { // diamond
      QPainterPath path;
      path.moveTo(QPointF(px - ss, py     ));
      path.lineTo(QPointF(px     , py - ss));
      path.lineTo(QPointF(px + ss, py     ));
      path.lineTo(QPointF(px     , py + ss));
      path.closeSubpath();
      painter_->strokePath(path, pen);
      break;
    }
    case CGnuPlot::SYMBOL_FILLED_DIAMOND: { // filled diamond
      QPainterPath path;
      path.moveTo(QPointF(px - ss, py     ));
      path.lineTo(QPointF(px     , py - ss));
      path.lineTo(QPointF(px + ss, py     ));
      path.lineTo(QPointF(px     , py + ss));
      path.closeSubpath();
      painter_->fillPath(path, brush);
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
drawRect(const CBBox2D &rect, const CRGBA &c)
{
  double px1, py1, px2, py2;

  windowToPixel(rect.getXMin(), rect.getYMin(), &px1, &py1);
  windowToPixel(rect.getXMax(), rect.getYMax(), &px2, &py2);

  QPen pen(toQColor(c));

  painter_->setPen(pen);

  painter_->drawRect(QRectF(px1, py1, px2 - px1, py2 - py1));
}

void
CQGnuPlotRenderer::
patternRect(const CBBox2D &rect, CGnuPlot::PatternType pattern, const CRGBA &fg, const CRGBA &bg)
{
  CRGBA c = fg;

  Qt::BrushStyle qpattern;

  switch (pattern) {
    case CGnuPlot::PATTERN_NONE  : qpattern = Qt::NoBrush         ; break;
    case CGnuPlot::PATTERN_HATCH : qpattern = Qt::DiagCrossPattern; break;
    case CGnuPlot::PATTERN_DENSE : qpattern = Qt::Dense2Pattern   ; break;
    case CGnuPlot::PATTERN_FG    : qpattern = Qt::SolidPattern    ; break;
    case CGnuPlot::PATTERN_FDIAG : qpattern = Qt::BDiagPattern    ; break;
    case CGnuPlot::PATTERN_BDIAG : qpattern = Qt::FDiagPattern    ; break;
    case CGnuPlot::PATTERN_FDIAG1: qpattern = Qt::HorPattern      ; break;
    case CGnuPlot::PATTERN_BDIAG1: qpattern = Qt::VerPattern      ; break;
    case CGnuPlot::PATTERN_BG    : qpattern = Qt::SolidPattern    ; c = bg; break;
    default                      : qpattern = Qt::NoBrush         ; break;
  }

  double px1, py1, px2, py2;

  windowToPixel(rect.getXMin(), rect.getYMin(), &px1, &py1);
  windowToPixel(rect.getXMax(), rect.getYMax(), &px2, &py2);

  QBrush b(toQColor(c), qpattern);

  painter_->fillRect(QRectF(px1, py1, px2 - px1, py2 - py1), b);
}

void
CQGnuPlotRenderer::
fillRect(const CBBox2D &rect, const CRGBA &c)
{
  double px1, py1, px2, py2;

  windowToPixel(rect.getXMin(), rect.getYMin(), &px1, &py1);
  windowToPixel(rect.getXMax(), rect.getYMax(), &px2, &py2);

  QBrush b(toQColor(c));

  painter_->fillRect(QRectF(px1, py1, px2 - px1, py2 - py1), b);
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

  double pxmin = region_.getLeft  ()*pw_;
  double pymin = region_.getBottom()*ph_;
  double pxmax = region_.getRight ()*pw_;
  double pymax = region_.getTop   ()*ph_;

  double pw = pxmax - pxmin;
  double ph = pymax - pymin;

  double lmargin = pw*margin_.left  ()/100.0;
  double rmargin = pw*margin_.right ()/100.0;
  double tmargin = ph*margin_.top   ()/100.0;
  double bmargin = ph*margin_.bottom()/100.0;

  pxmin += lmargin; pxmax -= rmargin;
  pymin += tmargin; pymax -= bmargin;

  double xmin = range_.getXMin();
  double ymin = range_.getYMin();
  double xmax = range_.getXMax();
  double ymax = range_.getYMax();

  *px = ((wx - xmin)/(xmax - xmin))*(pxmax - pxmin) + pxmin;
  *py = ((wy - ymax)/(ymin - ymax))*(pymax - pymin) + pymin;
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

  double pxmin = region_.getLeft  ()*pw_;
  double pymin = region_.getBottom()*ph_;
  double pxmax = region_.getRight ()*pw_;
  double pymax = region_.getTop   ()*ph_;

  double pw = pxmax - pxmin;
  double ph = pymax - pymin;

  double lmargin = pw*margin_.left  ()/100.0;
  double rmargin = pw*margin_.right ()/100.0;
  double tmargin = ph*margin_.top   ()/100.0;
  double bmargin = ph*margin_.bottom()/100.0;

  pxmin -= lmargin; pxmax -= rmargin;
  pymin -= tmargin; pymax -= bmargin;

  double xmin = range_.getXMin();
  double ymin = range_.getYMin();
  double xmax = range_.getXMax();
  double ymax = range_.getYMax();

  *wx = (px - pxmin)*(xmax - xmin)/(pxmax - pxmin) + xmin;
  *wy = (py - pymin)*(ymin - ymax)/(pymax - pymin) + ymax;
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
