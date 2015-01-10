#include <CQGnuPlotWindow.h>
#include <CQGnuPlot.h>
#include <CQGnuPlotGroup.h>
#include <CQGnuPlotPlot.h>
#include <CQGnuPlotCanvas.h>
#include <CQGnuPlotRenderer.h>
#include <CQGnuPlotLineStyle.h>
#include <CQGnuPlotObject.h>

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

static const int TreeWidth = 250;

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

  rframe->setMinimumWidth(TreeWidth);

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
}

CQGnuPlotWindow::
~CQGnuPlotWindow()
{
}

void
CQGnuPlotWindow::
setSize(const CISize2D &s)
{
  resize(s.width + TreeWidth, s.height);
}

void
CQGnuPlotWindow::
addProperties()
{
  tree_->clear();

  if (! redit_[0]) {
    redit_[0] = new CQPropertyRealEditor(0, 360, 1);
    redit_[1] = new CQPropertyRealEditor(-1000, 1000, 0.1);
    redit_[2] = new CQPropertyRealEditor(0, 50, 1);
    redit_[3] = new CQPropertyRealEditor(0, 1, 0.05);
  }

  if (is3D()) {
    tree_->addProperty("", this, "enable3D");
    tree_->addProperty("", this, "rotateX" )->setEditorFactory(redit_[0]);
    tree_->addProperty("", this, "rotateY" )->setEditorFactory(redit_[0]);
    tree_->addProperty("", this, "rotateZ" )->setEditorFactory(redit_[0]);
    tree_->addProperty("", this, "xmin3D"  )->setEditorFactory(redit_[1]);
    tree_->addProperty("", this, "xmax3D"  )->setEditorFactory(redit_[1]);
    tree_->addProperty("", this, "ymin3D"  )->setEditorFactory(redit_[1]);
    tree_->addProperty("", this, "ymax3D"  )->setEditorFactory(redit_[1]);
    tree_->addProperty("", this, "near3D"  )->setEditorFactory(redit_[1]);
    tree_->addProperty("", this, "far3D"   )->setEditorFactory(redit_[1]);
  }

  if (is3D()) {
    tree_->addProperty("", this, "hidden3D");
    tree_->addProperty("", this, "surface3D");
    tree_->addProperty("", this, "contour3D");
  }

  const CGnuPlot::LineStyles &lineStyles = plot_->lineStyles();

  for (const auto &ls : lineStyles) {
    CQGnuPlotLineStyle *ls1 = static_cast<CQGnuPlotLineStyle *>(ls.second.get());

    QString name = QString("lineStyle%1").arg(ls1->ind());

    tree_->addProperty(name, ls1, "width"    );
    tree_->addProperty(name, ls1, "color"    );
    tree_->addProperty(name, ls1, "pointType");
    tree_->addProperty(name, ls1, "pointSize");
  }

  for (auto group : groups()) {
    CQGnuPlotGroup *qgroup = static_cast<CQGnuPlotGroup *>(group);

    addGroupProperties(qgroup);

    for (auto subPlot : qgroup->plots())
      addPlotProperties(subPlot);
  }
}

