#include <CQGnuPlotWindow.h>
#include <CQGnuPlotMultiplot.h>
#include <CQGnuPlot.h>
#include <CQGnuPlotGroup.h>
#include <CQGnuPlotPlot.h>
#include <CQGnuPlotCanvas.h>
#include <CQGnuPlotRenderer.h>
#include <CQGnuPlotLineStyle.h>
#include <CQGnuPlotAnnotation.h>
#include <CQGnuPlotArrow.h>
#include <CQGnuPlotCircle.h>
#include <CQGnuPlotEllipse.h>
#include <CQGnuPlotPolygon.h>
#include <CQGnuPlotRectangle.h>
#include <CQGnuPlotAxis.h>
#include <CQGnuPlotKey.h>
#include <CQGnuPlotColorBox.h>
#include <CQGnuPlotPalette.h>
#include <CQGnuPlotTitle.h>
#include <CQGnuPlotCamera.h>
#include <CQGnuPlotPm3D.h>
#include <CQGnuPlotTimeStamp.h>
#include <CQGnuPlotLabel.h>
#include <CQGnuPlotDevice.h>
#include <CQGnuPlotUtil.h>
#include <CQGnuPlotPositionEdit.h>

#include <CQGnuPlotArrowObject.h>
#include <CQGnuPlotBoxBarObject.h>
#include <CQGnuPlotEndBar.h>
#include <CQGnuPlotBoxObject.h>
#include <CQGnuPlotBubbleObject.h>
#include <CQGnuPlotEllipseObject.h>
#include <CQGnuPlotErrorBarObject.h>
#include <CQGnuPlotFinanceBarObject.h>
#include <CQGnuPlotImageObject.h>
#include <CQGnuPlotLabelObject.h>
#include <CQGnuPlotPathObject.h>
#include <CQGnuPlotPieObject.h>
#include <CQGnuPlotPointObject.h>
#include <CQGnuPlotPolygonObject.h>
#include <CQGnuPlotRectObject.h>

#include <CQGnuPlotFill.h>
#include <CQGnuPlotStroke.h>
#include <CQGnuPlotMark.h>

#include <CQGnuPlotToolBar.h>
#include <CQGnuPlotLoadFileDialog.h>
#include <CQGnuPlotLoadFunctionDialog.h>
#include <CQGnuPlotManageFunctionsDialog.h>
#include <CQGnuPlotManageVariablesDialog.h>
#include <CQGnuPlotPaletteDialog.h>
#include <CQGnuPlotCreateDialog.h>
#include <CQGnuPlotDataDialog.h>
#include <CQGnuPlotSaveDialog.h>

#include <CQGnuPlotPNGRenderer.h>
#include <CGnuPlotSVGRenderer.h>
#include <CQUtil.h>
#include <CQZoomRegion.h>
#include <CQCursor.h>

#include <CQPropertyTree.h>
#include <CQPropertyItem.h>
#include <CQPropertyEditor.h>
#include <CQFloatLabel.h>
#include <CQRubberBand.h>
#include <CQPixmapCache.h>
#include <CUnixFile.h>

#include <QPainter>
#include <QHBoxLayout>
#include <QMenuBar>
#include <QSplitter>
#include <QStatusBar>
#include <QLabel>
#include <QFrame>
#include <QTimer>
#include <QPushButton>
#include <QLineEdit>
#include <QFileDialog>

#include <svg/select_svg.h>
#include <svg/move_svg.h>
#include <svg/zoom_svg.h>
#include <svg/grid_svg.h>
#include <svg/xaxis_svg.h>
#include <svg/yaxis_svg.h>
#include <svg/probe_svg.h>
#include <svg/points_svg.h>

static const int TreeWidth = 400;

CQGnuPlotWindow::
CQGnuPlotWindow(CQGnuPlot *plot) :
 CGnuPlotWindow(plot), plot_(plot)
{
}

void
CQGnuPlotWindow::
setApp(CQGnuPlot *plot)
{
  plot_ = plot;

  CGnuPlotWindow::setApp(plot);
}

void
CQGnuPlotWindow::
highlightObject(QObject *obj)
{
  CQGnuPlotDevice *qdevice = qapp()->qdevice();

  for (auto qobject : qdevice->objects())
    CQUtil::setProperty(qobject, "highlighted", QVariant(false));

  if (obj)
    CQUtil::setProperty(obj, "highlighted", QVariant(true));

  redraw();
}

//------

uint CQGnuPlotMainWindow::lastId;

CQGnuPlotMainWindow::
CQGnuPlotMainWindow(CQGnuPlot *plot) :
 QMainWindow(), CQGnuPlotWindow(plot), id_(++lastId), mode_(Mode::SELECT)
{
  setObjectName("window");

  //---

  selectAction_ = new QAction(CQPixmapCacheInst->getIcon("SELECT"), "Select", this);
  moveAction_   = new QAction(CQPixmapCacheInst->getIcon("MOVE"  ), "Move"  , this);
  zoomAction_   = new QAction(CQPixmapCacheInst->getIcon("ZOOM"  ), "Zoom"  , this);
  probeAction_  = new QAction(CQPixmapCacheInst->getIcon("PROBE" ), "Probe" , this);
  pointsAction_ = new QAction(CQPixmapCacheInst->getIcon("POINTS"), "Points", this);

  selectAction_->setCheckable(true);
  moveAction_  ->setCheckable(true);
  zoomAction_  ->setCheckable(true);
  probeAction_ ->setCheckable(true);

  selectAction_->setChecked(true);

  connect(selectAction_, SIGNAL(triggered(bool)), this, SLOT(selectMode(bool)));
  connect(moveAction_  , SIGNAL(triggered(bool)), this, SLOT(moveMode  (bool)));
  connect(zoomAction_  , SIGNAL(triggered(bool)), this, SLOT(zoomMode  (bool)));
  connect(probeAction_ , SIGNAL(triggered(bool)), this, SLOT(probeMode (bool)));

  connect(pointsAction_, SIGNAL(triggered(bool)), this, SLOT(pointsSlot()));

  //---

  CQGnuPlotToolBar *toolbar = new CQGnuPlotToolBar("Controls");

  addToolBar(toolbar);

  toolbar->addAction(selectAction_);
  toolbar->addAction(moveAction_);
  toolbar->addAction(zoomAction_);
  toolbar->addAction(probeAction_);
  toolbar->addAction(pointsAction_);

  //---

  QWidget *frame = new QWidget;
  frame->setObjectName("frame");

  QHBoxLayout *layout = new QHBoxLayout(frame);
  layout->setMargin(2); layout->setSpacing(2);

  QSplitter *splitter = new QSplitter;
  splitter->setObjectName("splitter");

  canvas_ = new CQGnuPlotCanvas(this);

  splitter->addWidget(canvas_);

  QFrame *rframe = new QFrame;
  rframe->setObjectName("rframe");

  rframe->setMinimumWidth(TreeWidth);

  QVBoxLayout *rlayout = new QVBoxLayout(rframe);
  rlayout->setMargin(2); rlayout->setSpacing(2);

  edit_ = new QLineEdit;

  connect(edit_, SIGNAL(returnPressed()), this, SLOT(searchProperties()));

  rlayout->addWidget(edit_);

  tree_ = new CQPropertyTree;

  connect(tree_, SIGNAL(valueChanged(QObject *, const QString &)), canvas_, SLOT(update()));

  connect(tree_, SIGNAL(itemSelected(QObject *, const QString &)),
          this, SLOT(itemSelectedSlot(QObject *, const QString &)));

  connect(tree_, SIGNAL(menuExec(QObject *, const QPoint &)),
          this, SLOT(treeMenuExec(QObject *, const QPoint &)));

  rlayout->addWidget(tree_);

  QFrame *buttonFrame = new QFrame;
  buttonFrame->setObjectName("buttonFrame");

  QHBoxLayout *buttonLayout = new QHBoxLayout(buttonFrame);
  buttonLayout->setMargin(2); buttonLayout->setSpacing(2);

  QPushButton *reloadButton = new QPushButton("Reload");

  connect(reloadButton, SIGNAL(clicked()), this, SLOT(addProperties()));

  buttonLayout->addWidget(reloadButton);

  QPushButton *expandButton = new QPushButton("Expand");

  connect(expandButton, SIGNAL(clicked()), tree_, SLOT(expandSelected()));

  buttonLayout->addWidget(expandButton);

  buttonLayout->addStretch(1);

  rlayout->addWidget(buttonFrame);

  splitter->addWidget(rframe);

  layout->addWidget(splitter);

  setCentralWidget(frame);

  //----

  QMenu *fileMenu = menuBar()->addMenu("&File");

  QAction *newWindowAction = new QAction("New Window", this);

  fileMenu->addAction(newWindowAction);

  connect(newWindowAction, SIGNAL(triggered()), this, SLOT(newWindow()));

  //---

  QAction *loadFileAction = new QAction("Plot File", this);

  fileMenu->addAction(loadFileAction);

  connect(loadFileAction, SIGNAL(triggered()), this, SLOT(loadFile()));

  QAction *loadFnAction = new QAction("Plot Function", this);

  fileMenu->addAction(loadFnAction);

  connect(loadFnAction, SIGNAL(triggered()), this, SLOT(loadFunction()));

  //---

  QAction *dataAction = new QAction("Process Data", this);

  fileMenu->addAction(dataAction);

  connect(dataAction, SIGNAL(triggered()), this, SLOT(showData()));

  //---

  QAction *createAction = new QAction("Create Object", this);

  fileMenu->addAction(createAction);

  connect(createAction, SIGNAL(triggered()), this, SLOT(createObjects()));

  //---

  QAction *saveAction = new QAction("Save Image", this);

  fileMenu->addAction(saveAction);

  connect(saveAction, SIGNAL(triggered()), this, SLOT(saveSlot()));

#if 0
  QAction *saveSVGAction = new QAction("Save S&VG", this);
  QAction *savePNGAction = new QAction("Save &PNG", this);

  fileMenu->addAction(saveSVGAction);
  fileMenu->addAction(savePNGAction);

  connect(saveSVGAction, SIGNAL(triggered()), this, SLOT(saveSVG()));
  connect(savePNGAction, SIGNAL(triggered()), this, SLOT(savePNG()));
#endif

  //---

  QAction *closeAction = new QAction("&Close", this);

  connect(closeAction, SIGNAL(triggered()), this, SLOT(close()));

  fileMenu->addAction(closeAction);

  //--------

  QMenu *viewMenu = menuBar()->addMenu("&View");

  QAction *functionsAction = new QAction("Manage Functions", this);

  viewMenu->addAction(functionsAction);

  connect(functionsAction, SIGNAL(triggered()), this, SLOT(manageFunctions()));

  QAction *variablesAction = new QAction("Manage Variables", this);

  viewMenu->addAction(variablesAction);

  connect(variablesAction, SIGNAL(triggered()), this, SLOT(manageVariables()));

  QAction *paletteAction = new QAction("Color Palette", this);

  viewMenu->addAction(paletteAction);

  connect(paletteAction, SIGNAL(triggered()), this, SLOT(editColorPalette()));

  //--------

  QMenu *modeMenu = menuBar()->addMenu("&Mode");

  modeMenu->addAction(selectAction_);
  modeMenu->addAction(moveAction_);
  modeMenu->addAction(zoomAction_);
  modeMenu->addAction(probeAction_);
  modeMenu->addAction(pointsAction_);

  //------

  QMenu *dispMenu = menuBar()->addMenu("&Display");

  QAction *xAxisAction = new QAction(CQPixmapCacheInst->getIcon("XAXIS"), "&X Axis", this);
  QAction *yAxisAction = new QAction(CQPixmapCacheInst->getIcon("YAXIS"), "&Y Axis", this);
  QAction *gridAction  = new QAction(CQPixmapCacheInst->getIcon("GRID" ), "&Grid"  , this);

  xAxisAction->setCheckable(true); xAxisAction->setChecked(true);
  yAxisAction->setCheckable(true); yAxisAction->setChecked(true);
  gridAction ->setCheckable(true); gridAction ->setChecked(false);

  connect(xAxisAction, SIGNAL(triggered(bool)), this, SLOT(xAxisSlot(bool)));
  connect(yAxisAction, SIGNAL(triggered(bool)), this, SLOT(yAxisSlot(bool)));
  connect(gridAction , SIGNAL(triggered(bool)), this, SLOT(gridSlot(bool)));

  QAction *pixelAction = new QAction("&Pixels", this);
  pixelAction->setCheckable(true); pixelAction->setChecked(false);

  connect(pixelAction, SIGNAL(triggered(bool)), this, SLOT(pixelSlot(bool)));

  dispMenu->addAction(xAxisAction);
  dispMenu->addAction(yAxisAction);
  dispMenu->addAction(gridAction);
  dispMenu->addAction(pixelAction);

  toolbar->addAction(xAxisAction);
  toolbar->addAction(yAxisAction);
  toolbar->addAction(gridAction);

  //------

  QMenu *helpMenu = menuBar()->addMenu("&Help");

  QAction *helpAction = new QAction("&Help", this);

  helpMenu->addAction(helpAction);

  //---

  posLabel_ = new QLabel;
  posLabel_->setObjectName("posLabel");

  plotLabel_ = new QLabel;
  plotLabel_->setObjectName("plotLabel");

  statusBar()->addPermanentWidget(posLabel_ );
  statusBar()->addPermanentWidget(plotLabel_);

  showPos("", 0, 0, 0, 0);

  //---

  zoomRegion_ = new CQZoomRegion(canvas_);

  //---

  //cursor_ = CQCursorMgrInst->createCursor("info_cursor.svg", 2, 2);
  //canvas_->setCursor(cursor_->cursor());
}

