#include <CQApp.h>
#include <CQUtil.h>
#include <CQFont.h>
#include <CQZoomMouseMode.h>
#include <CQPanMouseMode.h>
#include <CQPropertyTree.h>
#include <CQPropertyItem.h>
#include <CQPropertyEditor.h>

#include <CQGnuPlot.h>
#include <CGnuPlot.h>
#include <CFontMgr.h>

#include <QPainter>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QMenuBar>
#include <QSplitter>
#include <QStatusBar>

namespace {
  QColor toQColor(const CRGBA &c) {
    return QColor(c.getRedI(),c.getGreenI(),c.getBlueI(),c.getAlphaI());
  }
}

class CQGnuPlotRenderer : public CGnuPlotRenderer {
 public:
  CQGnuPlotRenderer(CQGnuPlotWindow *window);
 ~CQGnuPlotRenderer();

  void setPainter(QPainter *painter);

  void drawPoint  (const CPoint2D &p, const CRGBA &c);
  void drawSymbol (const CPoint2D &p, CGnuPlot::SymbolType type, double size, const CRGBA &c);
  void drawLine   (const CPoint2D &p1, const CPoint2D &p2, double width, const CRGBA &c);
  void drawRect   (const CBBox2D &rect, const CRGBA &c);
  void patternRect(const CBBox2D &rect, CGnuPlot::PatternType pattern,
                   const CRGBA &fg, const CRGBA &bg);
  void fillRect   (const CBBox2D &rect, const CRGBA &c);
  void drawBezier (const CPoint2D &p1, const CPoint2D &p2, const CPoint2D &p3, const CPoint2D &p4);

  void drawPolygon(const std::vector<CPoint2D> &points, double w, const CRGBA &c);
  void fillPolygon(const std::vector<CPoint2D> &points, const CRGBA &c);

  void drawText(const CPoint2D &p, const std::string &text, const CRGBA &c);

  void windowToPixel(double x, double y, double *px, double *py);
  void pixelToWindow(double px, double py, double *wx, double *wy);

  double pixelWidthToWindowWidth  (double p);
  double pixelHeightToWindowHeight(double p);

  double windowWidthToPixelWidth  (double w);
  double windowHeightToPixelHeight(double w);

  void setMapping(bool b) { mapping_ = b; }

  CFontPtr getFont() const { return font_; }
  void setFont(CFontPtr font);

  void setLineDash(const CLineDash &line_dash);

 private:
  CQGnuPlotWindow *window_;
  QPainter        *painter_;
  CFontPtr         font_;
  int              pw_, ph_;
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

  window->resize(1000, 800);

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
  QWidget *frame = new QWidget;

  QHBoxLayout *layout = new QHBoxLayout(frame);
  layout->setMargin(2); layout->setSpacing(2);

  QSplitter *splitter = new QSplitter;

  canvas_ = new CQGnuPlotCanvas(this);

  splitter->addWidget(canvas_);

  QFrame *rframe = new QFrame;

  rframe->setMinimumWidth(275);

  QVBoxLayout *rlayout = new QVBoxLayout(rframe);
  rlayout->setMargin(2); rlayout->setSpacing(2);

  CQPropertyTree *tree = new CQPropertyTree;

  addPropeties(tree);

  rlayout->addWidget(tree);

  QFrame *buttonFrame = new QFrame;

  QHBoxLayout *buttonLayout = new QHBoxLayout(buttonFrame);
  buttonLayout->setMargin(2); buttonLayout->setSpacing(2);

  buttonLayout->addStretch(1);

  rlayout->addWidget(buttonFrame);

  splitter->addWidget(rframe);

  layout->addWidget(splitter);

  setCentralWidget(frame);

  //------

  renderer_ = new CQGnuPlotRenderer(this);

