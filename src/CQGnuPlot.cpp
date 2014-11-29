#include <CQApp.h>
#include <CQUtil.h>
#include <CQFont.h>
#include <CQZoomMouseMode.h>
#include <CQPanMouseMode.h>

#include <CQGnuPlot.h>
#include <CGnuPlot.h>
#include <CFontMgr.h>

#include <QPainter>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QMenuBar>
#include <QStatusBar>

enum CQGnuPlotSymbol {
  SYMBOL_NONE,
  SYMBOL_POINT,
  SYMBOL_PLUS,
  SYMBOL_CROSS,
  SYMBOL_STAR,
  SYMBOL_BOX,
  SYMBOL_FILLED_BOX,
  SYMBOL_CIRCLE,
  SYMBOL_FILLED_CIRCLE,
  SYMBOL_TRIANGLE,
  SYMBOL_FILLED_TRIANGLE,
  SYMBOL_DIAMOND,
  SYMBOL_FILLED_DIAMOND
};

static QColor colors[] = {
  QColor("black"),
  QColor("red"),
  QColor("green"),
  QColor("blue"),
  QColor("magenta"),
  QColor("cyan"),
  QColor("yellow"),
  QColor("orange"),
  QColor("grey")
};

class CQGnuPlotRenderer : public CGnuPlotRenderer {
 public:
  CQGnuPlotRenderer(CQGnuPlotWindow *window);
 ~CQGnuPlotRenderer();

  void setPainter(QPainter *painter);

  void drawPoint  (const CPoint2D &p);
  void drawSymbol (const CPoint2D &p, int type, double size);
  void drawLine   (const CPoint2D &p1, const CPoint2D &p2, double width);
  void drawRect   (const CBBox2D &rect);
  void patternRect(const CBBox2D &rect, int pattern);
  void fillRect   (const CBBox2D &rect, const CGnuPlot::Color &color);
  void drawBezier (const CPoint2D &p1, const CPoint2D &p2, const CPoint2D &p3, const CPoint2D &p4);

  void drawPolygon(const std::vector<CPoint2D> &points, double w, bool fill);

  void drawText(const CPoint2D &p, const std::string &text);

  void windowToPixel(double x, double y, double *px, double *py);
  void pixelToWindow(double px, double py, double *wx, double *wy);

  double pixelWidthToWindowWidth  (double p);
  double pixelHeightToWindowHeight(double p);

  double windowWidthToPixelWidth  (double w);
  double windowHeightToPixelHeight(double w);

  void setMapping(bool b) { mapping_ = b; }

  CFontPtr getFont() const { return font_; }

  void setFont(CFontPtr font);

 private:
  CQGnuPlotWindow *window_;
  QPainter        *painter_;
  CFontPtr         font_;
  int              pw_, ph_;
  double           xmin_, xmax_;
  double           ymin_, ymax_;
  bool             mapping_;
};

int
main(int argc, char **argv)
{
  CQApp app(argc, argv);

  bool debug  = false;
  bool edebug = false;

  std::vector<std::string> files;

  for (int i = 1; i < argc; i++) {
    if (argv[i][0] == '-') {
      if      (argv[i][1] == 'd')
        debug = true;
      else if (argv[i][1] == 'D') {
        debug  = true;
        edebug = true;
      }
    }
    else
      files.push_back(argv[i]);
  }

  CQGnuPlot plot;

  plot.setDebug(debug);
  plot.setExprDebug(edebug);

  uint num_files = files.size();

  for (uint i = 0; i < num_files; i++)
    plot.load(files[i]);

  plot.loop();

  return 0;
}

//------

CQGnuPlot::
CQGnuPlot()
{
}

CQGnuPlot::
~CQGnuPlot()
{
}

CGnuPlotWindow *
CQGnuPlot::
createWindow()
{
  CQGnuPlotWindow *window = new CQGnuPlotWindow(this);

  window->resize(600, 600);

  window->show();

  return window;
}

void
CQGnuPlot::
timeout()
{
  qApp->processEvents();
}

void
CQGnuPlot::
load(const std::string &filename)
{
  CGnuPlot::load(filename);
}

//------

uint CQGnuPlotWindow::lastId;