CQGnuPlotMainWindow::
~CQGnuPlotMainWindow()
{
}

void
CQGnuPlotMainWindow::
setSize(const CISize2D &s)
{
  resize(s.width + TreeWidth, s.height);
}

int
CQGnuPlotMainWindow::
cursorSize() const
{
  return CQCursorMgrInst->size();
}

void
CQGnuPlotMainWindow::
setCursorSize(int s)
{
  CQCursorMgrInst->setSize(s);

  canvas_->setCursor(cursor_->cursor());
}

void
CQGnuPlotMainWindow::
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
CQGnuPlotMainWindow::
searchProperties()
{
  QLineEdit *edit = qobject_cast<QLineEdit *>(sender());

  tree_->search(edit->text());
}

void
CQGnuPlotMainWindow::
addProperties()
{
  tree_->clear();

  tree_->addProperty("", this, "cursorSize");
  tree_->addProperty("", this, "tipOutside");
  tree_->addProperty("", this, "backgroundColor");

  const CGnuPlot::LineStyles &lineStyles = plot_->lineStyles();

  for (const auto &ls : lineStyles) {
    CQGnuPlotLineStyle *ls1 = static_cast<CQGnuPlotLineStyle *>(ls.second.get());

    if (! ls1->ind().isValid())
      continue;

    QString lineStyleName = QString("LineStyles/lineStyle%1").arg(ls1->ind().getValue());

    tree_->addProperty(lineStyleName, ls1, "lineType"     );
    tree_->addProperty(lineStyleName, ls1, "lineWidth"    );
    tree_->addProperty(lineStyleName, ls1, "lineDash"     );
    tree_->addProperty(lineStyleName, ls1, "lineColor"    );
    tree_->addProperty(lineStyleName, ls1, "pointType"    );
    tree_->addProperty(lineStyleName, ls1, "pointSize"    );
    tree_->addProperty(lineStyleName, ls1, "pointInterval");
    tree_->addProperty(lineStyleName, ls1, "palette"      );
  }

  //---

  CQGnuPlotMultiplot *qmultiplot = (plot_->multiplot().isValid() ?
    static_cast<CQGnuPlotMultiplot *>(plot_->multiplot().getPtr()) : 0);

  if (qmultiplot) {
    QString multiplotName("Multiplot");

    tree_->addProperty(multiplotName, qmultiplot, "enabled");
    tree_->addProperty(multiplotName, qmultiplot, "autoFit");
    tree_->addProperty(multiplotName, qmultiplot, "enhanced");
    tree_->addProperty(multiplotName, qmultiplot, "rows");
    tree_->addProperty(multiplotName, qmultiplot, "cols");
    tree_->addProperty(multiplotName, qmultiplot, "rowsFirst");
    tree_->addProperty(multiplotName, qmultiplot, "downward");
    tree_->addProperty(multiplotName, qmultiplot, "xscale");
    tree_->addProperty(multiplotName, qmultiplot, "yscale");
    tree_->addProperty(multiplotName, qmultiplot, "xoffset");
    tree_->addProperty(multiplotName, qmultiplot, "yoffset");
    tree_->addProperty(multiplotName, qmultiplot, "xspacing");
    tree_->addProperty(multiplotName, qmultiplot, "yspacing");
    tree_->addProperty(multiplotName, qmultiplot, "lmargin");
    tree_->addProperty(multiplotName, qmultiplot, "rmargin");
    tree_->addProperty(multiplotName, qmultiplot, "tmargin");
    tree_->addProperty(multiplotName, qmultiplot, "bmargin");
    tree_->addProperty(multiplotName, qmultiplot, "title");
    tree_->addProperty(multiplotName, qmultiplot, "titleFont");
  }

  //---

  for (auto group : groups()) {
    CQGnuPlotGroup *qgroup = static_cast<CQGnuPlotGroup *>(group.get());

    addGroupProperties(qgroup);

    for (auto subPlot : qgroup->plots())
      addPlotProperties(subPlot.get());
  }
}

