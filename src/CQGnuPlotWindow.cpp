#include <CQGnuPlotWindow.h>
#include <CQGnuPlot.h>
#include <CQGnuPlotGroup.h>
#include <CQGnuPlotPlot.h>
#include <CQGnuPlotCanvas.h>
#include <CQGnuPlotRenderer.h>
#include <CQGnuPlotLineStyle.h>
#include <CQGnuPlotAnnotation.h>
#include <CQGnuPlotArrow.h>
#include <CQGnuPlotEllipse.h>
#include <CQGnuPlotPolygon.h>
#include <CQGnuPlotRectangle.h>
#include <CQGnuPlotAxis.h>
#include <CQGnuPlotKey.h>
#include <CQGnuPlotTitle.h>
#include <CQGnuPlotLabel.h>
#include <CQGnuPlotBar.h>
#include <CQGnuPlotPie.h>
#include <CQGnuPlotBubble.h>
#include <CQGnuPlotDevice.h>

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
#include <QTimer>

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

  connect(tree_, SIGNAL(itemSelected(QObject *, const QString &)),
          this, SLOT(itemSelectedSlot(QObject *, const QString &)));

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
updateProperties()
{
  if (! propTimer_) {
    propTimer_ = new QTimer(this);

    connect(propTimer_, SIGNAL(timeout()), this, SLOT(addProperties()));
  }

  propTimer_->setSingleShot(true);
  propTimer_->start(250);
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

    QString name = QString("LineStyles/lineStyle%1").arg(ls1->ind());

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

  tree_->addProperty(name, qgroup, "histogramStyle");

  //---

  CQGnuPlotTitle *qtitle = static_cast<CQGnuPlotTitle *>(group->title());

  QString titleName = QString("%1/title").arg(name);

  tree_->addProperty(titleName, qtitle, "text"  );
  tree_->addProperty(titleName, qtitle, "offset");
  tree_->addProperty(titleName, qtitle, "color" );
  tree_->addProperty(titleName, qtitle, "font"  );

  //---

  for (const auto &axis : qgroup->axes()) {
    CQGnuPlotAxis *qaxis = static_cast<CQGnuPlotAxis *>(axis.second);

    QString axesName = QString("%1/axes_%2").arg(name).arg(qaxis->id().c_str());

    tree_->addProperty(axesName, qaxis, "displayed");
    tree_->addProperty(axesName, qaxis, "label");
  }

#if 0
  tree_->addProperty(axesName, qgroup, "xmin")->setEditorFactory(redit_[1]);
  tree_->addProperty(axesName, qgroup, "xmax")->setEditorFactory(redit_[1]);
  tree_->addProperty(axesName, qgroup, "ymin")->setEditorFactory(redit_[1]);
  tree_->addProperty(axesName, qgroup, "ymax")->setEditorFactory(redit_[1]);

  tree_->addProperty(axesName, qgroup, "xtics");
  tree_->addProperty(axesName, qgroup, "ytics");
  tree_->addProperty(axesName, qgroup, "xticsMirror");
  tree_->addProperty(axesName, qgroup, "yticsMirror");
  tree_->addProperty(axesName, qgroup, "xgrid");
  tree_->addProperty(axesName, qgroup, "ygrid");
#endif

  tree_->addProperty(name, qgroup, "borders");
  tree_->addProperty(name, qgroup, "borderWidth");

  //---

  CQGnuPlotKey *qkey = dynamic_cast<CQGnuPlotKey *>(qgroup->key().get());

  QString keyName = name + "/key";

  tree_->addProperty(keyName, qkey, "displayed");
  tree_->addProperty(keyName, qkey, "drawBox");
  tree_->addProperty(keyName, qkey, "fillBox");
  tree_->addProperty(keyName, qkey, "reverse");
  tree_->addProperty(keyName, qkey, "outside");
  tree_->addProperty(keyName, qkey, "halign");
  tree_->addProperty(keyName, qkey, "valign");
  tree_->addProperty(keyName, qkey, "font");

  //---

  for (auto &ann : group->annotations()) {
    CGnuPlotArrow     *arrow   = 0;
    CGnuPlotEllipse   *ellipse = 0;
    CGnuPlotLabel     *label   = 0;
    CGnuPlotPolygon   *poly    = 0;
    CGnuPlotRectangle *rect    = 0;

    //CQGnuPlotArrow *qann = static_cast<CQGnuPlotArrow *>(ann.get());

    if      ((arrow = dynamic_cast<CGnuPlotArrow *>(ann.get()))) {
      QString name1 = QString("%1/%2_%3").arg(name).arg(arrow->getName()).arg(ann->getInd());

      CQGnuPlotArrow *qarrow = static_cast<CQGnuPlotArrow *>(arrow);

      tree_->addProperty(name1, qarrow, "strokeColor");
      tree_->addProperty(name1, qarrow, "fillColor");
      tree_->addProperty(name1, qarrow, "drawLayer");

      tree_->addProperty(name1, qarrow, "from");
      tree_->addProperty(name1, qarrow, "to");
      tree_->addProperty(name1, qarrow, "relative");
      tree_->addProperty(name1, qarrow, "length");
      tree_->addProperty(name1, qarrow, "angle");
      tree_->addProperty(name1, qarrow, "backAngle");
      tree_->addProperty(name1, qarrow, "fhead");
      tree_->addProperty(name1, qarrow, "thead");
      tree_->addProperty(name1, qarrow, "filled");
      tree_->addProperty(name1, qarrow, "empty");
      tree_->addProperty(name1, qarrow, "front");
      tree_->addProperty(name1, qarrow, "lineType");
      tree_->addProperty(name1, qarrow, "lineWidth");
    }
    else if ((ellipse = dynamic_cast<CGnuPlotEllipse *>(ann.get()))) {
      QString name1 = QString("%1/%2_%3").arg(name).arg(ellipse->getName()).arg(ann->getInd());

      CQGnuPlotEllipse *qellipse = static_cast<CQGnuPlotEllipse *>(ellipse);

      tree_->addProperty(name1, qellipse, "strokeColor");
      tree_->addProperty(name1, qellipse, "fillColor");
      tree_->addProperty(name1, qellipse, "drawLayer");

      tree_->addProperty(name1, qellipse, "center");
      tree_->addProperty(name1, qellipse, "rx");
      tree_->addProperty(name1, qellipse, "ry");
    }
    else if ((label = dynamic_cast<CGnuPlotLabel *>(ann.get()))) {
      QString name1 = QString("%1/%2_%3").arg(name).arg(label->getName()).arg(ann->getInd());

      CQGnuPlotLabel *qlabel = static_cast<CQGnuPlotLabel *>(label);

      tree_->addProperty(name1, qlabel, "strokeColor");
      //tree_->addProperty(name1, qlabel, "fillColor");
      tree_->addProperty(name1, qlabel, "drawLayer");

      tree_->addProperty(name1, qlabel, "text");
      tree_->addProperty(name1, qlabel, "pos");
      tree_->addProperty(name1, qlabel, "font");
      tree_->addProperty(name1, qlabel, "angle");
      tree_->addProperty(name1, qlabel, "front");
      tree_->addProperty(name1, qlabel, "offset");
    }
    else if ((poly = dynamic_cast<CGnuPlotPolygon *>(ann.get()))) {
      QString name1 = QString("%1/%2_%3").arg(name).arg(poly->getName()).arg(ann->getInd());

      CQGnuPlotPolygon *qpoly = static_cast<CQGnuPlotPolygon *>(poly);

      tree_->addProperty(name1, qpoly, "strokeColor");
      tree_->addProperty(name1, qpoly, "fillColor");
      tree_->addProperty(name1, qpoly, "drawLayer");
    }
    else if ((rect = dynamic_cast<CGnuPlotRectangle *>(ann.get()))) {
      QString name1 = QString("%1/%2_%3").arg(name).arg(rect->getName()).arg(ann->getInd());

      CQGnuPlotRectangle *qrect = static_cast<CQGnuPlotRectangle *>(rect);

      tree_->addProperty(name1, qrect, "strokeColor");
      tree_->addProperty(name1, qrect, "fillColor");
      tree_->addProperty(name1, qrect, "drawLayer");

      tree_->addProperty(name1, qrect, "from");
      tree_->addProperty(name1, qrect, "to");
      tree_->addProperty(name1, qrect, "fillType");
      tree_->addProperty(name1, qrect, "lineWidth");
    }
  }
}

