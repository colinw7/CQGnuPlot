#include <CQGnuPlotWindow.h>
#include <CQGnuPlot.h>
#include <CQGnuPlotPlot.h>
#include <CQGnuPlotCanvas.h>
#include <CQGnuPlotRenderer.h>

#include <CQZoomMouseMode.h>
#include <CQPanMouseMode.h>
#include <CQPropertyTree.h>
#include <CQPropertyItem.h>
#include <CQPropertyEditor.h>

#include <QPainter>
#include <QHBoxLayout>
#include <QMenuBar>
#include <QSplitter>
#include <QStatusBar>
#include <QLabel>
#include <QFrame>

uint CQGnuPlotWindow::lastId;

CQGnuPlotWindow::
CQGnuPlotWindow(CQGnuPlot *plot) :
 QMainWindow(), CGnuPlotWindow(plot), id_(++lastId), plot_(plot),
 redit1_(0), redit2_(0), redit3_(0), currentPlot_(0)
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

  tree_ = new CQPropertyTree;

  connect(tree_, SIGNAL(valueChanged(QObject *, const QString &)), canvas_, SLOT(update()));

  rlayout->addWidget(tree_);

  QFrame *buttonFrame = new QFrame;

  QHBoxLayout *buttonLayout = new QHBoxLayout(buttonFrame);
  buttonLayout->setMargin(2); buttonLayout->setSpacing(2);

  buttonLayout->addStretch(1);

  rlayout->addWidget(buttonFrame);

  splitter->addWidget(rframe);

  layout->addWidget(splitter);

  setCentralWidget(frame);

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

  xAxisAction->setCheckable(true); xAxisAction->setChecked(true);
  yAxisAction->setCheckable(true); yAxisAction->setChecked(true);

  connect(xAxisAction, SIGNAL(triggered(bool)), this, SLOT(xAxisSlot(bool)));
  connect(yAxisAction, SIGNAL(triggered(bool)), this, SLOT(yAxisSlot(bool)));

  dispMenu->addAction(xAxisAction);
  dispMenu->addAction(yAxisAction);

  QAction *helpAction = new QAction("&Help", this);

  helpMenu->addAction(helpAction);

  //---

  posLabel_  = new QLabel;
  plotLabel_ = new QLabel;

  statusBar()->addPermanentWidget(posLabel_ );
  statusBar()->addPermanentWidget(plotLabel_);

  showPos(0, 0);

  //---

  renderer_ = new CQGnuPlotRenderer(canvas_);

  setRenderer(renderer_);
}

CQGnuPlotWindow::
~CQGnuPlotWindow()
{
}

void
CQGnuPlotWindow::
addProperties()
{
  tree_->clear();

  if (! redit1_) {
    redit1_ = new CQPropertyRealEditor(0, 360, 1);
    redit2_ = new CQPropertyRealEditor(-1000, 1000, 0.1);
    redit3_ = new CQPropertyRealEditor(0, 50, 1);
  }

  if (is3D()) {
    tree_->addProperty("", this, "enable3D");
    tree_->addProperty("", this, "rotateX" )->setEditorFactory(redit1_);
    tree_->addProperty("", this, "rotateY" )->setEditorFactory(redit1_);
    tree_->addProperty("", this, "rotateZ" )->setEditorFactory(redit1_);
    tree_->addProperty("", this, "xmin3D"  )->setEditorFactory(redit2_);
    tree_->addProperty("", this, "xmax3D"  )->setEditorFactory(redit2_);
    tree_->addProperty("", this, "ymin3D"  )->setEditorFactory(redit2_);
    tree_->addProperty("", this, "ymax3D"  )->setEditorFactory(redit2_);
    tree_->addProperty("", this, "near3D"  )->setEditorFactory(redit2_);
    tree_->addProperty("", this, "far3D"   )->setEditorFactory(redit2_);
  }

  if (is3D()) {
    tree_->addProperty("", this, "hidden3D");
    tree_->addProperty("", this, "surface3D");
    tree_->addProperty("", this, "contour3D");
  }

  tree_->addProperty("", this, "title");

  for (auto plot : plots()) {
    const Plots &subPlots = plot->subPlots();

    if (! subPlots.empty()) {
      addPlotProperties(plot);

      for (auto subPlot : subPlots)
        addPlotProperties(subPlot, true);
    }
    else {
      addPlotProperties(plot);
    }
  }
}