void
CQGnuPlotMainWindow::
addGroupProperties(CGnuPlotGroup *group)
{
  CQGnuPlotGroup *qgroup = static_cast<CQGnuPlotGroup *>(group);

  QString groupName = QString("Group%1").arg(group->id());

  tree_->addProperty(groupName, qgroup, "ind" );
  tree_->addProperty(groupName, qgroup, "is3D");
  tree_->addProperty(groupName, qgroup, "isPolar");

  if (group->is3D()) {
    CQGnuPlotCamera *qcamera = static_cast<CQGnuPlotCamera *>(group->camera().get());

    QString cameraName = groupName + "/camera";

    tree_->addProperty(cameraName, qcamera, "enabled");

    tree_->addProperty(cameraName, qcamera, "rotateX")->setEditorFactory(realSlider("0:360:1"));
    tree_->addProperty(cameraName, qcamera, "rotateY")->setEditorFactory(realSlider("0:360:1"));
    tree_->addProperty(cameraName, qcamera, "rotateZ")->setEditorFactory(realSlider("0:360:1"));

    tree_->addProperty(cameraName, qcamera, "xmin")->setEditorFactory(realEdit("-1000:1000:0.1"));
    tree_->addProperty(cameraName, qcamera, "xmax")->setEditorFactory(realEdit("-1000:1000:0.1"));
    tree_->addProperty(cameraName, qcamera, "ymin")->setEditorFactory(realEdit("-1000:1000:0.1"));
    tree_->addProperty(cameraName, qcamera, "ymax")->setEditorFactory(realEdit("-1000:1000:0.1"));
    tree_->addProperty(cameraName, qcamera, "near")->setEditorFactory(realEdit("-1000:1000:0.1"));
    tree_->addProperty(cameraName, qcamera, "far" )->setEditorFactory(realEdit("-1000:1000:0.1"));

    tree_->addProperty(cameraName, qcamera, "planeZ");
    tree_->addProperty(cameraName, qcamera, "planeRelative");

    tree_->addProperty(groupName, qgroup, "hidden3D");
    tree_->addProperty(groupName, qgroup, "hiddenGrayScale");

    //---

    CQGnuPlotPm3D *qpm3d = static_cast<CQGnuPlotPm3D *>(group->pm3D().get());

    QString pm3dName = groupName + "/pm3d";

    tree_->addProperty(pm3dName, qpm3d, "enabled");
    tree_->addProperty(pm3dName, qpm3d, "ftriangles");
    tree_->addProperty(pm3dName, qpm3d, "clipIn");
    tree_->addProperty(pm3dName, qpm3d, "lineType");
    tree_->addProperty(pm3dName, qpm3d, "implicit");
  }

  QString regionName = groupName + "/region";

  tree_->addProperty(regionName, qgroup, "regionLeft"  )->setLabel("left"  );
  tree_->addProperty(regionName, qgroup, "regionBottom")->setLabel("bottom");
  tree_->addProperty(regionName, qgroup, "regionRight" )->setLabel("right" );
  tree_->addProperty(regionName, qgroup, "regionTop"   )->setLabel("top"   );

  QString marginName = groupName + "/margin";

  tree_->addProperty(marginName, qgroup, "marginLeft"  )->
   setLabel("left"  )->setEditorFactory(realEdit("0:50:1"));
  tree_->addProperty(marginName, qgroup, "marginBottom")->
   setLabel("bottom")->setEditorFactory(realEdit("0:50:1"));
  tree_->addProperty(marginName, qgroup, "marginRight" )->
   setLabel("right" )->setEditorFactory(realEdit("0:50:1"));
  tree_->addProperty(marginName, qgroup, "marginTop"   )->
   setLabel("top"   )->setEditorFactory(realEdit("0:50:1"));

  QString rangeName = groupName + "/range";

  tree_->addProperty(rangeName, qgroup, "rangeLeft"  )->setLabel("left"  );
  tree_->addProperty(rangeName, qgroup, "rangeBottom")->setLabel("bottom");
  tree_->addProperty(rangeName, qgroup, "rangeRight" )->setLabel("right" );
  tree_->addProperty(rangeName, qgroup, "rangeTop"   )->setLabel("top"   );

  tree_->addProperty(groupName, qgroup, "ratio")->setEditorFactory(realEdit("-1000:1000:0.1"));

  tree_->addProperty(groupName, qgroup, "histogramStyle");

  tree_->addProperty(groupName, qgroup, "histogramGap");

  //---

  CQGnuPlotTitle *qtitle = static_cast<CQGnuPlotTitle *>(group->title().get());

  QString titleName = QString("%1/title").arg(groupName);

  tree_->addProperty(titleName, qtitle, "displayed");
  tree_->addProperty(titleName, qtitle, "text"     );
  tree_->addProperty(titleName, qtitle, "offset"   );
  tree_->addProperty(titleName, qtitle, "font"     );
  tree_->addProperty(titleName, qtitle, "color"    );
  tree_->addProperty(titleName, qtitle, "enhanced" );

  //---

  CQGnuPlotTimeStamp *qtimeStamp = static_cast<CQGnuPlotTimeStamp *>(group->timeStamp().getPtr());

  QString timeStampName = QString("%1/timeStamp").arg(groupName);

  tree_->addProperty(timeStampName, qtimeStamp, "top");

  //---

  for (const auto &axis : qgroup->axes()) {
    const CGnuPlotGroup::IAxes &iaxes = axis.second;

    for (const auto &iaxis : iaxes) {
      CQGnuPlotAxis *qaxis = static_cast<CQGnuPlotAxis *>(iaxis.second);

      QString axisName = QString("%1/Axes/axis_%2").arg(groupName).arg(qaxis->id().c_str());

      tree_->addProperty(axisName, qaxis, "displayed");
      tree_->addProperty(axisName, qaxis, "start");
      tree_->addProperty(axisName, qaxis, "end");
      tree_->addProperty(axisName, qaxis, "start1");
      tree_->addProperty(axisName, qaxis, "end1");
      tree_->addProperty(axisName, qaxis, "log");
      tree_->addProperty(axisName, qaxis, "logBase");
      tree_->addProperty(axisName, qaxis, "majorTics");
      tree_->addProperty(axisName, qaxis, "minorTics");
      tree_->addProperty(axisName, qaxis, "majorIncrement");
      tree_->addProperty(axisName, qaxis, "minorIncrement");
      tree_->addProperty(axisName, qaxis, "tickIncrement");
      tree_->addProperty(axisName, qaxis, "majorScale");
      tree_->addProperty(axisName, qaxis, "minorScale");
      tree_->addProperty(axisName, qaxis, "ticsRotate")->setEditorFactory(realSlider("0:360:1"));
      tree_->addProperty(axisName, qaxis, "labelRotate")->setEditorFactory(realSlider("0:360:1"));
      tree_->addProperty(axisName, qaxis, "tickInside");
      tree_->addProperty(axisName, qaxis, "labelInside");
      tree_->addProperty(axisName, qaxis, "label");
      tree_->addProperty(axisName, qaxis, "timeFormat");
      tree_->addProperty(axisName, qaxis, "ticFont");
      tree_->addProperty(axisName, qaxis, "labelFont");
      tree_->addProperty(axisName, qaxis, "ticColor");
      tree_->addProperty(axisName, qaxis, "labelColor");
      tree_->addProperty(axisName, qaxis, "parallel");
      tree_->addProperty(axisName, qaxis, "zeroAxisDisplayed");
      tree_->addProperty(axisName, qaxis, "zeroAxisLineWidth");
      tree_->addProperty(axisName, qaxis, "borderTics");
      tree_->addProperty(axisName, qaxis, "enhanced");
      tree_->addProperty(axisName, qaxis, "mirror");
      tree_->addProperty(axisName, qaxis, "drawTickMark");
      tree_->addProperty(axisName, qaxis, "drawMinorTickMark");
      tree_->addProperty(axisName, qaxis, "drawTickLabel");
      tree_->addProperty(axisName, qaxis, "drawLabel");

      QString gridName = QString("%1/grid").arg(axisName);

      tree_->addProperty(gridName, qaxis, "grid"              )->setLabel("displayed");
      tree_->addProperty(gridName, qaxis, "gridMajor"         )->setLabel("majorTics");
      tree_->addProperty(gridName, qaxis, "gridMinor"         )->setLabel("minorTics");
      tree_->addProperty(gridName, qaxis, "gridPolarAngle"    )->setLabel("polarAngle");
      tree_->addProperty(gridName, qaxis, "gridMajorLineStyle")->setLabel("majorLineStyle");
      tree_->addProperty(gridName, qaxis, "gridMinorLineStyle")->setLabel("minorLineStyle");
      tree_->addProperty(gridName, qaxis, "gridMajorLineType" )->setLabel("majorLineType");
      tree_->addProperty(gridName, qaxis, "gridMinorLineType" )->setLabel("minorLineType");
      tree_->addProperty(gridName, qaxis, "gridMajorLineWidth")->setLabel("majorLineWidth");
      tree_->addProperty(gridName, qaxis, "gridMinorLineWidth")->setLabel("minorLineWidth");
      tree_->addProperty(gridName, qaxis, "gridLayer"         )->setLabel("layer");
    }
  }

  QString borderName = groupName + "/border";

  tree_->addProperty(borderName, qgroup, "borderSides")->setLabel("sides");
  tree_->addProperty(borderName, qgroup, "borderLayer")->setLabel("layer");
  tree_->addProperty(borderName, qgroup, "borderWidth")->setLabel("width");
  tree_->addProperty(borderName, qgroup, "borderStyle")->setLabel("style");
  tree_->addProperty(borderName, qgroup, "borderType" )->setLabel("type");

  //---

  CQGnuPlotKey *qkey = dynamic_cast<CQGnuPlotKey *>(qgroup->key().get());

  QString keyName = groupName + "/key";

  tree_->addProperty(keyName, qkey, "displayed");
  tree_->addProperty(keyName, qkey, "drawBox");
  tree_->addProperty(keyName, qkey, "fillBox");
  tree_->addProperty(keyName, qkey, "boxLineStyle");
  tree_->addProperty(keyName, qkey, "title");
  tree_->addProperty(keyName, qkey, "reverse");
  tree_->addProperty(keyName, qkey, "outside");
  tree_->addProperty(keyName, qkey, "halign");
  tree_->addProperty(keyName, qkey, "valign");
  tree_->addProperty(keyName, qkey, "font");
  tree_->addProperty(keyName, qkey, "enhanced");
  tree_->addProperty(keyName, qkey, "lmargin");
  tree_->addProperty(keyName, qkey, "rmargin");
  tree_->addProperty(keyName, qkey, "tmargin");
  tree_->addProperty(keyName, qkey, "bmargin");
  tree_->addProperty(keyName, qkey, "sampLen");
  tree_->addProperty(keyName, qkey, "spacing");

  //---

  CQGnuPlotColorBox *qcolorBox = dynamic_cast<CQGnuPlotColorBox *>(qgroup->colorBox().get());

  QString colorBoxName = groupName + "/colorBox";

  tree_->addProperty(colorBoxName, qcolorBox, "enabled");
  tree_->addProperty(colorBoxName, qcolorBox, "vertical");
  tree_->addProperty(colorBoxName, qcolorBox, "user");
  tree_->addProperty(colorBoxName, qcolorBox, "front");
  tree_->addProperty(colorBoxName, qcolorBox, "border");
  tree_->addProperty(colorBoxName, qcolorBox, "borderStyle");
  tree_->addProperty(colorBoxName, qcolorBox, "origin");
  tree_->addProperty(colorBoxName, qcolorBox, "size");
  tree_->addProperty(colorBoxName, qcolorBox, "title");
  tree_->addProperty(colorBoxName, qcolorBox, "angle");
  tree_->addProperty(colorBoxName, qcolorBox, "min");
  tree_->addProperty(colorBoxName, qcolorBox, "max");

  //---

  CQGnuPlotPalette *qpalette = dynamic_cast<CQGnuPlotPalette *>(qgroup->palette().get());

  QString paletteName = groupName + "/palette";

  tree_->addProperty(paletteName, qpalette, "colorType");
  tree_->addProperty(paletteName, qpalette, "colorModel");
  tree_->addProperty(paletteName, qpalette, "gamma");
  tree_->addProperty(paletteName, qpalette, "gray");
  tree_->addProperty(paletteName, qpalette, "negative");

  tree_->addProperty(paletteName, qpalette, "redModel"  )->setEditorFactory(integerEdit("0:33"));
  tree_->addProperty(paletteName, qpalette, "greenModel")->setEditorFactory(integerEdit("0:33"));
  tree_->addProperty(paletteName, qpalette, "blueModel" )->setEditorFactory(integerEdit("0:33"));

  //---

  for (auto &vann : group->annotations()) {
    for (auto &ann : vann.second) {
      CGnuPlotArrow     *arrow   = 0;
      CGnuPlotCircle    *circle  = 0;
      CGnuPlotEllipse   *ellipse = 0;
      CGnuPlotLabel     *label   = 0;
      CGnuPlotPolygon   *poly    = 0;
      CGnuPlotRectangle *rect    = 0;

      if      ((arrow = dynamic_cast<CGnuPlotArrow *>(ann.get()))) {
        QString arrowName =
          QString("%1/Arrows/%2_%3").arg(groupName).arg(arrow->getName()).arg(ann->getInd());

        CQGnuPlotArrow *qarrow = static_cast<CQGnuPlotArrow *>(arrow);

        tree_->addProperty(arrowName, qarrow, "strokeColor");
        tree_->addProperty(arrowName, qarrow, "fillColor");
        tree_->addProperty(arrowName, qarrow, "drawLayer");

        tree_->addProperty(arrowName, qarrow, "coordType");
        tree_->addProperty(arrowName, qarrow, "from");
        tree_->addProperty(arrowName, qarrow, "to");
        tree_->addProperty(arrowName, qarrow, "length");
        tree_->addProperty(arrowName, qarrow, "angle");
        tree_->addProperty(arrowName, qarrow, "headLength");
        tree_->addProperty(arrowName, qarrow, "headAngle");
        tree_->addProperty(arrowName, qarrow, "headBackAngle");
        tree_->addProperty(arrowName, qarrow, "fhead");
        tree_->addProperty(arrowName, qarrow, "thead");
        tree_->addProperty(arrowName, qarrow, "headFilled");
        tree_->addProperty(arrowName, qarrow, "headEmpty");

        tree_->addProperty(arrowName, qarrow, "lineType");
        tree_->addProperty(arrowName, qarrow, "lineWidth")->setEditorFactory(realEdit("0:50:1"));
        tree_->addProperty(arrowName, qarrow, "lineDash");
      }
      else if ((circle = dynamic_cast<CGnuPlotCircle *>(ann.get()))) {
        QString circleName =
          QString("%1/Circles/%2_%3").arg(groupName).arg(circle->getName()).arg(ann->getInd());

        CQGnuPlotCircle *qcircle = static_cast<CQGnuPlotCircle *>(circle);

        tree_->addProperty(circleName, qcircle, "strokeColor");
        tree_->addProperty(circleName, qcircle, "fillColor");
        tree_->addProperty(circleName, qcircle, "clip");
        tree_->addProperty(circleName, qcircle, "drawLayer");

        tree_->addProperty(circleName, qcircle, "center");
        tree_->addProperty(circleName, qcircle, "radius");
        tree_->addProperty(circleName, qcircle, "lineWidth");
      }
      else if ((ellipse = dynamic_cast<CGnuPlotEllipse *>(ann.get()))) {
        QString ellipseName =
          QString("%1/Ellipses/%2_%3").arg(groupName).arg(ellipse->getName()).arg(ann->getInd());

        CQGnuPlotEllipse *qellipse = static_cast<CQGnuPlotEllipse *>(ellipse);

        tree_->addProperty(ellipseName, qellipse, "strokeColor");
        tree_->addProperty(ellipseName, qellipse, "fillColor");
        tree_->addProperty(ellipseName, qellipse, "clip");
        tree_->addProperty(ellipseName, qellipse, "drawLayer");

        tree_->addProperty(ellipseName, qellipse, "center");
        tree_->addProperty(ellipseName, qellipse, "size");
        tree_->addProperty(ellipseName, qellipse, "angle");
        tree_->addProperty(ellipseName, qellipse, "lineWidth");
      }
      else if ((label = dynamic_cast<CGnuPlotLabel *>(ann.get()))) {
        QString labelName =
          QString("%1/Labels/%2_%3").arg(groupName).arg(label->getName()).arg(ann->getInd());

        CQGnuPlotLabel *qlabel = static_cast<CQGnuPlotLabel *>(label);

        tree_->addProperty(labelName, qlabel, "displayed");
        tree_->addProperty(labelName, qlabel, "drawLayer");
        tree_->addProperty(labelName, qlabel, "text");
        tree_->addProperty(labelName, qlabel, "pos");
        tree_->addProperty(labelName, qlabel, "color");
        tree_->addProperty(labelName, qlabel, "angle")->setEditorFactory(realSlider("0:360:1"));
        tree_->addProperty(labelName, qlabel, "enhanced");
        tree_->addProperty(labelName, qlabel, "align");
        tree_->addProperty(labelName, qlabel, "font");
        tree_->addProperty(labelName, qlabel, "offset");
        tree_->addProperty(labelName, qlabel, "lineType");
        tree_->addProperty(labelName, qlabel, "boxFill");
        tree_->addProperty(labelName, qlabel, "boxFillColor");
        tree_->addProperty(labelName, qlabel, "box");
        tree_->addProperty(labelName, qlabel, "boxStrokeColor");
        tree_->addProperty(labelName, qlabel, "boxStrokeWidth");
        tree_->addProperty(labelName, qlabel, "showPoint");
        tree_->addProperty(labelName, qlabel, "pointType");
        tree_->addProperty(labelName, qlabel, "pointSize");
        tree_->addProperty(labelName, qlabel, "pointWidth");
        tree_->addProperty(labelName, qlabel, "hypertext");
      }
      else if ((poly = dynamic_cast<CGnuPlotPolygon *>(ann.get()))) {
        QString polyName =
          QString("%1/Polygons/%2_%3").arg(groupName).arg(poly->getName()).arg(ann->getInd());

        CQGnuPlotPolygon *qpoly = static_cast<CQGnuPlotPolygon *>(poly);

        tree_->addProperty(polyName, qpoly, "strokeColor");
        tree_->addProperty(polyName, qpoly, "fillColor");
        tree_->addProperty(polyName, qpoly, "clip");
        tree_->addProperty(polyName, qpoly, "drawLayer");
        tree_->addProperty(polyName, qpoly, "lineWidth");
      }
      else if ((rect = dynamic_cast<CGnuPlotRectangle *>(ann.get()))) {
        QString rectName =
          QString("%1/Rectangles/%2_%3").arg(groupName).arg(rect->getName()).arg(ann->getInd());

        CQGnuPlotRectangle *qrect = static_cast<CQGnuPlotRectangle *>(rect);

        tree_->addProperty(rectName, qrect, "strokeColor");
        tree_->addProperty(rectName, qrect, "fillColor");
        tree_->addProperty(rectName, qrect, "clip");
        tree_->addProperty(rectName, qrect, "drawLayer");

        tree_->addProperty(rectName, qrect, "from");
        tree_->addProperty(rectName, qrect, "to");
        tree_->addProperty(rectName, qrect, "fillType");
        tree_->addProperty(rectName, qrect, "lineWidth");
        tree_->addProperty(rectName, qrect, "lineDash");
      }
    }
  }
}