  setRenderer(renderer_);

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
addPropeties(CQPropertyTree *tree)
{
  CQPropertyRealEditor *redit1 = new CQPropertyRealEditor(0, 360, 1);
  CQPropertyRealEditor *redit2 = new CQPropertyRealEditor(-100, 100, 0.1);
  CQPropertyRealEditor *redit3 = new CQPropertyRealEditor(0, 50, 1);

  tree->addProperty("", this, "enable3D");
  tree->addProperty("", this, "rotateX" )->setEditorFactory(redit1);
  tree->addProperty("", this, "rotateY" )->setEditorFactory(redit1);
  tree->addProperty("", this, "rotateZ" )->setEditorFactory(redit1);
  tree->addProperty("", this, "xmin3D"  )->setEditorFactory(redit2);
  tree->addProperty("", this, "xmax3D"  )->setEditorFactory(redit2);
  tree->addProperty("", this, "ymin3D"  )->setEditorFactory(redit2);
  tree->addProperty("", this, "ymax3D"  )->setEditorFactory(redit2);
  tree->addProperty("", this, "near3D"  )->setEditorFactory(redit2);
  tree->addProperty("", this, "far3D"   )->setEditorFactory(redit2);

  tree->addProperty("", this, "marginLeft"  )->setEditorFactory(redit3);
  tree->addProperty("", this, "marginRight" )->setEditorFactory(redit3);
  tree->addProperty("", this, "marginTop"   )->setEditorFactory(redit3);
  tree->addProperty("", this, "marginBottom")->setEditorFactory(redit3);

  tree->addProperty("", this, "hidden3D");
  tree->addProperty("", this, "surface3D");
  tree->addProperty("", this, "contour3D");
  tree->addProperty("", this, "trianglePattern3D");

  connect(tree, SIGNAL(valueChanged(QObject *, const QString &)), canvas_, SLOT(update()));
}

void
CQGnuPlotWindow::
setEnable3D(bool b)
{
  setCameraEnabled(b);

  redraw();
}

void
CQGnuPlotWindow::
setRotateX(double a)
{
  setCameraRotateX(a);

  redraw();
}

void
CQGnuPlotWindow::
setRotateY(double a)
{
  setCameraRotateY(a);

  redraw();
}

void
CQGnuPlotWindow::
setRotateZ(double a)
{
  setCameraRotateZ(a);

  redraw();
}

void
CQGnuPlotWindow::
setXMin3D(double x)
{
  setCameraXMin(x);

  redraw();
}

void
CQGnuPlotWindow::
setXMax3D(double x)
{
  setCameraXMax(x);

  redraw();
}

void
CQGnuPlotWindow::
setYMin3D(double y)
{
  setCameraYMin(y);

  redraw();
}

void
CQGnuPlotWindow::
setYMax3D(double y)
{
  setCameraYMax(y);

  redraw();
}

void
CQGnuPlotWindow::
setNear3D(double z)
{
  setCameraNear(z);

  redraw();
}

void
CQGnuPlotWindow::
setFar3D(double z)
{
  setCameraFar(z);

  redraw();
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
 window_(window), pw_(1), ph_(1), mapping_(true)
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

  double lmargin = pw_*window_->marginLeft  ()/100.0;
  double rmargin = pw_*window_->marginRight ()/100.0;
  double tmargin = ph_*window_->marginTop   ()/100.0;
  double bmargin = ph_*window_->marginBottom()/100.0;

  double pxmin = lmargin, pxmax = pw_ - rmargin;
  double pymin = tmargin, pymax = ph_ - bmargin;

  double xmin, ymin, xmax, ymax;

  window_->getDisplayRange(&xmin, &ymin, &xmax, &ymax);

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

  double lmargin = pw_*window_->marginLeft  ()/100.0;
  double rmargin = pw_*window_->marginRight ()/100.0;
  double tmargin = ph_*window_->marginTop   ()/100.0;
  double bmargin = ph_*window_->marginBottom()/100.0;

  double pxmin = lmargin, pxmax = pw_ - rmargin;
  double pymin = tmargin, pymax = ph_ - bmargin;

  double xmin, ymin, xmax, ymax;

  window_->getDisplayRange(&xmin, &ymin, &xmax, &ymax);

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