void
CQGnuPlotWindow::
addPlotProperties(CGnuPlotPlot *plot, bool child)
{
  CQGnuPlotPlot *plot1 = static_cast<CQGnuPlotPlot *>(plot);

  QString name = QString("Plot%1").arg(plot->id());

  if (! child) {
    tree_->addProperty(name, plot1, "regionLeft"  )->setEditorFactory(redit2_);
    tree_->addProperty(name, plot1, "regionBottom")->setEditorFactory(redit2_);
    tree_->addProperty(name, plot1, "regionRight" )->setEditorFactory(redit2_);
    tree_->addProperty(name, plot1, "regionTop"   )->setEditorFactory(redit2_);

    tree_->addProperty(name, plot1, "marginLeft"  )->setEditorFactory(redit3_);
    tree_->addProperty(name, plot1, "marginRight" )->setEditorFactory(redit3_);
    tree_->addProperty(name, plot1, "marginTop"   )->setEditorFactory(redit3_);
    tree_->addProperty(name, plot1, "marginBottom")->setEditorFactory(redit3_);

    tree_->addProperty(name, plot1, "xmin")->setEditorFactory(redit2_);
    tree_->addProperty(name, plot1, "ymin")->setEditorFactory(redit2_);
    tree_->addProperty(name, plot1, "xmax")->setEditorFactory(redit2_);
    tree_->addProperty(name, plot1, "ymax")->setEditorFactory(redit2_);

    tree_->addProperty(name, plot1, "ratio")->setEditorFactory(redit2_);

    tree_->addProperty(name, plot1, "xlabel");
    tree_->addProperty(name, plot1, "ylabel");
    tree_->addProperty(name, plot1, "xtics");
    tree_->addProperty(name, plot1, "ytics");
    tree_->addProperty(name, plot1, "xgrid");
    tree_->addProperty(name, plot1, "ygrid");

    tree_->addProperty(name, plot1, "borders");
    tree_->addProperty(name, plot1, "borderWidth")->setEditorFactory(redit3_);

    tree_->addProperty(name, plot1, "keyDisplayed");
    tree_->addProperty(name, plot1, "keyBox");
    tree_->addProperty(name, plot1, "keyHAlign");
    tree_->addProperty(name, plot1, "keyVAlign");

    if (plot->window()->is3D())
      tree_->addProperty(name, plot1, "trianglePattern3D");
  }
  else {
    tree_->addProperty(name, plot1, "lineColor");
    tree_->addProperty(name, plot1, "lineWidth");
    tree_->addProperty(name, plot1, "pointType");
    tree_->addProperty(name, plot1, "pointSize");
  }
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

QString
CQGnuPlotWindow::
title() const
{
  CGnuPlot::Title title = CGnuPlotWindow::title();

  return title.str.c_str();
}

void
CQGnuPlotWindow::
setTitle(const QString &s)
{
  CGnuPlot::Title title = CGnuPlotWindow::title();

  title.str = s.toStdString();

  CGnuPlotWindow::setTitle(title);
}

void
CQGnuPlotWindow::
stateChanged(CGnuPlot::ChangeState)
{
  addProperties();

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
  if (currentPlot_)
    currentPlot_->showXAxis(checked);

  update();
}

void
CQGnuPlotWindow::
yAxisSlot(bool checked)
{
  if (currentPlot_)
    currentPlot_->showYAxis(checked);

  update();
}

void
CQGnuPlotWindow::
setCurrentPlot(CQGnuPlotPlot *plot)
{
  currentPlot_ = plot;

  addProperties();
}

CQGnuPlotPlot *
CQGnuPlotWindow::
getPlotAt(const QPoint &pos)
{
  double wx, wy;

  pixelToWindow(pos.x(), pos.y(), &wx, &wy);

  for (auto plot : plots()) {
    if (plot->region().inside(CPoint2D(wx, wy)))
      return static_cast<CQGnuPlotPlot *>(plot);
  }

  return 0;
}

void
CQGnuPlotWindow::
paint(QPainter *p)
{
  renderer_->setPainter(p);

  for (auto plot : plots())
    static_cast<CQGnuPlotPlot *>(plot)->setPainter(p);

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
  posLabel_->setText(QString("%1 %2").arg(wx).arg(wy));
}

void
CQGnuPlotWindow::
pixelToWindow(double px, double py, double *wx, double *wy)
{
  *wx = px/(canvas_->width () - 1);
  *wy = py/(canvas_->height() - 1);
}
