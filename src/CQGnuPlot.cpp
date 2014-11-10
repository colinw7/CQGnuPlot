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

class CQGnuPlotRenderer : public CGnuPlotRenderer {
 public:
  CQGnuPlotRenderer(CQGnuPlot *app, CGnuPlot::Plot *plot);

  void setPainter(QPainter *painter);

  void drawPoint (const Point &p);
  void drawLine  (const Point &p1, const Point &p2);
  void drawBezier(const Point &p1, const Point &p2, const Point &p3, const Point &p4);

  void drawText(const CGnuPlot::Point &p, const std::string &text);

  void windowToPixel(double x, double y, double *px, double *py);
  void pixelToWindow(double px, double py, double *wx, double *wy);

  double pixelWidthToWindowWidth  (double p);
  double pixelHeightToWindowHeight(double p);

  CFontPtr getFont() const { return font_; }

  void setFont(CFontPtr font);

 private:
  CQGnuPlot      *app_;
  CGnuPlot::Plot *plot_;
  QPainter       *painter_;
  CFontPtr        font_;
  int             pw_, ph_;
  double          xmin_, xmax_;
  double          ymin_, ymax_;
};

int
main(int argc, char **argv)
{
  CQApp app(argc, argv);

  bool debug = false;

  std::vector<std::string> files;

  for (int i = 1; i < argc; i++) {
    if (argv[i][0] == '-') {
      if (argv[i][1] == 'd')
        debug = true;
    }
    else
      files.push_back(argv[i]);
  }

  CQGnuPlot plot;

  plot.setDebug(debug);

  uint num_files = files.size();

  for (uint i = 0; i < num_files; i++)
    plot.load(files[i]);

  plot.resize(400, 400);

  plot.show();

  plot.loop();

  return 0;
}

CQGnuPlot::
CQGnuPlot() :
 QMainWindow()
{
  canvas_ = new CQGnuPlotCanvas(this);

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
  yAxisAction->setCheckable(true);

  connect(xAxisAction, SIGNAL(triggered(bool)), this, SLOT(xAxisSlot(bool)));
  connect(yAxisAction, SIGNAL(triggered(bool)), this, SLOT(yAxisSlot(bool)));

  dispMenu->addAction(xAxisAction);
  dispMenu->addAction(yAxisAction);

  QAction *helpAction = new QAction("&Help", this);

  helpMenu->addAction(helpAction);
}

void
CQGnuPlot::
timeout()
{
  qApp->processEvents();
}

void
CQGnuPlot::
stateChanged(CGnuPlot::ChangeState)
{
  redraw();
}

void
CQGnuPlot::
redraw()
{
  canvas_->update();
}

void
CQGnuPlot::
redrawOverlay()
{
  redraw();
}

QPointF
CQGnuPlot::
pixelToWindow(const QPoint &p)
{
  return QPointF(p.x(), p.y());
}

void
CQGnuPlot::
setCursor(QCursor cursor)
{
  canvas_->setCursor(cursor);
}

// Pan
void
CQGnuPlot::
panBy(double, double)
{
}

void
CQGnuPlot::
panTo(const CPoint2D &)
{
}

void
CQGnuPlot::
panLeft()
{
}

void
CQGnuPlot::
panRight()
{
}

void
CQGnuPlot::
panUp()
{
}

void
CQGnuPlot::
panDown()
{
}

void
CQGnuPlot::
resetPan()
{
}

// Zoom
void
CQGnuPlot::
zoomTo(const CBBox2D &)
{
}

void
CQGnuPlot::
zoomIn(const CPoint2D &)
{
}

void
CQGnuPlot::
zoomOut(const CPoint2D &)
{
}

void
CQGnuPlot::
zoomIn()
{
}

void
CQGnuPlot::
zoomOut()
{
}

void
CQGnuPlot::
resetZoom()
{
}

void
CQGnuPlot::
load(const std::string &filename)
{
  CGnuPlot::load(filename);
}

void
CQGnuPlot::
xAxisSlot(bool checked)
{
  uint np = CGnuPlot::numPlots();

  for (uint i = 0; i < np; ++i) {
    CGnuPlot::Plot *plot = CGnuPlot::getPlot(i);

    plot->showXAxis(checked);
  }

  update();
}

void
CQGnuPlot::
yAxisSlot(bool checked)
{
  uint np = CGnuPlot::numPlots();

  for (uint i = 0; i < np; ++i) {
    CGnuPlot::Plot *plot = CGnuPlot::getPlot(i);

    plot->showYAxis(checked);
  }

  update();
}

void
CQGnuPlot::
paint(QPainter *p)
{
  p->fillRect(canvas_->rect(), QBrush(QColor(255,255,255)));

  p->setPen(QColor(0,0,0));

  uint np = CGnuPlot::numPlots();

  for (uint i = 0; i < np; ++i) {
    CGnuPlot::Plot *plot = CGnuPlot::getPlot(i);

    paintPlot(p, plot);
  }
}

void
CQGnuPlot::
paintPlot(QPainter *p, CGnuPlot::Plot *plot)
{
  CQGnuPlotRenderer renderer(this, plot);

  renderer.setPainter(p);

  plot->draw(&renderer);
}

//-----------

CQGnuPlotRenderer::
CQGnuPlotRenderer(CQGnuPlot *app, CGnuPlot::Plot *plot) :
 app_(app), plot_(plot), pw_(1), ph_(1), xmin_(0), xmax_(1), ymin_(0), ymax_(1)
{
  pw_ = app_->canvas()->width ();
  ph_ = app_->canvas()->height();

  plot->getXRange(xmin_, xmax_);
  plot->getYRange(ymin_, ymax_);

  font_ = CFontMgrInst->lookupFont("helvetica", CFONT_STYLE_NORMAL, 12);
}

void
CQGnuPlotRenderer::
setPainter(QPainter *painter)
{
  painter_ = painter;
}

void
CQGnuPlotRenderer::
drawPoint(const Point &point)
{
  double px, py;

  windowToPixel(point.x, point.y, &px, &py);

  int ss = 2;

  painter_->drawLine(QPoint(px - ss, py - ss), QPoint(px + ss, py + ss));
  painter_->drawLine(QPoint(px - ss, py + ss), QPoint(px + ss, py - ss));
}

void
CQGnuPlotRenderer::
drawLine(const Point &point1, const Point &point2)
{
  double px1, py1, px2, py2;

  windowToPixel(point1.x, point1.y, &px1, &py1);
  windowToPixel(point2.x, point2.y, &px2, &py2);

  painter_->drawLine(px1, py1, px2, py2);
}

void
CQGnuPlotRenderer::
drawBezier(const Point &point1, const Point &point2, const Point &point3, const Point &point4)
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
drawText(const CGnuPlot::Point &point, const std::string &text)
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

  return fabs(wx2 - wx1);
}

double
CQGnuPlotRenderer::
pixelHeightToWindowHeight(double h)
{
  double wx1, wy1, wx2, wy2;

  pixelToWindow(0, 0, &wx1, &wy1);
  pixelToWindow(h, h, &wx2, &wy2);

  return fabs(wy2 - wy1);
}

//-----------

CQGnuPlotCanvas::
CQGnuPlotCanvas(CQGnuPlot *plot) :
 QWidget(NULL), plot_(plot)
{
}

void
CQGnuPlotCanvas::
paintEvent(QPaintEvent *)
{
  QPainter p(this);

  plot_->paint(&p);
}