void
CQGnuPlotWindow::
addGroupProperties(CGnuPlotGroup *group)
{
  CQGnuPlotGroup *qgroup = static_cast<CQGnuPlotGroup *>(group);

  QString name = QString("Group%1").arg(group->id());

  QString regionName = name + "/region";

  tree_->addProperty(regionName, qgroup, "regionLeft"  )->setEditorFactory(redit_[1]);
  tree_->addProperty(regionName, qgroup, "regionBottom")->setEditorFactory(redit_[1]);
  tree_->addProperty(regionName, qgroup, "regionRight" )->setEditorFactory(redit_[1]);
  tree_->addProperty(regionName, qgroup, "regionTop"   )->setEditorFactory(redit_[1]);

  QString marginName = name + "/margin";

  tree_->addProperty(marginName, qgroup, "marginLeft"  )->setEditorFactory(redit_[2]);
  tree_->addProperty(marginName, qgroup, "marginRight" )->setEditorFactory(redit_[2]);
  tree_->addProperty(marginName, qgroup, "marginTop"   )->setEditorFactory(redit_[2]);
  tree_->addProperty(marginName, qgroup, "marginBottom")->setEditorFactory(redit_[2]);

  tree_->addProperty(name, qgroup, "ratio")->setEditorFactory(redit_[1]);

  QString axesName = name + "/axes";

  tree_->addProperty(axesName, qgroup, "xlabel");
  tree_->addProperty(axesName, qgroup, "ylabel");
  tree_->addProperty(axesName, qgroup, "xtics");
  tree_->addProperty(axesName, qgroup, "ytics");
  tree_->addProperty(axesName, qgroup, "xticsMirror");
  tree_->addProperty(axesName, qgroup, "yticsMirror");
  tree_->addProperty(axesName, qgroup, "xgrid");
  tree_->addProperty(axesName, qgroup, "ygrid");

  tree_->addProperty(name, qgroup, "borders");
  tree_->addProperty(name, qgroup, "borderWidth");

  QString keyName = name + "/key";

  tree_->addProperty(keyName, qgroup, "keyDisplayed");
  tree_->addProperty(keyName, qgroup, "keyHAlign");
  tree_->addProperty(keyName, qgroup, "keyVAlign");
  tree_->addProperty(keyName, qgroup, "keyBox");
  tree_->addProperty(keyName, qgroup, "keyReverse");

  tree_->addProperty(name, qgroup, "histogramStyle");

  //---

  int ia = 0;

  const auto &arrows = group->arrows();

  for (const auto &a : arrows) {
    QString name1 = QString("%1/arrow%2").arg(name).arg(ia + 1);

    CQGnuPlotArrow *a1 = static_cast<CQGnuPlotArrow *>(a.get());

    tree_->addProperty(name1, a1, "from");
    tree_->addProperty(name1, a1, "to");
    tree_->addProperty(name1, a1, "relative");
    tree_->addProperty(name1, a1, "length");
    tree_->addProperty(name1, a1, "angle");
    tree_->addProperty(name1, a1, "backAngle");
    tree_->addProperty(name1, a1, "fhead");
    tree_->addProperty(name1, a1, "thead");
    tree_->addProperty(name1, a1, "filled");
    tree_->addProperty(name1, a1, "empty");
    tree_->addProperty(name1, a1, "front");
    tree_->addProperty(name1, a1, "lineType");
    tree_->addProperty(name1, a1, "lineWidth");
    tree_->addProperty(name1, a1, "strokeColor");

    ++ia;
  }

  //---

  int il = 0;

  const auto &labels = group->labels();

  for (const auto &l : labels) {
    QString name1 = QString("%1/label%2").arg(name).arg(il + 1);

    CQGnuPlotLabel *l1 = static_cast<CQGnuPlotLabel *>(l.get());

    tree_->addProperty(name1, l1, "text");
    tree_->addProperty(name1, l1, "pos");
    tree_->addProperty(name1, l1, "font");
    tree_->addProperty(name1, l1, "angle");
    tree_->addProperty(name1, l1, "front");
    tree_->addProperty(name1, l1, "offset");
    tree_->addProperty(name1, l1, "strokeColor");

    ++il;
  }

  //---

  int ir = 0;

  const auto &rectangles = group->rectangles();

  for (const auto &r : rectangles) {
    QString name1 = QString("%1/rectangle%2").arg(name).arg(ir + 1);

    CQGnuPlotRectangle *r1 = static_cast<CQGnuPlotRectangle *>(r.get());

    tree_->addProperty(name1, r1, "from");
    tree_->addProperty(name1, r1, "to");
    tree_->addProperty(name1, r1, "fillStyle");
    tree_->addProperty(name1, r1, "lineWidth");
    tree_->addProperty(name1, r1, "strokeColor");
    tree_->addProperty(name1, r1, "fillColor");

    ++ir;
  }

  //---

  int ie = 0;

  const auto &ellipses = group->ellipses();

  for (const auto &e : ellipses) {
    QString name1 = QString("%1/ellipse%2").arg(name).arg(ie + 1);

    CQGnuPlotEllipse *e1 = static_cast<CQGnuPlotEllipse *>(e.get());

    tree_->addProperty(name1, e1, "center");
    tree_->addProperty(name1, e1, "rx");
    tree_->addProperty(name1, e1, "ry");
    tree_->addProperty(name1, e1, "strokeColor");
    tree_->addProperty(name1, e1, "fillColor");

    ++ie;
  }

  //---

  int ip = 0;

  const auto &polygons = group->polygons();

  for (const auto &p : polygons) {
    QString name1 = QString("%1/polygon%2").arg(name).arg(ip + 1);

    CQGnuPlotPolygon *p1 = static_cast<CQGnuPlotPolygon *>(p.get());

    tree_->addProperty(name1, p1, "strokeColor");
    tree_->addProperty(name1, p1, "fillColor");

    ++ip;
  }
}