void
CQGnuPlotMainWindow::
addPlotProperties(CGnuPlotPlot *plot)
{
  static int MAX_OBJECTS = 1000;

  CQGnuPlotPlot *qplot = static_cast<CQGnuPlotPlot *>(plot);

  CQGnuPlotGroup *qgroup = static_cast<CQGnuPlotGroup *>(plot->group());

  QString plotName = QString("Group%1/Plot%2").arg(qgroup->id()).arg(plot->id());

  tree_->addProperty(plotName, qplot, "xind");
  tree_->addProperty(plotName, qplot, "yind");

  tree_->addProperty(plotName, qplot, "setNum");
  tree_->addProperty(plotName, qplot, "usingCols");

  tree_->addProperty(plotName, qplot, "displayed");

  tree_->addProperty(plotName, qplot, "binary");
  tree_->addProperty(plotName, qplot, "matrix");
  tree_->addProperty(plotName, qplot, "polar");
  tree_->addProperty(plotName, qplot, "parametric");

  tree_->addProperty(plotName, qplot, "enhanced");

  tree_->addProperty(plotName, qplot, "functions");
  tree_->addProperty(plotName, qplot, "keyTitle");
  tree_->addProperty(plotName, qplot, "samplesNX");
  tree_->addProperty(plotName, qplot, "samplesNY");
  tree_->addProperty(plotName, qplot, "xrangeMin");
  tree_->addProperty(plotName, qplot, "xrangeMax");
  tree_->addProperty(plotName, qplot, "xrangeMin");
  tree_->addProperty(plotName, qplot, "xrangeMax");
  tree_->addProperty(plotName, qplot, "yrangeMin");
  tree_->addProperty(plotName, qplot, "yrangeMax");

  QString rangeName = plotName + "/range";

  tree_->addProperty(rangeName, qplot, "xmin");
  tree_->addProperty(rangeName, qplot, "xmax");
  tree_->addProperty(rangeName, qplot, "ymin");
  tree_->addProperty(rangeName, qplot, "ymax");
  tree_->addProperty(rangeName, qplot, "bymin");
  tree_->addProperty(rangeName, qplot, "bymax");

  if (plot->window()->is3D()) {
    QString surfaceName = plotName + "/surface";

    tree_->addProperty(surfaceName, qplot, "surfaceEnabled");
    tree_->addProperty(surfaceName, qplot, "surfaceColor");

    QString contourName = plotName + "/contour";

    tree_->addProperty(contourName, qplot, "contourEnabled");
    tree_->addProperty(contourName, qplot, "contourLevels")->setEditorFactory(integerEdit("2:100"));

    tree_->addProperty(plotName, qplot, "trianglePattern3D");
  }

  if (plot->isImageStyle()) {
    QString imageName = plotName + "/image";

    tree_->addProperty(imageName, qplot, "imageAngle")->
      setLabel("angle")->setEditorFactory(realEdit("0:360:1"));
  }

  tree_->addProperty(plotName, qplot, "plotStyle");
  tree_->addProperty(plotName, qplot, "lineStyleId");

  tree_->addProperty(plotName, qplot, "fillType"   );
  tree_->addProperty(plotName, qplot, "fillPattern");
  tree_->addProperty(plotName, qplot, "lineColor"  );
  tree_->addProperty(plotName, qplot, "lineWidth"  );
  tree_->addProperty(plotName, qplot, "pointType"  );
  tree_->addProperty(plotName, qplot, "pointSize"  );
  tree_->addProperty(plotName, qplot, "barsSize"   )->setEditorFactory(realEdit("-1000:1000:0.1"));
  tree_->addProperty(plotName, qplot, "barsFront"  );

  QString boxWidthName = plotName + "/boxWidth";

  tree_->addProperty(boxWidthName, qplot, "boxWidthValue")->
    setLabel("value")->setEditorFactory(realEdit("-2:2:0.01"));
  tree_->addProperty(boxWidthName, qplot, "boxWidthType")->
    setLabel("type");
  tree_->addProperty(boxWidthName, qplot, "boxWidthCalc")->
    setLabel("calc");

  //---

  if (! plot->arrowObjects().empty()) {
    //CQGnuPlotPlotArrowObjects *qarrowObjects = qplot->arrowObjectsObj();

    QString arrowsName = QString("%1/Arrows").arg(plotName);

    //---

    int i = 0;

    for (const auto &arrow : plot->arrowObjects()) {
      QString arrowName = QString("%1/Arrow%2").arg(arrowsName).arg(i + 1);

      CQGnuPlotArrowObject *qarrow = static_cast<CQGnuPlotArrowObject *>(arrow);

      tree_->addProperty(arrowName, qarrow, "displayed");
      tree_->addProperty(arrowName, qarrow, "coordType");
      tree_->addProperty(arrowName, qarrow, "from");
      tree_->addProperty(arrowName, qarrow, "to");
      tree_->addProperty(arrowName, qarrow, "length");
      tree_->addProperty(arrowName, qarrow, "angle");
      tree_->addProperty(arrowName, qarrow, "headLength");
      tree_->addProperty(arrowName, qarrow, "headAngle");
      tree_->addProperty(arrowName, qarrow, "headBackAngle");
      tree_->addProperty(arrowName, qarrow, "fhead");
      tree_->addProperty(arrowName, qarrow, "thead");
      tree_->addProperty(arrowName, qarrow, "headFilled");
      tree_->addProperty(arrowName, qarrow, "headEmpty");

      CQGnuPlotStroke *qstroke = qarrow->stroke();

      addStrokeProperties(arrowName, qstroke);

      ++i;
    }
  }

  if (! plot->boxBarObjects().empty()) {
    CQGnuPlotPlotBoxBarObjects *qboxBarObjects = qplot->boxBarObjectsObj();

    QString barsName = QString("%1/BoxBars").arg(plotName);

    tree_->addProperty(barsName, qboxBarObjects, "fillType");
    tree_->addProperty(barsName, qboxBarObjects, "fillPattern");
    tree_->addProperty(barsName, qboxBarObjects, "fillColor");
    tree_->addProperty(barsName, qboxBarObjects, "border");
    tree_->addProperty(barsName, qboxBarObjects, "lineColor");
    tree_->addProperty(barsName, qboxBarObjects, "lineWidth");

    //---

    int i = 0;

    for (const auto &bar : plot->boxBarObjects()) {
      QString barName = QString("%1/Bar%2").arg(barsName).arg(i + 1);

      CQGnuPlotBoxBarObject *qbar = static_cast<CQGnuPlotBoxBarObject *>(bar);

      tree_->addProperty(barName, qbar, "displayed");
      tree_->addProperty(barName, qbar, "x");
      tree_->addProperty(barName, qbar, "y");

      CQGnuPlotFill   *qfill   = qbar->fill();
      CQGnuPlotStroke *qstroke = qbar->stroke();

      addFillProperties  (barName, qfill);
      addStrokeProperties(barName, qstroke);

      int j = 0;

      for (const auto &endBar : qbar->endBars()) {
        CQGnuPlotEndBar *qendBar = static_cast<CQGnuPlotEndBar *>(endBar.get());

        QString endBarName = QString("%1/EndBar%2").arg(barName).arg(j + 1);

        tree_->addProperty(endBarName, qendBar, "startLine");
        tree_->addProperty(endBarName, qendBar, "endLine"  );
        tree_->addProperty(endBarName, qendBar, "endWidth" );

        CQGnuPlotStroke *qbarStroke = qendBar->stroke();

        QString endBarStrokeName = QString("%1/Stroke").arg(endBarName);

        tree_->addProperty(endBarStrokeName, qbarStroke, "enabled");
        tree_->addProperty(endBarStrokeName, qbarStroke, "color");
        tree_->addProperty(endBarStrokeName, qbarStroke, "width");
        tree_->addProperty(endBarStrokeName, qbarStroke, "lineDash");
        tree_->addProperty(endBarStrokeName, qbarStroke, "mitreLimit");

        ++j;
      }

      ++i;
    }
  }

  if (! plot->boxObjects().empty()) {
    //CQGnuPlotPlotBoxObjects *qboxObjects = qplot->boxObjectsObj();

    QString boxesName = QString("%1/Boxes").arg(plotName);

    //---

    int i = 0;

    for (const auto &box : plot->boxObjects()) {
      QString boxName = QString("%1/Box%2").arg(boxesName).arg(i + 1);

      CQGnuPlotBoxObject *qbox = static_cast<CQGnuPlotBoxObject *>(box);

      tree_->addProperty(boxName, qbox, "displayed");
      tree_->addProperty(boxName, qbox, "x");
      tree_->addProperty(boxName, qbox, "y");
      tree_->addProperty(boxName, qbox, "lineWidth");
      tree_->addProperty(boxName, qbox, "boxWidth");

      tree_->addProperty(boxName, qbox, "minValue");
      tree_->addProperty(boxName, qbox, "maxValue");
      tree_->addProperty(boxName, qbox, "medianValue");
      tree_->addProperty(boxName, qbox, "lowerValue");
      tree_->addProperty(boxName, qbox, "upperValue");

      CQGnuPlotFill   *qfill   = qbox->fill();
      CQGnuPlotStroke *qstroke = qbox->stroke();

      addFillProperties  (boxName, qfill);
      addStrokeProperties(boxName, qstroke);

      CQGnuPlotMark *qmark = qbox->outlierMark();

      QString markName = QString("%1/Outlier").arg(boxName);

      tree_->addProperty(markName, qmark, "type");
      tree_->addProperty(markName, qmark, "size");
      tree_->addProperty(markName, qmark, "color");
      tree_->addProperty(markName, qmark, "width");
      tree_->addProperty(markName, qmark, "pixelSize");

      ++i;
    }
  }

  if (! plot->bubbleObjects().empty()) {
    //CQGnuPlotPlotBubbleObjects *qbubbleObjects = qplot->bubbleObjectsObj();

    QString bubblesName = QString("%1/Bubbles").arg(plotName);

    //---

    int i = 0;

    for (const auto &bubble : plot->bubbleObjects()) {
      QString bubbleName = QString("%1/Bubble%2").arg(bubblesName).arg(i + 1);

      CQGnuPlotBubbleObject *qbubble = static_cast<CQGnuPlotBubbleObject *>(bubble);

      tree_->addProperty(bubbleName, qbubble, "displayed");
      tree_->addProperty(bubbleName, qbubble, "name" );
      tree_->addProperty(bubbleName, qbubble, "value");

      CQGnuPlotFill   *qfill   = qbubble->fill();
      CQGnuPlotStroke *qstroke = qbubble->stroke();

      addFillProperties  (bubbleName, qfill);
      addStrokeProperties(bubbleName, qstroke);

      ++i;
    }
  }

  if (! plot->ellipseObjects().empty()) {
    //CQGnuPlotPlotEllipseObjects *qellipseObjects = qplot->ellipseObjectsObj();

    QString ellipsesName = QString("%1/Ellipses").arg(plotName);

    //---

    int i = 0;

    for (const auto &ellipse : plot->ellipseObjects()) {
      QString ellipseName = QString("%1/Ellipse%2").arg(ellipsesName).arg(i + 1);

      CQGnuPlotEllipseObject *qellipse = static_cast<CQGnuPlotEllipseObject *>(ellipse);

      tree_->addProperty(ellipseName, qellipse, "displayed");
      tree_->addProperty(ellipseName, qellipse, "angle");
      tree_->addProperty(ellipseName, qellipse, "text");

      CQGnuPlotFill   *qfill   = qellipse->fill();
      CQGnuPlotStroke *qstroke = qellipse->stroke();

      addFillProperties  (ellipseName, qfill);
      addStrokeProperties(ellipseName, qstroke);

      ++i;
    }
  }

  if (! plot->errorBarObjects().empty()) {
    //CQGnuPlotPlotErrorBarObjects *qerrorBarObjects = qplot->errorBarObjectsObj();

    QString barsName = QString("%1/ErrorBars").arg(plotName);

    //---

    int i = 0;

    for (const auto &bar : plot->errorBarObjects()) {
      QString barName = QString("%1/Bar%2").arg(barsName).arg(i + 1);

      CQGnuPlotErrorBarObject *qbar = static_cast<CQGnuPlotErrorBarObject *>(bar);

      tree_->addProperty(barName, qbar, "displayed");
      tree_->addProperty(barName, qbar, "x");
      tree_->addProperty(barName, qbar, "y");

      if (qbar->isXDirection()) {
        tree_->addProperty(barName, qbar, "xLow");
        tree_->addProperty(barName, qbar, "xHigh");
        tree_->addProperty(barName, qbar, "barHeight");
      }

      if (qbar->isYDirection()) {
        tree_->addProperty(barName, qbar, "yLow");
        tree_->addProperty(barName, qbar, "yHigh");
        tree_->addProperty(barName, qbar, "barWidth");
      }

      CQGnuPlotStroke *qstroke = qbar->stroke();

      addStrokeProperties(barName, qstroke);

      CQGnuPlotMark *qmark = qbar->mark();

      QString markName = QString("%1/Mark").arg(barName);

      tree_->addProperty(markName, qmark, "type");
      tree_->addProperty(markName, qmark, "size");
      tree_->addProperty(markName, qmark, "color");
      tree_->addProperty(markName, qmark, "width");
      tree_->addProperty(markName, qmark, "pixelSize");

      ++i;
    }
  }

  if (! plot->financeBarObjects().empty()) {
    //CQGnuPlotPlotFinanceBarObjects *qfinanceBarObjects = qplot->financeBarObjectsObj();

    QString barsName = QString("%1/FinanceBars").arg(plotName);

    //---

    int i = 0;

    for (const auto &bar : plot->financeBarObjects()) {
      QString barName = QString("%1/Bar%2").arg(barsName).arg(i + 1);

      CQGnuPlotFinanceBarObject *qbar = static_cast<CQGnuPlotFinanceBarObject *>(bar);

      tree_->addProperty(barName, qbar, "displayed");
      tree_->addProperty(barName, qbar, "value");
      tree_->addProperty(barName, qbar, "low");
      tree_->addProperty(barName, qbar, "high");
      tree_->addProperty(barName, qbar, "open");
      tree_->addProperty(barName, qbar, "close");
      tree_->addProperty(barName, qbar, "barWidth");

      CQGnuPlotStroke *qstroke = qbar->stroke();

      addStrokeProperties(barName, qstroke);

      ++i;
    }
  }

  if (! plot->imageObjects().empty()) {
    //CQGnuPlotPlotImageObjects *qimageObjects = qplot->imageObjectsObj();

    QString imagesName = QString("%1/Images").arg(plotName);

    //---

    int i = 0;

    for (const auto &image : plot->imageObjects()) {
      QString imageName = QString("%1/Image%2").arg(imagesName).arg(i + 1);

      CQGnuPlotImageObject *qimage = static_cast<CQGnuPlotImageObject *>(image);

      tree_->addProperty(imageName, qimage, "displayed");
      tree_->addProperty(imageName, qimage, "size");
      tree_->addProperty(imageName, qimage, "origin");
      tree_->addProperty(imageName, qimage, "center");
      tree_->addProperty(imageName, qimage, "delta");
      tree_->addProperty(imageName, qimage, "angle");
      tree_->addProperty(imageName, qimage, "flipX");
      tree_->addProperty(imageName, qimage, "flipY");
      tree_->addProperty(imageName, qimage, "nearest");
      tree_->addProperty(imageName, qimage, "original");
      tree_->addProperty(imageName, qimage, "x");
      tree_->addProperty(imageName, qimage, "y");

      ++i;
    }
  }

  if (! plot->labelObjects().empty()) {
    //CQGnuPlotPlotLabelObjects *qlabelObjects = qplot->labelObjectsObj();

    QString labelsName = QString("%1/Labels").arg(plotName);

    //---

    int i = 0;

    for (const auto &label : plot->labelObjects()) {
      QString labelName = QString("%1/Label%2").arg(labelsName).arg(i + 1);

      CQGnuPlotLabelObject *qlabel = static_cast<CQGnuPlotLabelObject *>(label);

      tree_->addProperty(labelName, qlabel, "displayed");
      tree_->addProperty(labelName, qlabel, "text");
      tree_->addProperty(labelName, qlabel, "pos");
      tree_->addProperty(labelName, qlabel, "color");
      tree_->addProperty(labelName, qlabel, "angle")->setEditorFactory(realSlider("0:360:1"));
      tree_->addProperty(labelName, qlabel, "enhanced");
      tree_->addProperty(labelName, qlabel, "align");
      tree_->addProperty(labelName, qlabel, "font");
      tree_->addProperty(labelName, qlabel, "offset");
      tree_->addProperty(labelName, qlabel, "lineType");
      tree_->addProperty(labelName, qlabel, "showPoint");
      tree_->addProperty(labelName, qlabel, "pointType");
      tree_->addProperty(labelName, qlabel, "pointSize");
      tree_->addProperty(labelName, qlabel, "pointWidth");
      tree_->addProperty(labelName, qlabel, "hypertext");

      CQGnuPlotFill *qboxFill = qlabel->boxFill();

      QString fillName = QString("%1/Fill").arg(labelName);

      tree_->addProperty(fillName, qboxFill, "color");
      tree_->addProperty(fillName, qboxFill, "background");
      tree_->addProperty(fillName, qboxFill, "type");
      tree_->addProperty(fillName, qboxFill, "pattern");

      CQGnuPlotStroke *qboxStroke = qlabel->boxStroke();

      QString strokeName = QString("%1/Stroke").arg(labelName);

      tree_->addProperty(strokeName, qboxStroke, "enabled");
      tree_->addProperty(strokeName, qboxStroke, "color");
      tree_->addProperty(strokeName, qboxStroke, "width");
      tree_->addProperty(strokeName, qboxStroke, "lineDash");
      tree_->addProperty(strokeName, qboxStroke, "mitreLimit");

      ++i;
    }
  }

  if (! plot->pathObjects().empty()) {
    //CQGnuPlotPlotPathObjects *qpathObjects = qplot->pathObjectsObj();

    QString pathsName = QString("%1/Paths").arg(plotName);

    //---

    int i = 0;

    for (const auto &path : plot->pathObjects()) {
      QString pathName = QString("%1/Path%2").arg(pathsName).arg(i + 1);

      CQGnuPlotPathObject *qpath = static_cast<CQGnuPlotPathObject *>(path);

      tree_->addProperty(pathName, qpath, "displayed");
      tree_->addProperty(pathName, qpath, "clipped");

      CQGnuPlotStroke *qstroke = qpath->stroke();

      addStrokeProperties(pathName, qstroke);

      ++i;
    }
  }

  if (! plot->pieObjects().empty()) {
    CQGnuPlotPlotPieObjects *qpieObjects = qplot->pieObjectsObj();

    QString piesName = QString("%1/Pies").arg(plotName);

    tree_->addProperty(piesName, qpieObjects, "innerRadius");
    tree_->addProperty(piesName, qpieObjects, "labelRadius");

    //---

    int i = 0;

    for (const auto &pie : plot->pieObjects()) {
      QString pieName = QString("%1/Pie%2").arg(piesName).arg(i + 1);

      CQGnuPlotPieObject *qpie = static_cast<CQGnuPlotPieObject *>(pie);

      tree_->addProperty(pieName, qpie, "displayed"      );
      tree_->addProperty(pieName, qpie, "name"           );
      tree_->addProperty(pieName, qpie, "value"          );
      tree_->addProperty(pieName, qpie, "innerRadius"    );
      tree_->addProperty(pieName, qpie, "labelRadius"    );
      tree_->addProperty(pieName, qpie, "exploded"       );
      tree_->addProperty(pieName, qpie, "rotatedText"    );
      tree_->addProperty(pieName, qpie, "explodeSelected");

      CQGnuPlotFill   *qfill   = qpie->fill();
      CQGnuPlotStroke *qstroke = qpie->stroke();

      addFillProperties  (pieName, qfill);
      addStrokeProperties(pieName, qstroke);

      ++i;
    }
  }

  //---

  int numPointObjects = plot->pointObjects().size();

  if (numPointObjects > 0 && numPointObjects < MAX_OBJECTS) {
    CQGnuPlotPlotPointObjects *qpointObjects = qplot->pointObjectsObj();

    QString pointsName = QString("%1/Points").arg(plotName);

    tree_->addProperty(pointsName, qpointObjects, "pointType");
    tree_->addProperty(pointsName, qpointObjects, "size");
    tree_->addProperty(pointsName, qpointObjects, "color");

    //---

    int i = 0;

    for (const auto &point : plot->pointObjects()) {
      QString pointName = QString("%1/Point%2").arg(pointsName).arg(i + 1);

      CQGnuPlotPointObject *qpoint = static_cast<CQGnuPlotPointObject *>(point);

      tree_->addProperty(pointName, qpoint, "displayed");
      tree_->addProperty(pointName, qpoint, "point");
      tree_->addProperty(pointName, qpoint, "pointType");
      tree_->addProperty(pointName, qpoint, "size");
      tree_->addProperty(pointName, qpoint, "color");
      tree_->addProperty(pointName, qpoint, "lineWidth");
      tree_->addProperty(pointName, qpoint, "pointString");
      tree_->addProperty(pointName, qpoint, "erasePoint");
      tree_->addProperty(pointName, qpoint, "visible");

      ++i;
    }
  }

  //---

  int numPolygonObjects = plot->polygonObjects().size();

  if (numPolygonObjects > 0 && numPolygonObjects < MAX_OBJECTS) {
    CQGnuPlotPlotPolygonObjects *qpolygonObjects = qplot->polygonObjectsObj();

    QString polygonsName = QString("%1/Polygons").arg(plotName);

    CQGnuPlotFill   *qfill   = qpolygonObjects->fill();
    CQGnuPlotStroke *qstroke = qpolygonObjects->stroke();

    addFillProperties  (polygonsName, qfill);
    addStrokeProperties(polygonsName, qstroke);

    //---

    int i = 0;

    for (const auto &polygon : plot->polygonObjects()) {
      QString polygonName = QString("%1/Polygon%2").arg(polygonsName).arg(i + 1);

      CQGnuPlotPolygonObject *qpolygon = static_cast<CQGnuPlotPolygonObject *>(polygon);

      tree_->addProperty(polygonName, qpolygon, "displayed");
      tree_->addProperty(polygonName, qpolygon, "text");

      CQGnuPlotFill   *qfill   = qpolygon->fill();
      CQGnuPlotStroke *qstroke = qpolygon->stroke();

      addFillProperties  (polygonName, qfill);
      addStrokeProperties(polygonName, qstroke);

      ++i;
    }
  }

  //---

  int numRectObjects = plot->rectObjects().size();

  if (numRectObjects > 0 && numRectObjects < MAX_OBJECTS) {
    //CQGnuPlotPlotRectObjects *qrectObjects = qplot->rectObjectsObj();

    QString rectsName = QString("%1/Rects").arg(plotName);

    //---

    int i = 0;

    for (const auto &rect : plot->rectObjects()) {
      QString rectName = QString("%1/Rect%2").arg(rectsName).arg(i + 1);

      CQGnuPlotRectObject *qrect = static_cast<CQGnuPlotRectObject *>(rect);

      tree_->addProperty(rectName, qrect, "displayed");
      tree_->addProperty(rectName, qrect, "text");

      CQGnuPlotFill   *qfill   = qrect->fill();
      CQGnuPlotStroke *qstroke = qrect->stroke();

      addFillProperties  (rectName, qfill);
      addStrokeProperties(rectName, qstroke);

      ++i;
    }
  }
}