void
CQGnuPlotWindow::
addPlotProperties(CGnuPlotPlot *plot)
{
  CQGnuPlotPlot *qplot = static_cast<CQGnuPlotPlot *>(plot);

  CQGnuPlotGroup *qgroup = static_cast<CQGnuPlotGroup *>(plot->group());

  QString name = QString("Group%1/Plot%2").arg(qgroup->id()).arg(plot->id());

  QString rangeName = name + "/range";

  tree_->addProperty(rangeName, qplot, "xmin")->setEditorFactory(redit_[1]);
  tree_->addProperty(rangeName, qplot, "ymin")->setEditorFactory(redit_[1]);
  tree_->addProperty(rangeName, qplot, "xmax")->setEditorFactory(redit_[1]);
  tree_->addProperty(rangeName, qplot, "ymax")->setEditorFactory(redit_[1]);

  if (plot->window()->is3D())
    tree_->addProperty(name, qplot, "trianglePattern3D");

  if (plot->isImageStyle()) {
    QString imageName = name + "/image";

    tree_->addProperty(imageName, qplot, "imageAngle")->setEditorFactory(redit_[0]);
  }

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

  int i = 0;

  for (const auto &bar : plot->bars()) {
    QString name1 = QString("%1/Bar%2").arg(name).arg(i + 1);

    CQGnuPlotBar *qbar = static_cast<CQGnuPlotBar *>(bar);

    tree_->addProperty(name1, qbar, "lineColor");
    tree_->addProperty(name1, qbar, "fillColor");

    ++i;
  }

  i = 0;

  for (const auto &pie : plot->pies()) {
    QString name1 = QString("%1/Pie%2").arg(name).arg(i + 1);

    CQGnuPlotPie *qpie = static_cast<CQGnuPlotPie *>(pie);

    tree_->addProperty(name1, qpie, "name" );
    tree_->addProperty(name1, qpie, "color");

    ++i;
  }

  i = 0;

  for (const auto &bubble : plot->bubbles()) {
    QString name1 = QString("%1/Bubble%2").arg(name).arg(i + 1);

    CQGnuPlotBubble *qbubble = static_cast<CQGnuPlotBubble *>(bubble);

    tree_->addProperty(name1, qbubble, "name" );
    tree_->addProperty(name1, qbubble, "color");

    ++i;
  }
}