void
CQGnuPlotWindow::
addPlotProperties(CGnuPlotPlot *plot)
{
  CQGnuPlotPlot *qplot = static_cast<CQGnuPlotPlot *>(plot);

  QString name = QString("Plot%1").arg(plot->id());

  QString rangeName = name + "/range";

  tree_->addProperty(rangeName, qplot, "xmin")->setEditorFactory(redit_[1]);
  tree_->addProperty(rangeName, qplot, "ymin")->setEditorFactory(redit_[1]);
  tree_->addProperty(rangeName, qplot, "xmax")->setEditorFactory(redit_[1]);
  tree_->addProperty(rangeName, qplot, "ymax")->setEditorFactory(redit_[1]);

  if (plot->window()->is3D())
    tree_->addProperty(name, qplot, "trianglePattern3D");

  tree_->addProperty(name, qplot, "displayed");

  tree_->addProperty(name, qplot, "plotStyle");
  tree_->addProperty(name, qplot, "lineStyleId");

  tree_->addProperty(name, qplot, "fillType"     );
  tree_->addProperty(name, qplot, "fillPattern"  );
  tree_->addProperty(name, qplot, "lineColor"    );
  tree_->addProperty(name, qplot, "lineWidth"    );
  tree_->addProperty(name, qplot, "pointType"    );
  tree_->addProperty(name, qplot, "pointSize"    );
  tree_->addProperty(name, qplot, "boxWidthValue")->setEditorFactory(redit_[3]);
  tree_->addProperty(name, qplot, "boxWidthType" );
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
xAxisSlot(bool)
{
}

void
CQGnuPlotWindow::
yAxisSlot(bool)
{
}

void
CQGnuPlotWindow::
setCurrentGroup(CQGnuPlotGroup *group)
{
  if (group != currentGroup_) {
    currentGroup_ = group;

    addProperties();
  }
}

CQGnuPlotGroup *
CQGnuPlotWindow::
getGroupAt(const QPoint &pos)
{
  double wx, wy;

  pixelToWindow(pos.x(), pos.y(), &wx, &wy);

  for (auto group : groups()) {
    if (group->region().inside(CPoint2D(wx, wy))) {
      CQGnuPlotGroup *qgroup = static_cast<CQGnuPlotGroup *>(group);

      if (qgroup)
        return qgroup;
    }
  }

  return 0;
}

void
CQGnuPlotWindow::
mouseMove(const QPoint &qp)
{
  for (auto group : groups()) {
    CQGnuPlotGroup *qgroup = static_cast<CQGnuPlotGroup *>(group);

    CPoint2D p = group->pixelToWindow(CPoint2D(qp.x(), qp.y()));

    group->unmapLogPoint(&p.x, &p.y);

    qgroup->mouseMove(p);
  }
}

bool
CQGnuPlotWindow::
mouseTip(const QPoint &qp, CQGnuPlot::TipRect &tip)
{
  for (auto group : groups()) {
    CQGnuPlotGroup *qgroup = static_cast<CQGnuPlotGroup *>(group);

    CPoint2D p = group->pixelToWindow(CPoint2D(qp.x(), qp.y()));

    group->unmapLogPoint(&p.x, &p.y);

    if (qgroup->mouseTip(p, tip))
      return true;
  }

  return false;
}

void
CQGnuPlotWindow::
paint(QPainter *p)
{
  CQGnuPlotRenderer *qrenderer = qapp()->qrenderer();

  qrenderer->setWindow(this);
  qrenderer->setCanvas(canvas());
  qrenderer->setPainter(p);

  for (auto group : groups()) {
    CQGnuPlotGroup *qgroup = static_cast<CQGnuPlotGroup *>(group);

    qgroup->setPainter(p);
  }

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
  std::string xstr = (currentGroup_ ? currentGroup_->formatX(wx) : CStrUtil::toString(wx));
  std::string ystr = (currentGroup_ ? currentGroup_->formatY(wy) : CStrUtil::toString(wy));

  posLabel_->setText(QString("%1 %2").arg(xstr.c_str()).arg(ystr.c_str()));
}

void
CQGnuPlotWindow::
pixelToWindow(double px, double py, double *wx, double *wy)
{
  *wx = px/(canvas_->width () - 1);
  *wy = py/(canvas_->height() - 1);
}