void
CQGnuPlotMainWindow::
addFillProperties(const QString &name, CQGnuPlotFill *qfill)
{
  QString fillName = QString("%1/Fill").arg(name);

  tree_->addProperty(fillName, qfill, "color");
  tree_->addProperty(fillName, qfill, "background");
  tree_->addProperty(fillName, qfill, "type");
  tree_->addProperty(fillName, qfill, "pattern");
}

void
CQGnuPlotMainWindow::
addStrokeProperties(const QString &name, CQGnuPlotStroke *qstroke)
{
  QString strokeName = QString("%1/Stroke").arg(name);

  tree_->addProperty(strokeName, qstroke, "enabled");
  tree_->addProperty(strokeName, qstroke, "color");
  tree_->addProperty(strokeName, qstroke, "width");
  tree_->addProperty(strokeName, qstroke, "lineDash");
  tree_->addProperty(strokeName, qstroke, "lineCap");
  tree_->addProperty(strokeName, qstroke, "lineJoin");
  tree_->addProperty(strokeName, qstroke, "mitreLimit");
}

void
CQGnuPlotMainWindow::
selectObject(const QObject *obj)
{
  tree_->clearSelection();

  tree_->selectObject(obj);
}

void
CQGnuPlotMainWindow::
selectObjects(const Objects &objs)
{
  disconnect(tree_, SIGNAL(itemSelected(QObject *, const QString &)),
             this, SLOT(itemSelectedSlot(QObject *, const QString &)));

  deselectAllObjects();

  tree_->clearSelection();

  for (const auto &o : objs) {
    tree_->selectObject(o);

    CQUtil::setProperty(o, "selected", true);
  }

  connect(tree_, SIGNAL(itemSelected(QObject *, const QString &)),
          this, SLOT(itemSelectedSlot(QObject *, const QString &)));

  redraw();
}

QColor
CQGnuPlotMainWindow::
backgroundColor() const
{
  return toQColor(CGnuPlotWindow::backgroundColor());
}

void
CQGnuPlotMainWindow::
setBackgroundColor(const QColor &c)
{
  CGnuPlotWindow::setBackgroundColor(fromQColor(c));
}

void
CQGnuPlotMainWindow::
redraw()
{
  canvas_->update();
}

void
CQGnuPlotMainWindow::
setCursor(QCursor cursor)
{
  canvas_->setCursor(cursor);
}

void
CQGnuPlotMainWindow::
newWindow()
{
  CGnuPlotWindowP window = qapp()->createNewWindow();

  CQGnuPlotMainWindow *qwindow = dynamic_cast<CQGnuPlotMainWindow *>(window.get());

  qwindow->show();
}

void
CQGnuPlotMainWindow::
loadFile()
{
  CQGnuPlotLoadFileDialog *dialog = new CQGnuPlotLoadFileDialog(this);

  connect(dialog, SIGNAL(accepted()), this, SLOT(loadFileSlot()));

  dialog->exec();
}