void
CQGnuPlotWindow::
selectObject(const QObject *obj)
{
  tree_->selectObject(obj);
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
mousePress(const QPoint &qp)
{
  for (auto group : groups()) {
    CQGnuPlotGroup *qgroup = static_cast<CQGnuPlotGroup *>(group);

    qgroup->mousePress(qp);
  }
}

void
CQGnuPlotWindow::
mouseMove(const QPoint &qp)
{
  for (auto group : groups()) {
    CQGnuPlotGroup *qgroup = static_cast<CQGnuPlotGroup *>(group);

    qgroup->mouseMove(qp);
  }
}

bool
CQGnuPlotWindow::
mouseTip(const QPoint &qp, CQGnuPlot::TipRect &tip)
{
  for (auto group : groups()) {
    CQGnuPlotGroup *qgroup = static_cast<CQGnuPlotGroup *>(group);

    if (qgroup->mouseTip(qp, tip))
      return true;
  }

  return false;
}

void
CQGnuPlotWindow::
paint(QPainter *p)
{
  CQGnuPlotRenderer *qrenderer = qapp()->qrenderer();

  qrenderer->setWidth (width ());
  qrenderer->setHeight(height());

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
  std::string xstr, ystr;

  if (currentGroup_) {
    const CGnuPlot::AxisData &xaxis = currentGroup_->xaxis(1);
    const CGnuPlot::AxisData &yaxis = currentGroup_->yaxis(1);

    xstr = currentGroup_->formatAxisValue(xaxis, wx);
    ystr = currentGroup_->formatAxisValue(yaxis, wy);
  }
  else {
    xstr = CStrUtil::toString(wx);
    ystr = CStrUtil::toString(wy);
  }

  posLabel_->setText(QString("%1 %2").arg(xstr.c_str()).arg(ystr.c_str()));
}

// mouse mode (unused ?)
QPointF
CQGnuPlotWindow::
pixelToWindow(const QPoint &p)
{
  return QPointF(p.x(), p.y());
}

void
CQGnuPlotWindow::
pixelToWindow(double px, double py, double *wx, double *wy)
{
  *wx = px/(canvas_->width () - 1);
  *wy = py/(canvas_->height() - 1);
}

void
CQGnuPlotWindow::
itemSelectedSlot(QObject *obj, const QString & /*path*/)
{
  CQGnuPlotObject *qobject = dynamic_cast<CQGnuPlotObject *>(obj);
  if (! qobject) return;

  CQGnuPlotDevice *qdevice = qapp()->qdevice();

  for (auto qobject1 : qdevice->objects())
    qobject1->setSelected(false);

  //std::cerr << "Object \'" << qobject->objectName().toStdString() << "\'" << std::endl;

  qobject->setSelected(true);

  canvas_->update();
}