CQGnuPlotWindow::
CQGnuPlotWindow(CQGnuPlot *plot) :
 QMainWindow(), CGnuPlotWindow(plot), id_(++lastId), plot_(plot)
{
  canvas_ = new CQGnuPlotCanvas(this);

  renderer_ = new CQGnuPlotRenderer(this);

  setRenderer(renderer_);

  setCentralWidget(canvas_);

  //----

  zoomMode_ = new CQZoomMouseMode(this, this);
  panMode_  = new CQPanMouseMode (this, this);

  //----

  QMenu *fileMenu = menuBar()->addMenu("&File");
  QMenu *dispMenu = menuBar()->addMenu("&Display");
  QMenu *modeMenu = menuBar()->addMenu("&Mode");
  QMenu *helpMenu = menuBar()->addMenu("&Help");

  QAction *closeAction = new QAction("&Close", this);

  connect(closeAction, SIGNAL(triggered()), this, SLOT(close()));

  fileMenu->addAction(closeAction);

  //--------

  CQMouseModeMgrInst->createMenuItems(modeMenu);

  //--------

  QAction *xAxisAction = new QAction("&X Axis", this);
  QAction *yAxisAction = new QAction("&Y Axis", this);

  xAxisAction->setCheckable(true);
  xAxisAction->setChecked  (true);
  yAxisAction->setCheckable(true);
  yAxisAction->setChecked  (true);

  connect(xAxisAction, SIGNAL(triggered(bool)), this, SLOT(xAxisSlot(bool)));
  connect(yAxisAction, SIGNAL(triggered(bool)), this, SLOT(yAxisSlot(bool)));

  dispMenu->addAction(xAxisAction);
  dispMenu->addAction(yAxisAction);

  QAction *helpAction = new QAction("&Help", this);

  helpMenu->addAction(helpAction);

  //---

  showPos(0, 0);
}

CQGnuPlotWindow::
~CQGnuPlotWindow()
{
}

void
CQGnuPlotWindow::
stateChanged(CGnuPlot::ChangeState)
{
  redraw();
}

void
CQGnuPlotWindow::
redraw()
{
  canvas_->update();
}

void
CQGnuPlotWindow::
redrawOverlay()
{
  redraw();
}

QPointF
CQGnuPlotWindow::
pixelToWindow(const QPoint &p)
{
  return QPointF(p.x(), p.y());
}

void
CQGnuPlotWindow::
setCursor(QCursor cursor)
{
  canvas_->setCursor(cursor);
}

// Pan
void
CQGnuPlotWindow::
panBy(double, double)
{
}

void
CQGnuPlotWindow::
panTo(const CPoint2D &)
{
}

void
CQGnuPlotWindow::
panLeft()
{
}

void
CQGnuPlotWindow::
panRight()
{
}

void
CQGnuPlotWindow::
panUp()
{
}

void
CQGnuPlotWindow::
panDown()
{
}

void
CQGnuPlotWindow::
resetPan()
{
}

// Zoom
void
CQGnuPlotWindow::
zoomTo(const CBBox2D &)
{
}

void
CQGnuPlotWindow::
zoomIn(const CPoint2D &)
{
}

void
CQGnuPlotWindow::
zoomOut(const CPoint2D &)
{
}

void
CQGnuPlotWindow::
zoomIn()
{
}

void
CQGnuPlotWindow::
zoomOut()
{
}

void
CQGnuPlotWindow::
resetZoom()
{
}

void
CQGnuPlotWindow::
xAxisSlot(bool checked)
{
  showXAxis(checked);

  update();
}

void
CQGnuPlotWindow::
yAxisSlot(bool checked)
{
  showYAxis(checked);

  update();
}

void
CQGnuPlotWindow::
paint(QPainter *p)
{
  renderer_->setPainter(p);

  p->setRenderHint(QPainter::Antialiasing);
  p->setRenderHint(QPainter::TextAntialiasing);

  p->fillRect(canvas_->rect(), QBrush(QColor(255,255,255)));

  p->setPen(QColor(0,0,0));

  draw();
}

void
CQGnuPlotWindow::
showPos(double wx, double wy)
{
  statusBar()->showMessage(QString("%1 %2").arg(wx).arg(wy));
}

//-----------

CQGnuPlotRenderer::
CQGnuPlotRenderer(CQGnuPlotWindow *window) :
 window_(window), pw_(1), ph_(1), xmin_(0), xmax_(1), ymin_(0), ymax_(1), mapping_(true)
{
  pw_ = window_->canvas()->width ();
  ph_ = window_->canvas()->height();

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

  pw_ = window_->canvas()->width ();
  ph_ = window_->canvas()->height();

  window_->getDisplayRange(&xmin_, &ymin_, &xmax_, &ymax_);
}

void
CQGnuPlotRenderer::
drawPoint(const CPoint2D &point)
{
  double px, py;

  windowToPixel(point.x, point.y, &px, &py);

  painter_->drawPoint(QPoint(px, py));
}