void
CQGnuPlotMainWindow::
loadFileSlot()
{
  CQGnuPlotLoadFileDialog *dialog = qobject_cast<CQGnuPlotLoadFileDialog *>(sender());

  bool                     is2D         = dialog->is2D();
  bool                     isParametric = dialog->isParametric();
  bool                     binary       = dialog->isBinary();
  CGnuPlotTypes::PlotStyle style        = dialog->plotStyle();
  QString                  fileName     = dialog->fileName();
  double                   xmin         = dialog->xmin();
  double                   xmax         = dialog->xmax();
  QString                  usingStr     = dialog->usingStr();
  int                      lt           = dialog->lineType();

  bool isImage = false;

  if (fileName != "+" && fileName != "++") {
    QFileInfo fi(fileName);

    isImage = (fi.suffix().toLower() == "png");

    CUnixFile file(fileName.toStdString());

    if (! file.open())
      return;
  }

  initCurrentGroup();

  CGnuPlotGroupP group = *groups().begin();

  group->set2D(  is2D);
  group->set3D(! is2D);

  app()->setParametric(isParametric);

  if (! isParametric)
    app()->setXRange(xmin, xmax);
  else
    app()->setTRange(xmin, xmax);

  CGnuPlotUsingCols usingCols;

  usingCols.parse(usingStr.toStdString());

  if (! isImage) {
    if (! binary) {
      if (fileName == "+" || fileName == "++") {
        CGnuPlotPlotP plot = group->createNewPlot(style);

        plot->setSetNum(0);

        plot->set2D(  is2D);
        plot->set3D(! is2D);

        plot->setParametric(isParametric);

        plot->init();

        if (is2D) {
          if (fileName == "+") {
            app()->initGen1File2D(plot.get(), usingCols);
          }
          else {
            app()->initGen2File2D(plot.get(), usingCols);
          }
        }
        else {
          if (fileName == "+") {
            app()->initGen1File3D(plot.get(), usingCols);
          }
          else {
            app()->initGen2File3D(plot.get(), usingCols);
          }
        }

        CGnuPlotLineStyle lineStyle(app());

        lineStyle.setLineType(lt);

        plot->setLineStyle(lineStyle);
      }
      else {
        CGnuPlotFile dataFile;

        if (! app()->readDataFile(fileName.toStdString(), dataFile))
          return;

        dataFile.processFile();

        for (int setNum = 0; setNum < dataFile.numSets(); ++setNum) {
          CGnuPlotPlotP plot = group->createNewPlot(style);

          plot->setSetNum  (setNum);
          plot->setDataFile(dataFile);

          plot->set2D(  is2D);
          plot->set3D(! is2D);

          plot->init();

          if (plot->is2D())
            app()->setPlotValues2D(plot.get());
          else
            app()->setPlotValues3D(plot.get());

          CGnuPlotLineStyle lineStyle(app());

          lineStyle.setLineType(lt);

          plot->setLineStyle(lineStyle);
        }
      }
    }
    else {
      CGnuPlotPlotP plot = group->createNewPlot(style);

      plot->setSetNum(0);

      plot->set2D(  is2D);
      plot->set3D(! is2D);

      plot->init();

      if (plot->is2D()) {
        if (! app()->readBinaryFile2D(fileName.toStdString(), plot))
          return;
      }
      else {
        if (! app()->readBinaryFile3D(fileName.toStdString(), plot, usingCols))
          return;
      }

      CGnuPlotLineStyle lineStyle(app());

      lineStyle.setLineType(lt);

      plot->setLineStyle(lineStyle);
    }
  }
  else {
    CGnuPlotImageStyle is = app()->imageStyle();

    is.setFileType(CGnuPlotTypes::ImageType::PNG);

    app()->setImageStyle(is);

    style = CGnuPlotTypes::PlotStyle::RGBIMAGE;

    CGnuPlotPlotP plot = app()->addImage2D(group, fileName.toStdString(), style, usingCols);

    plot->setBinary(true);

    CGnuPlotLineStyle lineStyle(app());

    lineStyle.setLineType(lt);

    plot->setLineStyle(lineStyle);

    group->addSubPlot(plot);
  }

  redraw();
}

void
CQGnuPlotMainWindow::
loadFunction()
{
  CQGnuPlotLoadFunctionDialog *dialog = new CQGnuPlotLoadFunctionDialog(this);

  connect(dialog, SIGNAL(accepted()), this, SLOT(loadFunctionSlot()));

  dialog->exec();
}

void
CQGnuPlotMainWindow::
loadFunctionSlot()
{
  CQGnuPlotLoadFunctionDialog *dialog = qobject_cast<CQGnuPlotLoadFunctionDialog *>(sender());

  bool                     is2D         = dialog->is2D();
  bool                     isParametric = dialog->isParametric();
  CGnuPlotTypes::PlotStyle style        = dialog->plotStyle();
  QStringList              functions    = dialog->functions();
  int                      ns           = dialog->samples();
  double                   xmin         = dialog->xmin();
  double                   xmax         = dialog->xmax();
  int                      lt           = dialog->lineType();

  if (dialog->isOverlay())
    initCurrentGroup();
  else
    createTiledGroup();

  CGnuPlotGroupP group = *groups().rbegin();

  if (! group)
    return;

  group->set2D(  is2D);
  group->set3D(! is2D);

  CGnuPlot::Samples samples = app()->samples();

  samples.set(ns, ns);

  app()->setSamples(samples);

  app()->setParametric(isParametric);

  if (! isParametric)
    app()->setXRange(xmin, xmax);
  else
    app()->setTRange(xmin, xmax);

  int fi = (isParametric ? 2 : 1);
  int nf = functions.size()/fi;

  for (int i = 0; i < nf; ++i) {
    CGnuPlot::StringArray funcs;

    for (int j = 0; j < fi; ++j)
      funcs.push_back(functions[i*fi + j].toStdString());

    CGnuPlotPlotP plot;

    if (is2D)
      plot = app()->addFunction2D(group, funcs, style);
    else
      plot = app()->addFunction3D(group, funcs, style);

    //plot->setParametric(isParametric);

    CGnuPlotLineStyle lineStyle(app());

    lineStyle.setLineType(lt);

    plot->setLineStyle(lineStyle);

    group->addSubPlot(plot);
  }

  redraw();
}

void
CQGnuPlotMainWindow::
showData()
{
  delete dataDialog_;

  CQGnuPlotGroup *currentGroup = this->currentGroup();

  if (currentGroup) {
    CGnuPlotPlot *currentPlot = 0;

    for (auto plot : currentGroup->plots())
      currentPlot = plot.get();

    if (currentPlot)
      dataDialog_ = new CQGnuPlotDataDialog(this, currentPlot->dataFile());
    else
      dataDialog_ = new CQGnuPlotDataDialog(this, plot_->dataFile());
  }
  else
    dataDialog_ = new CQGnuPlotDataDialog(this, plot_->dataFile());

  dataDialog_->show();
}

void
CQGnuPlotMainWindow::
manageFunctions()
{
  if (! manageFnsDialog_) {
    manageFnsDialog_ = new CQGnuPlotManageFunctionsDialog(this);

    manageFnsDialog_->init();
  }

  manageFnsDialog_->show();
}

void
CQGnuPlotMainWindow::
manageVariables()
{
  if (! manageVarsDialog_) {
    manageVarsDialog_ = new CQGnuPlotManageVariablesDialog(this);

    manageVarsDialog_->init();
  }

  manageVarsDialog_->show();
}

void
CQGnuPlotMainWindow::
editColorPalette()
{
  if (! paletteDialog_) {
    paletteDialog_ = new CQGnuPlotPaletteDialog(this);

    paletteDialog_->init();

    connect(paletteDialog_, SIGNAL(accepted()), this, SLOT(applyPaletteSlot()));
  }

  paletteDialog_->show();
}

void
CQGnuPlotMainWindow::
applyPaletteSlot()
{
  CQGnuPlotGroup *group = this->currentGroup();

  if (group) {
    group->setPalette(*paletteDialog_->palette());

    for (auto plot : group->plots())
      plot->clearCaches();

    update();
  }
  else
    app()->setPalette(*paletteDialog_->palette());
}

void
CQGnuPlotMainWindow::
createObjects()
{
  if (! createDialog_) {
    createDialog_ = new CQGnuPlotCreateDialog(this);

    createDialog_->init();

    connect(createDialog_, SIGNAL(accepted()), this, SLOT(createObjectsSlot()));
  }

  createDialog_->show();
}

void
CQGnuPlotMainWindow::
createObjectsSlot()
{
  CQGnuPlotCreateDialog *dialog = qobject_cast<CQGnuPlotCreateDialog *>(sender());

  CQGnuPlotGroup *currentGroup = this->currentGroup();

  if (! currentGroup)
    return;

  CGnuPlotTypes::ObjectType objectType = dialog->objectType();

  if      (objectType == CGnuPlotTypes::ObjectType::ARROW) {
    CGnuPlotArrow *arrow = app()->device()->createArrow(currentGroup);

    arrow->autoSetInd();

    arrow->setStrokeColor(dialog->strokeColor());
    arrow->setFillColor  (dialog->fillColor());

    arrow->setFrom(dialog->arrowFrom());
    arrow->setTo  (dialog->arrowTo  ());

    currentGroup->addAnnotation(arrow);
  }
  else if (objectType == CGnuPlotTypes::ObjectType::CIRCLE) {
    CGnuPlotCircle *circle = app()->device()->createCircle(currentGroup);

    circle->autoSetInd();

    circle->setStrokeColor(dialog->strokeColor());
    circle->setLineWidth  (dialog->strokeWidth());
    circle->setLineDash   (dialog->strokeDash());
    circle->setFillColor  (dialog->fillColor());

    circle->setCenter  (dialog->circleCenter());
    circle->setRadius  (dialog->circleRadius());
    circle->setArcStart(dialog->circleStart());
    circle->setArcEnd  (dialog->circleEnd());

    currentGroup->addAnnotation(circle);
  }
  else if (objectType == CGnuPlotTypes::ObjectType::ELLIPSE) {
    CGnuPlotEllipse *ellipse = app()->device()->createEllipse(currentGroup);

    ellipse->autoSetInd();

    ellipse->setStrokeColor(dialog->strokeColor());
    ellipse->setLineWidth  (dialog->strokeWidth());
    ellipse->setLineDash   (dialog->strokeDash());
    ellipse->setFillColor  (dialog->fillColor());

    ellipse->setCenter(dialog->ellipseCenter());
    ellipse->setAngle (dialog->ellipseAngle());

    currentGroup->addAnnotation(ellipse);
  }
  else if (objectType == CGnuPlotTypes::ObjectType::LABEL) {
    CGnuPlotLabel *label = app()->device()->createLabel(currentGroup);

    label->autoSetInd();

    label->setStrokeColor(dialog->strokeColor());
    label->setFillColor  (dialog->fillColor());

    label->setPos  (dialog->labelOrigin());
    label->setText (dialog->labelText());
    label->setAngle(dialog->labelAngle());

    currentGroup->addAnnotation(label);
  }
  else if (objectType == CGnuPlotTypes::ObjectType::POLYGON) {
    CGnuPlotPolygon *polygon = app()->device()->createPolygon(currentGroup);

    polygon->autoSetInd();

    polygon->setStrokeColor(dialog->strokeColor());
    polygon->setLineWidth  (dialog->strokeWidth());
    polygon->setLineDash   (dialog->strokeDash());
    polygon->setFillColor  (dialog->fillColor());

    currentGroup->addAnnotation(polygon);
  }
  else if (objectType == CGnuPlotTypes::ObjectType::RECTANGLE) {
    CGnuPlotRectangle *rectangle = app()->device()->createRectangle(currentGroup);

    rectangle->autoSetInd();

    rectangle->setStrokeColor(dialog->strokeColor());
    rectangle->setLineWidth  (dialog->strokeWidth());
    rectangle->setLineDash   (dialog->strokeDash());
    rectangle->setFillColor  (dialog->fillColor());

    rectangle->setFrom(dialog->rectFrom());
    rectangle->setTo  (dialog->rectTo  ());

    currentGroup->addAnnotation(rectangle);
  }

  redraw();
}

void
CQGnuPlotMainWindow::
saveSlot()
{
  CQGnuPlotSaveDialog *dialog = new CQGnuPlotSaveDialog(this);

  connect(dialog, SIGNAL(accepted()), this, SLOT(saveAcceptSlot()));

  dialog->exec();
}

void
CQGnuPlotMainWindow::
saveAcceptSlot()
{
  CQGnuPlotSaveDialog *dialog = qobject_cast<CQGnuPlotSaveDialog *>(sender());

  bool    isSVG    = dialog->isSVG();
  QString filename = dialog->fileName();

  if (isSVG) {
    if (filename == "")
      filename = "temp.svg";

    saveSVG(filename);
  }
  else {
    if (filename == "")
      filename = "temp.png";

    savePNG(filename, dialog->width(), dialog->height());
  }
}

void
CQGnuPlotMainWindow::
saveSVG(const QString &filename)
{
  app()->pushDevice();

  app()->setDevice("svg");

  app()->setOutputFile(filename.toStdString());

  CGnuPlotSVGRenderer *renderer =
    dynamic_cast<CGnuPlotSVGRenderer *>(app()->device()->renderer());

  renderer->setWindow(this);

  app()->device()->drawInit(this);

  renderer->clear(CGnuPlotWindow::backgroundColor());

  for (auto group : groups()) {
    renderer->setRegion(group->region());

    group->draw();
  }

  app()->device()->drawTerm();

  app()->popDevice();

  app()->setOutputFile("");
}

void
CQGnuPlotMainWindow::
savePNG(const QString &filename, int width, int height)
{
  app()->pushDevice();

  app()->setDevice("png");

  app()->setOutputFile(filename.toStdString());

  CQGnuPlotPNGRenderer *renderer =
    dynamic_cast<CQGnuPlotPNGRenderer *>(app()->device()->renderer());

  renderer->setOutputSize(QSize(width, height));

  renderer->setWindow(this);

  app()->device()->drawInit(this);

  renderer->clear(CGnuPlotWindow::backgroundColor());

  for (auto group : groups()) {
    renderer->setRegion(group->region());

    group->draw();
  }

  app()->device()->drawTerm();

  app()->popDevice();

  app()->setOutputFile("");
}

void
CQGnuPlotMainWindow::
xAxisSlot(bool b)
{
  for (auto group : groups())
    group->showXAxis(b);

  redraw();
}

void
CQGnuPlotMainWindow::
yAxisSlot(bool b)
{
  for (auto group : groups())
    group->showYAxis(b);

  redraw();
}

void
CQGnuPlotMainWindow::
gridSlot(bool b)
{
  for (auto group : groups()) {
    group->setAxisGridDisplayed(CGnuPlotTypes::AxisType::X, 1, b);
    group->setAxisGridDisplayed(CGnuPlotTypes::AxisType::Y, 1, b);
  }

  redraw();
}

void
CQGnuPlotMainWindow::
pixelSlot(bool b)
{
  setShowPixels(b);
}

void
CQGnuPlotMainWindow::
selectMode(bool)
{
  updateMode(Mode::SELECT);
}

void
CQGnuPlotMainWindow::
moveMode(bool)
{
  updateMode(Mode::MOVE);
}

void
CQGnuPlotMainWindow::
zoomMode(bool)
{
  updateMode(Mode::ZOOM);
}

void
CQGnuPlotMainWindow::
probeMode(bool)
{
  updateMode(Mode::PROBE);
}

void
CQGnuPlotMainWindow::
updateMode(Mode mode)
{
  if (mode_ != mode) {
    mode_ = mode;

    selectAction_->setChecked(mode_ == Mode::SELECT);
    moveAction_  ->setChecked(mode_ == Mode::MOVE);
    zoomAction_  ->setChecked(mode_ == Mode::ZOOM);
    probeAction_ ->setChecked(mode_ == Mode::PROBE);

    for (const auto &pw : probeWidgets_)
      delete pw;

    probeWidgets_.clear();
  }
}

void
CQGnuPlotMainWindow::
pointsSlot()
{
  std::vector<QObject *> objs;

  tree_->getSelectedObjects(objs);

  for (const auto &o : objs) {
    CQGnuPlotPlot *qplot = qobject_cast<CQGnuPlotPlot *>(o);
    if (! qplot) continue;

    qplot->printPoints();
  }
}

void
CQGnuPlotMainWindow::
initCurrentGroup()
{
  if (! numGroups()) {
    createNewGroup();

    CGnuPlotGroupP group = *groups().begin();

    group->init();
  }

  CGnuPlotGroupP group = *groups().begin();

  currentGroup_ = static_cast<CQGnuPlotGroup *>(group.get());
}

CQGnuPlotGroup *
CQGnuPlotMainWindow::
createTiledGroup()
{
  createNewGroup();

  int n = groups().size();

  int nr = std::max(int(sqrt(n)), 1);
  int nc = (n + nr - 1)/nr;

  double dx = 1.0/nc;
  double dy = 1.0/nr;

  int i = 0;

  for (auto group : groups()) {
    CQGnuPlotGroup *qgroup = static_cast<CQGnuPlotGroup *>(group.get());

    int r = i / nc;
    int c = i % nc;

    qgroup->setRegion(CBBox2D(c*dx, r*dy, (c + 1)*dx, (r + 1)*dy));

    ++i;
  }

  CGnuPlotGroupP group = *groups().rbegin();

  return static_cast<CQGnuPlotGroup *>(group.get());
}

void
CQGnuPlotMainWindow::
setCurrentGroup(CQGnuPlotGroup *group)
{
  if (group != currentGroup_) {
    currentGroup_ = group;
  }
}

CQGnuPlotGroup *
CQGnuPlotMainWindow::
getGroupAt(const CPoint2D &p)
{
  CGnuPlotMouseEvent mouseEvent;

  mouseEvent.setPixel(p);

  for (auto group : groups()) {
    CQGnuPlotGroup *qgroup = static_cast<CQGnuPlotGroup *>(group.get());

    if (qgroup->inside(mouseEvent))
      return qgroup;
  }

  return 0;
}

void
CQGnuPlotMainWindow::
mousePress(const CGnuPlotMouseEvent &mouseEvent)
{
  escape_ = false;

  if      (mode_ == Mode::SELECT) {
    for (auto group : groups()) {
      CQGnuPlotGroup *qgroup = static_cast<CQGnuPlotGroup *>(group.get());

      qgroup->mousePress(mouseEvent);
    }
  }
  else if (mode_ == Mode::MOVE) {
  }
  else if (mode_ == Mode::ZOOM) {
    zoomPress_   = mouseEvent.pixel();
    zoomRelease_ = zoomPress_;
    zoomGroup_   = currentGroup();
  }
}

void
CQGnuPlotMainWindow::
mouseMove(const CGnuPlotMouseEvent &mouseEvent, bool pressed)
{
  if (mode_ == Mode::SELECT) {
    for (auto group : groups()) {
      CQGnuPlotGroup *qgroup = static_cast<CQGnuPlotGroup *>(group.get());

      qgroup->mouseMove(mouseEvent);
    }
  }
  else if (mode_ == Mode::MOVE) {
  }
  else if (mode_ == Mode::ZOOM) {
    if (pressed) {
      zoomRelease_ = mouseEvent.pixel();

      if (! zoomGroup_->is3D()) {
        int x = std::min(zoomPress_.x, zoomRelease_.x);
        int y = std::min(zoomPress_.y, zoomRelease_.y);

        int w = std::abs(zoomRelease_.x - zoomPress_.x);
        int h = std::abs(zoomRelease_.y - zoomPress_.y);

        if (w > 0 && h > 0) {
          zoomRegion_->setGeometry(QRect(x, y, w, h));

          CPoint2D start, end;

          if (zoomGroup_) {
            zoomGroup_->pixelToWindow(CPoint2D(x    , y    ), start);
            zoomGroup_->pixelToWindow(CPoint2D(x + w, y + h), end  );
          }

          zoomRegion_->setStart(QPointF(start.x, start.y));
          zoomRegion_->setEnd  (QPointF(end  .x, end  .y));

          zoomRegion_->show();
        }
        else {
          zoomRegion_->hide();
        }
      }
      else {
        CGnuPlotCameraP camera = zoomGroup_->camera();

        double dx = zoomRelease_.x - zoomPress_.x;
        double dy = zoomRelease_.y - zoomPress_.y;

        zoomGroup_->setCameraRotateZ(camera->rotateZ() - dx/10);
        zoomGroup_->setCameraRotateX(camera->rotateX() - dy/10);

        zoomPress_ = zoomRelease_;

        redraw();
      }
    }
  }
  else if (mode_ == Mode::PROBE) {
    CQGnuPlotGroup *group = currentGroup();
    if (! group) return;

    CPoint2D start;

    group->pixelToWindow(CPoint2D(mouseEvent.pixel().x, 0                ), start);
  //group->pixelToWindow(CPoint2D(mouseEvent.pixel().x, canvas_->height()), end  );

    CGnuPlotProbeEvent probeEvent;

    probeEvent.setX      (start.x);
    probeEvent.setNearest(true);

    if (group->mouseProbe(probeEvent)) {
      const CGnuPlotProbeEvent::Lines &lines = probeEvent.lines();

      uint nl = lines.size();

      while (probeWidgets_.size() > nl) {
        delete probeWidgets_.back();

        probeWidgets_.pop_back();
      }

      while (probeWidgets_.size() < nl) {
        probeWidgets_.push_back(new ProbeWidget(canvas_));
      }

      probeWidgets_.resize(nl);

      bool concatTips = true;

      std::string tipStr;
      COptReal    concatX;
      COptReal    concatY;

      uint i = 0;

      for (const auto &l : lines) {
        ProbeWidget *pw = probeWidgets_[i];

        CPoint2D p1, p2, p3;

        group->windowToPixel(CPoint2D(l.x(), l.y1()), p1);
        group->windowToPixel(CPoint2D(l.x(), l.y2()), p2);
        group->windowToPixel(CPoint2D(l.x(), l.y3()), p3);

        pw->line->setGeometry(QRect(p1.x, p2.y, 1, p1.y - p2.y));

        pw->line->show();

        if (! concatTips) {
          pw->tip->setHtml(l.tip().c_str());

          QPoint gp = canvas_->mapToGlobal(QPoint(p3.x, p3.y));

          pw->tip->show(gp);
        }
        else {
          if (tipStr != "")
            tipStr += "\n";

          tipStr += l.tip();

          concatX = p3.x;

          concatY.updateMin(p3.y);
        }

        ++i;
      }

      if (concatTips && nl > 0) {
        ProbeWidget *pw = probeWidgets_[0];

        pw->tip->setHtml(tipStr.c_str());

        QPoint gp = canvas_->mapToGlobal(QPoint(concatX.getValue(0), concatY.getValue(0)));

        pw->tip->show(gp);
      }
    }
    else {
      for (const auto &pw : probeWidgets_)
        delete pw;

      probeWidgets_.clear();
    }
  }
}

void
CQGnuPlotMainWindow::
mouseWheel(double d)
{
  if (mode_ == Mode::ZOOM) {
    zoomGroup_ = currentGroup();

    if (zoomGroup_ && zoomGroup_->is3D()) {
      CGnuPlotCameraP camera = zoomGroup_->camera();

      if (d > 0)
        camera->zoomIn();
      else
        camera->zoomOut();

      zoomGroup_->reset3D();

      redraw();
    }
  }
}

void
CQGnuPlotMainWindow::
mouseRelease(const CGnuPlotMouseEvent &)
{
  if      (mode_ == Mode::SELECT) {
  }
  else if (mode_ == Mode::MOVE) {
  }
  else if (mode_ == Mode::ZOOM) {
    if (! zoomGroup_->is3D()) {
      double dx = fabs(zoomRelease_.x - zoomPress_.x);
      double dy = fabs(zoomRelease_.y - zoomPress_.y);

      if (dx >= 4 && dy >= 4) {
        if (zoomGroup_ && ! escape_) {
          if (! zoomGroup_->is3D()) {
            zoomGroup_->setAxisRange(CGnuPlotTypes::AxisType::X, 1,
                                     zoomRegion_->start().x(), zoomRegion_->end().x());
            zoomGroup_->setAxisRange(CGnuPlotTypes::AxisType::Y, 1,
                                     zoomRegion_->start().y(), zoomRegion_->end().y());
          }

          redraw();
        }
      }

      zoomRegion_->hide();
    }
  }
}

void
CQGnuPlotMainWindow::
keyPress(const CGnuPlotKeyEvent &keyEvent)
{
  CQGnuPlotGroup *group = currentGroup();
  if (! group) return;

  int key = keyEvent.key();

  if (key == Qt::Key_Escape) {
    selectMode(true);

    escape_ = true;

    return;
  }

  if      (mode_ == Mode::SELECT) {
    if      (key == Qt::Key_Left || key == Qt::Key_Right) {
      scrollLeftRightKey(group, key == Qt::Key_Left);
    }
    else if (key == Qt::Key_Down || key == Qt::Key_Up) {
      scrollUpDownKey(group, key == Qt::Key_Down);
    }
    else if (key == Qt::Key_Plus) {
      zoomInKey(group);
    }
    else if (key == Qt::Key_Minus) {
      zoomOutKey(group);
    }
    else if (key == Qt::Key_Period) {
      QPoint pos = canvas_->mapFromGlobal(QCursor::pos());

      CPoint2D pixel(pos.x(), pos.y());

      if (CQGnuPlotPositionGrabMgrInst->currentGrab()) {
        CQGnuPlotPositionEdit *edit = CQGnuPlotPositionGrabMgrInst->currentGrab()->edit();

        CGnuPlotPosition pos = edit->position();

        group->updatePosition(pixel, pos);

        edit->setPosition(pos);
      }
      else {
        group->displayPixelCoordinates(pixel);
      }
    }
    else {
      for (auto group : groups()) {
        CQGnuPlotGroup *qgroup = static_cast<CQGnuPlotGroup *>(group.get());

        qgroup->keyPress(keyEvent);
      }
    }
  }
  else if (mode_ == Mode::MOVE) {
    if (key == Qt::Key_Left || key == Qt::Key_Right ||
        key == Qt::Key_Up   || key == Qt::Key_Down) {
      group->moveObjects(key);

      redraw();
    }
  }
  else if (mode_ == Mode::ZOOM) {
    //double xmin = group->getXMin(), ymin = group->getYMin();
    //double xmax = group->getXMax(), ymax = group->getYMax();

    //CPoint2D p1 = group->mapLogPoint(1, 1, 1, CPoint2D(xmin, ymin));
    //CPoint2D p2 = group->mapLogPoint(1, 1, 1, CPoint2D(xmax, ymax));

    //double w = p2.x - p1.x;
    //double h = p2.y - p1.y;

    //double xc = (p1.x + p2.x)/2;
    //double yc = (p1.y + p2.y)/2;

    if      (key == Qt::Key_Left || key == Qt::Key_Right) {
      scrollLeftRightKey(group, key == Qt::Key_Left);
    }
    else if (key == Qt::Key_Down || key == Qt::Key_Up) {
      scrollUpDownKey(group, key == Qt::Key_Down);
    }
    else if (key == Qt::Key_Plus) {
      zoomInKey(group);
    }
    else if (key == Qt::Key_Minus) {
      zoomOutKey(group);
    }
    else if (key == Qt::Key_Home) {
      if (! group->is3D()) {
        //group->restoreRange();

        CQGnuPlotRenderer *qrenderer = qapp()->qrenderer();

        qrenderer->setScale(1.0, 1.0);
        qrenderer->setOffset(CPoint2D(0,0));
      }
      else {
        CGnuPlotCameraP camera = group->camera();

        camera->resetZoom();

        group->reset3D();
      }

      redraw();
    }
  }
}