void
CQGnuPlotRenderer::
drawSymbol(const CPoint2D &point, int type, double size)
{
  QPen   pen  (QColor(0,0,0));
  QBrush brush(QColor(0,0,0));

  double px, py;

  windowToPixel(point.x, point.y, &px, &py);

  int ss = 4*size;

  int type1 = type % 14;

  switch (type1) {
    case 0: { // point
      painter_->drawPoint(QPointF(px, py));
      break;
    }
    case 1: { // plus
      painter_->drawLine(QPointF(px - ss, py     ), QPointF(px + ss, py     ));
      painter_->drawLine(QPointF(px     , py - ss), QPointF(px     , py + ss));
      break;
    }
    case 2: { // cross
      painter_->drawLine(QPointF(px - ss, py - ss), QPointF(px + ss, py + ss));
      painter_->drawLine(QPointF(px - ss, py + ss), QPointF(px + ss, py - ss));
      break;
    }
    case 3: { // star
      painter_->drawLine(QPointF(px - ss, py     ), QPointF(px + ss, py     ));
      painter_->drawLine(QPointF(px     , py - ss), QPointF(px     , py + ss));
      painter_->drawLine(QPointF(px - ss, py - ss), QPointF(px + ss, py + ss));
      painter_->drawLine(QPointF(px - ss, py + ss), QPointF(px + ss, py - ss));
      break;
    }
    case 4: { // box
      QPainterPath path;
      path.moveTo(QPointF(px - ss, py - ss));
      path.lineTo(QPointF(px + ss, py - ss));
      path.lineTo(QPointF(px + ss, py + ss));
      path.lineTo(QPointF(px - ss, py + ss));
      path.closeSubpath();
      painter_->strokePath(path, pen);
      break;
    }
    case 5: { // filled box
      QPainterPath path;
      path.moveTo(QPointF(px - ss, py - ss));
      path.lineTo(QPointF(px + ss, py - ss));
      path.lineTo(QPointF(px + ss, py + ss));
      path.lineTo(QPointF(px - ss, py + ss));
      path.closeSubpath();
      painter_->fillPath(path, brush);
      break;
    }
    case 6: { // circle
      QPainterPath path;
      path.addEllipse(QRectF(px - ss, py - ss, 2*ss, 2*ss));
      painter_->strokePath(path, pen);
      break;
    }
    case 7: { // fill circle
      QPainterPath path;
      path.addEllipse(QRectF(px - ss, py - ss, 2*ss, 2*ss));
      painter_->fillPath(path, brush);
      break;
    }
    case 8: { // triangle
      QPainterPath path;
      path.moveTo(QPointF(px     , py - ss));
      path.lineTo(QPointF(px + ss, py + ss));
      path.lineTo(QPointF(px - ss, py + ss));
      path.closeSubpath();
      painter_->strokePath(path, pen);
      break;
    }
    case 9: { // filled triangle
      QPainterPath path;
      path.moveTo(QPointF(px     , py - ss));
      path.lineTo(QPointF(px + ss, py + ss));
      path.lineTo(QPointF(px - ss, py + ss));
      path.closeSubpath();
      painter_->fillPath(path, brush);
      break;
    }
    case 10: { // inv triangle
      QPainterPath path;
      path.moveTo(QPointF(px     , py + ss));
      path.lineTo(QPointF(px + ss, py - ss));
      path.lineTo(QPointF(px - ss, py - ss));
      path.closeSubpath();
      painter_->strokePath(path, pen);
      break;
    }
    case 11: { // filled inv triangle
      QPainterPath path;
      path.moveTo(QPointF(px     , py + ss));
      path.lineTo(QPointF(px + ss, py - ss));
      path.lineTo(QPointF(px - ss, py - ss));
      path.closeSubpath();
      painter_->fillPath(path, brush);
      break;
    }
    case 12: { // diamond
      QPainterPath path;
      path.moveTo(QPointF(px - ss, py     ));
      path.lineTo(QPointF(px     , py - ss));
      path.lineTo(QPointF(px + ss, py     ));
      path.lineTo(QPointF(px     , py + ss));
      path.closeSubpath();
      painter_->strokePath(path, pen);
      break;
    }
    case 13: { // filled diamond
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
drawLine(const CPoint2D &point1, const CPoint2D &point2, double width)
{
  QPen p = painter_->pen();

  p.setWidthF(width);

  painter_->setPen(p);

  double px1, py1, px2, py2;

  windowToPixel(point1.x, point1.y, &px1, &py1);
  windowToPixel(point2.x, point2.y, &px2, &py2);

  painter_->drawLine(px1, py1, px2, py2);
}

void
CQGnuPlotRenderer::
drawRect(const CBBox2D &rect)
{
  double px1, py1, px2, py2;

  windowToPixel(rect.getXMin(), rect.getYMin(), &px1, &py1);
  windowToPixel(rect.getXMax(), rect.getYMax(), &px2, &py2);

  painter_->drawRect(px1, py1, px2 - px1, py2 - py1);
}

void
CQGnuPlotRenderer::
patternRect(const CBBox2D &rect, int pattern)
{
  static Qt::BrushStyle patterns[] = {
    Qt::BDiagPattern,
    Qt::FDiagPattern,
    Qt::HorPattern,
    Qt::VerPattern,
    Qt::CrossPattern,
    Qt::DiagCrossPattern,
    Qt::Dense7Pattern,
    Qt::Dense6Pattern,
    Qt::Dense5Pattern,
    Qt::Dense4Pattern,
    Qt::Dense3Pattern,
    Qt::Dense2Pattern,
    Qt::Dense1Pattern,
  };

  static int npatterns = sizeof(patterns)/sizeof(patterns[0]);

  double px1, py1, px2, py2;

  windowToPixel(rect.getXMin(), rect.getYMin(), &px1, &py1);
  windowToPixel(rect.getXMax(), rect.getYMax(), &px2, &py2);

  QBrush b(QBrush(patterns[pattern % npatterns]));

  painter_->fillRect(px1, py1, px2 - px1, py2 - py1, b);
}

void
CQGnuPlotRenderer::
fillRect(const CBBox2D &rect, const CGnuPlot::Color &color)
{
  double px1, py1, px2, py2;

  windowToPixel(rect.getXMin(), rect.getYMin(), &px1, &py1);
  windowToPixel(rect.getXMax(), rect.getYMax(), &px2, &py2);

  QBrush b(QColor(255*color.r, 255*color.g, 255*color.b), Qt::SolidPattern);

  painter_->fillRect(px1, py1, px2 - px1, py2 - py1, b);
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
drawPolygon(const std::vector<CPoint2D> &points, double w, bool fill)
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

  if (! fill) {
    QPen pen(QColor(0,0,0));

    pen.setWidthF(w);

    painter_->strokePath(path, pen);
  }
  else {
    QBrush brush(QColor(0,0,0));

    painter_->fillPath(path, brush);
  }
}

void
CQGnuPlotRenderer::
drawText(const CPoint2D &point, const std::string &text)
{
  double px, py;

  windowToPixel(point.x, point.y, &px, &py);

  //painter_->drawText(px, py, text.c_str());

  QMatrix m = painter_->worldMatrix();

  painter_->setWorldMatrix(QMatrix());

  QTransform t1, t2, t3;

  t1.translate(px, py);
  t2.rotate   (-font_.cast<CQFont>()->getAngle());
  t3.translate(-px, -py);

  painter_->setWorldTransform(t3*t2*t1);

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
windowToPixel(double wx, double wy, double *px, double *py)
{
  if (! mapping_) {
    *px = wx;
    *py = wy;
    return;
  }

  int margin = pw_/10;

  int pxmin = margin, pxmax = pw_ - margin;
  int pymin = margin, pymax = ph_ - margin;

  *px = ((wx - xmin_)/(xmax_ - xmin_))*(pxmax - pxmin) + pxmin;
  *py = ((wy - ymax_)/(ymin_ - ymax_))*(pymax - pymin) + pymin;
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

  int margin = pw_/10;

  int pxmin = margin, pxmax = pw_ - margin;
  int pymin = margin, pymax = ph_ - margin;

  *wx = (px - pxmin)*(xmax_ - xmin_)/(pxmax - pxmin) + xmin_;
  *wy = (py - pymin)*(ymin_ - ymax_)/(pymax - pymin) + ymax_;
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

//-----------

CQGnuPlotCanvas::
CQGnuPlotCanvas(CQGnuPlotWindow *window) :
 QWidget(0), window_(window)
{
  setFocusPolicy(Qt::StrongFocus);

  setMouseTracking(true);
}

CQGnuPlotCanvas::
~CQGnuPlotCanvas()
{
}

void
CQGnuPlotCanvas::
paintEvent(QPaintEvent *)
{
  QPainter p(this);

  window_->paint(&p);
}

void
CQGnuPlotCanvas::
mouseMoveEvent(QMouseEvent *e)
{
  CQGnuPlotRenderer *renderer = static_cast<CQGnuPlotRenderer *>(window_->renderer());

  double wx, wy;

  renderer->pixelToWindow(e->pos().x(), e->pos().y(), &wx, &wy);

  window_->unmapLogPoint(&wx, &wy);

  window_->showPos(wx, wy);
}