void
CQGnuPlotMainWindow::
scrollLeftRightKey(CQGnuPlotGroup *group, bool left)
{
  if (! group->is3D()) {
    CQGnuPlotRenderer *qrenderer = qapp()->qrenderer();

    double scale = qrenderer->xscale();

    double xmin = group->getXMin(), ymin = group->getYMin();
    double xmax = group->getXMax(), ymax = group->getYMax();

    CPoint2D p1 = group->mapLogPoint(1, 1, 1, CPoint2D(xmin, ymin));
    CPoint2D p2 = group->mapLogPoint(1, 1, 1, CPoint2D(xmax, ymax));

    double w = p2.x - p1.x;
    //double h = p2.y - p1.y;

    //double xc = (p1.x + p2.x)/2;
    //double yc = (p1.y + p2.y)/2;

    double dx = w/(4*scale);

    if (left)
      dx = -dx;

    //CPoint2D p3 = group->unmapLogPoint(1, 1, 1, CPoint2D(p1.x + dx, p1.y));
    //CPoint2D p4 = group->unmapLogPoint(1, 1, 1, CPoint2D(p2.x + dx, p1.y));

    //group->setAxisRange(CGnuPlotTypes::AxisType::X, 1, p3.x, p4.x);
    qrenderer->setOffset(qrenderer->offset() + CPoint2D(dx, 0));
  }
  else {
    CGnuPlotCameraP camera = group->camera();

    camera->moveDX(left ? -0.01 : 0.01);

    group->reset3D();
  }

  redraw();
}

void
CQGnuPlotMainWindow::
scrollUpDownKey(CQGnuPlotGroup *group, bool down)
{
  if (! group->is3D()) {
    CQGnuPlotRenderer *qrenderer = qapp()->qrenderer();

    double scale = qrenderer->yscale();

    double xmin = group->getXMin(), ymin = group->getYMin();
    double xmax = group->getXMax(), ymax = group->getYMax();

    CPoint2D p1 = group->mapLogPoint(1, 1, 1, CPoint2D(xmin, ymin));
    CPoint2D p2 = group->mapLogPoint(1, 1, 1, CPoint2D(xmax, ymax));

    //double w = p2.x - p1.x;
    double h = p2.y - p1.y;

    //double xc = (p1.x + p2.x)/2;
    //double yc = (p1.y + p2.y)/2;

    double dy = h/(4*scale);

    if (down)
      dy = -dy;

    //CPoint2D p3 = group->unmapLogPoint(1, 1, 1, CPoint2D(p1.x, p1.y + dy));
    //CPoint2D p4 = group->unmapLogPoint(1, 1, 1, CPoint2D(p1.x, p2.y + dy));

    //group->setAxisRange(CGnuPlotTypes::AxisType::Y, 1, p3.y, p4.y);
    qrenderer->setOffset(qrenderer->offset() + CPoint2D(0, dy));
  }
  else {
    CGnuPlotCameraP camera = group->camera();

    camera->moveDY(down ? -0.01 : 0.01);

    group->reset3D();
  }

  redraw();
}

void
CQGnuPlotMainWindow::
zoomInKey(CQGnuPlotGroup *group)
{
  if (! group->is3D()) {
    //CPoint2D p3 = group->unmapLogPoint(1, 1, 1, CPoint2D(xc - w/4, yc - h/4));
    //CPoint2D p4 = group->unmapLogPoint(1, 1, 1, CPoint2D(xc + w/4, yc + h/4));

    //group->setAxisRange(CGnuPlotTypes::AxisType::X, 1, p3.x, p4.x);
    //group->setAxisRange(CGnuPlotTypes::AxisType::Y, 1, p3.y, p4.y);

    CQGnuPlotRenderer *qrenderer = qapp()->qrenderer();

    qrenderer->setXScale(qrenderer->xscale()*2);
    qrenderer->setYScale(qrenderer->yscale()*2);
  }
  else {
    CGnuPlotCameraP camera = group->camera();

    camera->zoomIn();

    group->reset3D();
  }

  redraw();
}

void
CQGnuPlotMainWindow::
zoomOutKey(CQGnuPlotGroup *group)
{
  if (! group->is3D()) {
    //CPoint2D p3 = group->unmapLogPoint(1, 1, 1, CPoint2D(xc - w, yc - h));
    //CPoint2D p4 = group->unmapLogPoint(1, 1, 1, CPoint2D(xc + w, yc + h));

    //group->setAxisRange(CGnuPlotTypes::AxisType::X, 1, p3.x, p4.x);
    //group->setAxisRange(CGnuPlotTypes::AxisType::Y, 1, p3.y, p4.y);

    CQGnuPlotRenderer *qrenderer = qapp()->qrenderer();

    qrenderer->setScale(qrenderer->xscale()/2, qrenderer->yscale()/2);
  }
  else {
    CGnuPlotCameraP camera = group->camera();

    camera->zoomOut();

    group->reset3D();
  }

  redraw();
}

bool
CQGnuPlotMainWindow::
mouseTip(const CGnuPlotMouseEvent &mouseEvent, CGnuPlotTipData &tip)
{
  for (auto group : groups()) {
    CQGnuPlotGroup *qgroup = static_cast<CQGnuPlotGroup *>(group.get());

    if (qgroup->mouseTip(mouseEvent, tip))
      return true;
  }

  highlightObject(0);

  return false;
}

void
CQGnuPlotMainWindow::
paint(QPainter *p)
{
  CQGnuPlotRenderer *qrenderer = qapp()->qrenderer();

  qrenderer->setWidth (canvas_->width ());
  qrenderer->setHeight(canvas_->height());

  qrenderer->setWindow(this);
  qrenderer->setCanvas(canvas());
  qrenderer->setPainter(p);

  for (auto group : groups()) {
    CQGnuPlotGroup *qgroup = static_cast<CQGnuPlotGroup *>(group.get());

    qgroup->setPainter(p);
  }

  p->setRenderHint(QPainter::Antialiasing);
  p->setRenderHint(QPainter::TextAntialiasing);

  p->fillRect(canvas_->rect(), QBrush(QColor(255,255,255)));

  p->setPen(QColor(0,0,0));

  draw();
}

int
CQGnuPlotMainWindow::
pixelWidth () const
{
  return canvas_->width ();
}

int
CQGnuPlotMainWindow::
pixelHeight() const
{
  return canvas_->height();
}

void
CQGnuPlotMainWindow::
showPos(const QString &name, double px, double py, double wx, double wy)
{
  CQGnuPlotGroup *currentGroup = this->currentGroup();

  std::string xstr, ystr;

  if (currentGroup) {
    const CGnuPlotAxisData &xaxis = currentGroup->xaxis(1);
    const CGnuPlotAxisData &yaxis = currentGroup->yaxis(1);

    xstr = xaxis.formatAxisValue(wx);
    ystr = yaxis.formatAxisValue(wy);
  }
  else {
    xstr = CStrUtil::toString(wx);
    ystr = CStrUtil::toString(wy);
  }

  QString text;

  if (name != "")
    text += QString("%1: ").arg(name);

  text += QString("%1 %2").arg(xstr.c_str()).arg(ystr.c_str());

  if (isShowPixels())
    text += QString(" : %1 %2").arg(px).arg(py);

  posLabel_->setText(text);
}

// mouse mode (unused ?)
QPointF
CQGnuPlotMainWindow::
pixelToWindow(const QPoint &p)
{
  return QPointF(p.x(), p.y());
}

void
CQGnuPlotMainWindow::
pixelToWindow(double px, double py, double *wx, double *wy)
{
  *wx = px/(canvas_->width () - 1);
  *wy = py/(canvas_->height() - 1);
}

void
CQGnuPlotMainWindow::
itemSelectedSlot(QObject *obj, const QString & /*path*/)
{
  deselectAllObjects();

  //std::cerr << "Object \'" << qobject->objectName().toStdString() << "\'" << std::endl;

  CQUtil::setProperty(obj, "selected", QVariant(true));

  redraw();
}

void
CQGnuPlotMainWindow::
treeMenuExec(QObject *obj, const QPoint &gpos)
{
  CQGnuPlotGroup *qgroup = qobject_cast<CQGnuPlotGroup *>(obj);

  if (qgroup) {
    QMenu menu;

    menu.addAction("Fit", qgroup, SLOT(fitSlot()));

    menu.exec(gpos);
  }
}

void
CQGnuPlotMainWindow::
deselectAllObjects()
{
  CQGnuPlotDevice *qdevice = qapp()->qdevice();

  for (auto qobject : qdevice->objects())
    CQUtil::setProperty(qobject, "selected", QVariant(false));
}

CQPropertyRealEditor *
CQGnuPlotMainWindow::
realEdit(const std::string &str)
{
  auto p = redits_.find(str);

  if (p == redits_.end()) {
    double start = 0.0, end = 1.0, delta = 0.1;

    std::vector<std::string> fields;

    CStrUtil::addFields(str, fields, ":");

    if (fields.size() > 0) start = CStrUtil::toReal(fields[0]);
    if (fields.size() > 1) end   = CStrUtil::toReal(fields[1]);
    if (fields.size() > 2) delta = CStrUtil::toReal(fields[2]);

    CQPropertyRealEditor *redit = new CQPropertyRealEditor(start, end, delta);

    p = redits_.insert(p, RealEdits::value_type(str, redit));
  }

  return (*p).second;
}

CQPropertyIntegerEditor *
CQGnuPlotMainWindow::
integerEdit(const std::string &str)
{
  auto p = iedits_.find(str);

  if (p == iedits_.end()) {
    int start = 0, end = 100, delta = 1;

    std::vector<std::string> fields;

    CStrUtil::addFields(str, fields, ":");

    if (fields.size() > 0) start = CStrUtil::toInteger(fields[0]);
    if (fields.size() > 1) end   = CStrUtil::toInteger(fields[1]);
    if (fields.size() > 2) delta = CStrUtil::toInteger(fields[2]);

    CQPropertyIntegerEditor *iedit = new CQPropertyIntegerEditor(start, end, delta);

    p = iedits_.insert(p, IntegerEdits::value_type(str, iedit));
  }

  return (*p).second;
}

CQPropertyRealEditor *
CQGnuPlotMainWindow::
realSlider(const std::string &str)
{
  auto p = rsliders_.find(str);

  if (p == rsliders_.end()) {
    double start = 0.0, end = 1.0, delta = 0.1;

    std::vector<std::string> fields;

    CStrUtil::addFields(str, fields, ":");

    if (fields.size() > 0) start = CStrUtil::toReal(fields[0]);
    if (fields.size() > 1) end   = CStrUtil::toReal(fields[1]);
    if (fields.size() > 2) delta = CStrUtil::toReal(fields[2]);

    CQPropertyRealEditor *redit = new CQPropertyRealEditor(start, end, delta);

    redit->setType(CQPropertyRealEditor::Type::ComboSlider);

    p = rsliders_.insert(p, RealSliders::value_type(str, redit));
  }

  return (*p).second;
}

//-------

CQGnuPlotWindow::ProbeWidget::
ProbeWidget(CQGnuPlotCanvas *canvas)
{
  line = new CQRubberBand(QRubberBand::Line, canvas);
  tip  = new CQFloatLabel(canvas);
}

CQGnuPlotWindow::ProbeWidget::
~ProbeWidget()
{
  delete line;
  delete tip;
}
