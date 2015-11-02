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
#include <CQGnuPlotPNGRenderer.h>
#include <CGnuPlotSVGRenderer.h>
#include <CQUtil.h>
#include <CQZoomRegion.h>
#include <CQCursor.h>

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
#include <QRubberBand>
#include <QPushButton>
#include <QLineEdit>

//#include <xpm/select.xpm>
//#include <xpm/zoom.xpm>
//#include <xpm/grid.xpm>
//#include <xpm/xaxis.xpm>
//#include <xpm/yaxis.xpm>

#include <svg/select_svg.h>
#include <svg/move_svg.h>
#include <svg/zoom_svg.h>
#include <svg/grid_svg.h>
#include <svg/xaxis_svg.h>
#include <svg/yaxis_svg.h>

namespace {
  QIcon svgIcon(const uchar *data, int len) {
    QPixmap pixmap;

    pixmap.loadFromData(data, len);

    return QIcon(pixmap);
  }
}

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

  selectAction_ = new QAction(svgIcon(select_data, SELECT_DATA_LEN), "Select", this);
  moveAction_   = new QAction(svgIcon(move_data  , MOVE_DATA_LEN  ), "Move"  , this);
  zoomAction_   = new QAction(svgIcon(zoom_data  , ZOOM_DATA_LEN  ), "Zoom"  , this);

  selectAction_->setCheckable(true);
  moveAction_  ->setCheckable(true);
  zoomAction_  ->setCheckable(true);

  selectAction_->setChecked(true);

  connect(selectAction_, SIGNAL(triggered(bool)), this, SLOT(selectMode(bool)));
  connect(moveAction_  , SIGNAL(triggered(bool)), this, SLOT(moveMode  (bool)));
  connect(zoomAction_  , SIGNAL(triggered(bool)), this, SLOT(zoomMode  (bool)));

  //---

  CQGnuPlotToolBar *toolbar = new CQGnuPlotToolBar("Controls");

  addToolBar(toolbar);

  toolbar->addAction(selectAction_);
  toolbar->addAction(moveAction_);
  toolbar->addAction(zoomAction_);

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

  QAction *saveSVGAction = new QAction("Save S&VG", this);
  QAction *savePNGAction = new QAction("Save &PNG", this);

  fileMenu->addAction(saveSVGAction);
  fileMenu->addAction(savePNGAction);

  connect(saveSVGAction, SIGNAL(triggered()), this, SLOT(saveSVG()));
  connect(savePNGAction, SIGNAL(triggered()), this, SLOT(savePNG()));

  QAction *closeAction = new QAction("&Close", this);

  connect(closeAction, SIGNAL(triggered()), this, SLOT(close()));

  fileMenu->addAction(closeAction);

  //--------

  QMenu *modeMenu = menuBar()->addMenu("&Mode");

  modeMenu->addAction(selectAction_);
  modeMenu->addAction(moveAction_);
  modeMenu->addAction(zoomAction_);

  //------

  QMenu *dispMenu = menuBar()->addMenu("&Display");

//QAction *xAxisAction = new QAction(QIcon(QPixmap(xaxis_data)), "&X Axis", this);
  QAction *xAxisAction = new QAction(svgIcon(xaxis_data, XAXIS_DATA_LEN), "&X Axis", this);
//QAction *yAxisAction = new QAction(QIcon(QPixmap(yaxis_data)), "&Y Axis", this);
  QAction *yAxisAction = new QAction(svgIcon(yaxis_data, YAXIS_DATA_LEN), "&Y Axis", this);
//QAction *gridAction  = new QAction(QIcon(QPixmap(grid_data )), "&Grid"  , this);
  QAction *gridAction  = new QAction(svgIcon(grid_data , GRID_DATA_LEN ), "&Grid"  , this);

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

  //zoomRegion_ = new QRubberBand(QRubberBand::Rectangle, canvas_);
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
    CQGnuPlotGroup *qgroup = static_cast<CQGnuPlotGroup *>(group);

    addGroupProperties(qgroup);

    for (auto subPlot : qgroup->plots())
      addPlotProperties(subPlot);
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

  if (group->is3D()) {
    CQGnuPlotCamera *qcamera = static_cast<CQGnuPlotCamera *>(group->camera().getPtr());

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

    tree_->addProperty(groupName, qgroup, "hidden3D");

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

  tree_->addProperty(regionName, qgroup, "regionLeft"  )->
   setLabel("left")->setEditorFactory(realEdit("-1000:1000:0.1"));
  tree_->addProperty(regionName, qgroup, "regionBottom")->
   setLabel("bottom")->setEditorFactory(realEdit("-1000:1000:0.1"));
  tree_->addProperty(regionName, qgroup, "regionRight" )->
   setLabel("right")->setEditorFactory(realEdit("-1000:1000:0.1"));
  tree_->addProperty(regionName, qgroup, "regionTop"   )->
   setLabel("top")->setEditorFactory(realEdit("-1000:1000:0.1"));

  QString marginName = groupName + "/margin";

  tree_->addProperty(marginName, qgroup, "marginLeft"  )->
   setLabel("left")->setEditorFactory(realEdit("0:50:1"));
  tree_->addProperty(marginName, qgroup, "marginRight" )->
   setLabel("right")->setEditorFactory(realEdit("0:50:1"));
  tree_->addProperty(marginName, qgroup, "marginTop"   )->
   setLabel("top")->setEditorFactory(realEdit("0:50:1"));
  tree_->addProperty(marginName, qgroup, "marginBottom")->
   setLabel("bottom")->setEditorFactory(realEdit("0:50:1"));

  tree_->addProperty(groupName, qgroup, "ratio")->setEditorFactory(realEdit("-1000:1000:0.1"));

  tree_->addProperty(groupName, qgroup, "histogramStyle");

  tree_->addProperty(groupName, qgroup, "histogramGap");

  //---

  CQGnuPlotTitle *qtitle = static_cast<CQGnuPlotTitle *>(group->title().get());

  QString titleName = QString("%1/title").arg(groupName);

  tree_->addProperty(titleName, qtitle, "text"    );
  tree_->addProperty(titleName, qtitle, "offset"  );
  tree_->addProperty(titleName, qtitle, "font"    );
  tree_->addProperty(titleName, qtitle, "color"   );
  tree_->addProperty(titleName, qtitle, "enhanced");

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
  tree_->addProperty(keyName, qkey, "lmargin");
  tree_->addProperty(keyName, qkey, "rmargin");
  tree_->addProperty(keyName, qkey, "tmargin");
  tree_->addProperty(keyName, qkey, "bmargin");

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
      }
    }
  }
}

void
CQGnuPlotMainWindow::
addPlotProperties(CGnuPlotPlot *plot)
{
  CQGnuPlotPlot *qplot = static_cast<CQGnuPlotPlot *>(plot);

  CQGnuPlotGroup *qgroup = static_cast<CQGnuPlotGroup *>(plot->group());

  QString plotName = QString("Group%1/Plot%2").arg(qgroup->id()).arg(plot->id());

  tree_->addProperty(plotName, qplot, "xind");
  tree_->addProperty(plotName, qplot, "yind");

  tree_->addProperty(plotName, qplot, "displayed");

  tree_->addProperty(plotName, qplot, "binary");
  tree_->addProperty(plotName, qplot, "matrix");

  tree_->addProperty(plotName, qplot, "enhanced");

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
    int i = 0;

    for (const auto &arrow : plot->arrowObjects()) {
      QString arrowName = QString("%1/Arrows/Arrow%2").arg(plotName).arg(i + 1);

      CQGnuPlotArrowObject *qarrow = static_cast<CQGnuPlotArrowObject *>(arrow);

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

      QString strokeName = QString("%1/Stroke").arg(arrowName);

    //tree_->addProperty(strokeName, qstroke, "enabled");
      tree_->addProperty(strokeName, qstroke, "color");
      tree_->addProperty(strokeName, qstroke, "width")->setEditorFactory(realEdit("0:50:1"));
      tree_->addProperty(strokeName, qstroke, "lineDash");
      tree_->addProperty(strokeName, qstroke, "mitreLimit");

      ++i;
    }
  }

  if (! plot->boxBarObjects().empty()) {
    QString barsName = QString("%1/BoxBars").arg(plotName);

    CQGnuPlotPlotBoxBarObjects *qboxBarObjects = qplot->boxBarObjectsObj();

    tree_->addProperty(barsName, qboxBarObjects, "fillType");
    tree_->addProperty(barsName, qboxBarObjects, "fillPattern");
    tree_->addProperty(barsName, qboxBarObjects, "fillColor");
    tree_->addProperty(barsName, qboxBarObjects, "border");
    tree_->addProperty(barsName, qboxBarObjects, "lineColor");
    tree_->addProperty(barsName, qboxBarObjects, "lineWidth");

    int i = 0;

    for (const auto &bar : plot->boxBarObjects()) {
      QString barName = QString("%1/Bar%2").arg(barsName).arg(i + 1);

      CQGnuPlotBoxBarObject *qbar = static_cast<CQGnuPlotBoxBarObject *>(bar);

      tree_->addProperty(barName, qbar, "x");
      tree_->addProperty(barName, qbar, "y");

      CQGnuPlotFill *qfill = qbar->fill();

      QString fillName = QString("%1/Fill").arg(barName);

      tree_->addProperty(fillName, qfill, "color");
      tree_->addProperty(fillName, qfill, "background");
      tree_->addProperty(fillName, qfill, "type");
      tree_->addProperty(fillName, qfill, "pattern");

      CQGnuPlotStroke *qstroke = qbar->stroke();

      QString strokeName = QString("%1/Stroke").arg(barName);

      tree_->addProperty(strokeName, qstroke, "enabled");
      tree_->addProperty(strokeName, qstroke, "color");
      tree_->addProperty(strokeName, qstroke, "width");
      tree_->addProperty(strokeName, qstroke, "lineDash");
      tree_->addProperty(strokeName, qstroke, "mitreLimit");

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
    QString boxesName = QString("%1/Boxes").arg(plotName);

    int i = 0;

    for (const auto &box : plot->boxObjects()) {
      QString boxName = QString("%1/Box%2").arg(boxesName).arg(i + 1);

      CQGnuPlotBoxObject *qbox = static_cast<CQGnuPlotBoxObject *>(box);

      tree_->addProperty(boxName, qbox, "x");
      tree_->addProperty(boxName, qbox, "y");
      tree_->addProperty(boxName, qbox, "lineWidth");
      tree_->addProperty(boxName, qbox, "boxWidth");

      tree_->addProperty(boxName, qbox, "minValue");
      tree_->addProperty(boxName, qbox, "maxValue");
      tree_->addProperty(boxName, qbox, "medianValue");
      tree_->addProperty(boxName, qbox, "lowerValue");
      tree_->addProperty(boxName, qbox, "upperValue");

      CQGnuPlotFill *qfill = qbox->fill();

      QString fillName = QString("%1/Fill").arg(boxName);

      tree_->addProperty(fillName, qfill, "color");
      tree_->addProperty(fillName, qfill, "background");
      tree_->addProperty(fillName, qfill, "type");
      tree_->addProperty(fillName, qfill, "pattern");

      CQGnuPlotStroke *qstroke = qbox->stroke();

      QString strokeName = QString("%1/Stroke").arg(boxName);

      tree_->addProperty(strokeName, qstroke, "enabled");
      tree_->addProperty(strokeName, qstroke, "color");
      tree_->addProperty(strokeName, qstroke, "width");
      tree_->addProperty(strokeName, qstroke, "lineDash");
      tree_->addProperty(strokeName, qstroke, "mitreLimit");

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
    int i = 0;

    for (const auto &bubble : plot->bubbleObjects()) {
      QString bubbleName = QString("%1/Bubbles/Bubble%2").arg(plotName).arg(i + 1);

      CQGnuPlotBubbleObject *qbubble = static_cast<CQGnuPlotBubbleObject *>(bubble);

      tree_->addProperty(bubbleName, qbubble, "name" );
      tree_->addProperty(bubbleName, qbubble, "value");

      CQGnuPlotFill *qfill = qbubble->fill();

      QString fillName = QString("%1/Fill").arg(bubbleName);

      tree_->addProperty(fillName, qfill, "color");
      tree_->addProperty(fillName, qfill, "background");
      tree_->addProperty(fillName, qfill, "type");
      tree_->addProperty(fillName, qfill, "pattern");

      CQGnuPlotStroke *qstroke = qbubble->stroke();

      QString strokeName = QString("%1/Stroke").arg(bubbleName);

      tree_->addProperty(strokeName, qstroke, "enabled");
      tree_->addProperty(strokeName, qstroke, "color");
      tree_->addProperty(strokeName, qstroke, "width");
      tree_->addProperty(strokeName, qstroke, "lineDash");
      tree_->addProperty(strokeName, qstroke, "mitreLimit");

      ++i;
    }
  }

  if (! plot->ellipseObjects().empty()) {
    int i = 0;

    for (const auto &ellipse : plot->ellipseObjects()) {
      QString ellipseName = QString("%1/Ellipses/Ellipse%2").arg(plotName).arg(i + 1);

      CQGnuPlotEllipseObject *qellipse = static_cast<CQGnuPlotEllipseObject *>(ellipse);

      tree_->addProperty(ellipseName, qellipse, "angle");
      tree_->addProperty(ellipseName, qellipse, "text");

      CQGnuPlotFill *qfill = qellipse->fill();

      QString fillName = QString("%1/Fill").arg(ellipseName);

      tree_->addProperty(fillName, qfill, "color");
      tree_->addProperty(fillName, qfill, "background");
      tree_->addProperty(fillName, qfill, "type");
      tree_->addProperty(fillName, qfill, "pattern");

      CQGnuPlotStroke *qstroke = qellipse->stroke();

      QString strokeName = QString("%1/Stroke").arg(ellipseName);

      tree_->addProperty(strokeName, qstroke, "enabled");
      tree_->addProperty(strokeName, qstroke, "color");
      tree_->addProperty(strokeName, qstroke, "width");
      tree_->addProperty(strokeName, qstroke, "lineDash");
      tree_->addProperty(strokeName, qstroke, "mitreLimit");

      ++i;
    }
  }

  if (! plot->errorBarObjects().empty()) {
    QString barsName = QString("%1/ErrorBars").arg(plotName);

    int i = 0;

    for (const auto &bar : plot->errorBarObjects()) {
      QString barName = QString("%1/Bar%2").arg(barsName).arg(i + 1);

      CQGnuPlotErrorBarObject *qbar = static_cast<CQGnuPlotErrorBarObject *>(bar);

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

      QString strokeName = QString("%1/Stroke").arg(barName);

      tree_->addProperty(strokeName, qstroke, "enabled");
      tree_->addProperty(strokeName, qstroke, "color");
      tree_->addProperty(strokeName, qstroke, "width");
      tree_->addProperty(strokeName, qstroke, "lineDash");
      tree_->addProperty(strokeName, qstroke, "mitreLimit");

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
    QString barsName = QString("%1/FinanceBars").arg(plotName);

    int i = 0;

    for (const auto &bar : plot->financeBarObjects()) {
      QString barName = QString("%1/Bar%2").arg(barsName).arg(i + 1);

      CQGnuPlotFinanceBarObject *qbar = static_cast<CQGnuPlotFinanceBarObject *>(bar);

      tree_->addProperty(barName, qbar, "value");
      tree_->addProperty(barName, qbar, "low");
      tree_->addProperty(barName, qbar, "high");
      tree_->addProperty(barName, qbar, "open");
      tree_->addProperty(barName, qbar, "close");
      tree_->addProperty(barName, qbar, "barWidth");

      CQGnuPlotStroke *qstroke = qbar->stroke();

      QString strokeName = QString("%1/Stroke").arg(barName);

      tree_->addProperty(strokeName, qstroke, "enabled");
      tree_->addProperty(strokeName, qstroke, "color");
      tree_->addProperty(strokeName, qstroke, "width");
      tree_->addProperty(strokeName, qstroke, "lineDash");
      tree_->addProperty(strokeName, qstroke, "mitreLimit");

      ++i;
    }
  }

  if (! plot->imageObjects().empty()) {
    int i = 0;

    for (const auto &image : plot->imageObjects()) {
      QString imageName = QString("%1/Images/Image%2").arg(plotName).arg(i + 1);

      CQGnuPlotImageObject *qimage = static_cast<CQGnuPlotImageObject *>(image);

      tree_->addProperty(imageName, qimage, "x");
      tree_->addProperty(imageName, qimage, "y");
      tree_->addProperty(imageName, qimage, "angle");
      tree_->addProperty(imageName, qimage, "flipY");

      ++i;
    }
  }

  if (! plot->labelObjects().empty()) {
    int i = 0;

    for (const auto &label : plot->labelObjects()) {
      QString labelName = QString("%1/Labels/Label%2").arg(plotName).arg(i + 1);

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
    int i = 0;

    for (const auto &path : plot->pathObjects()) {
      QString pathName = QString("%1/Paths/Path%2").arg(plotName).arg(i + 1);

      CQGnuPlotPathObject *qpath = static_cast<CQGnuPlotPathObject *>(path);

      tree_->addProperty(pathName, qpath, "clipped");

      CQGnuPlotStroke *qstroke = qpath->stroke();

      QString strokeName = QString("%1/Stroke").arg(pathName);

      tree_->addProperty(strokeName, qstroke, "enabled");
      tree_->addProperty(strokeName, qstroke, "color");
      tree_->addProperty(strokeName, qstroke, "width");
      tree_->addProperty(strokeName, qstroke, "lineDash");
      tree_->addProperty(strokeName, qstroke, "mitreLimit");

      ++i;
    }
  }

  if (! plot->pieObjects().empty()) {
    QString piesName = QString("%1/Pies").arg(plotName);

    CQGnuPlotPlotPieObjects *qpieObjects = qplot->pieObjectsObj();

    tree_->addProperty(piesName, qpieObjects, "innerRadius");
    tree_->addProperty(piesName, qpieObjects, "labelRadius");

    int i = 0;

    for (const auto &pie : plot->pieObjects()) {
      QString pieName = QString("%1/Pie%2").arg(piesName).arg(i + 1);

      CQGnuPlotPieObject *qpie = static_cast<CQGnuPlotPieObject *>(pie);

      tree_->addProperty(pieName, qpie, "name"       );
      tree_->addProperty(pieName, qpie, "value"      );
      tree_->addProperty(pieName, qpie, "innerRadius");
      tree_->addProperty(pieName, qpie, "labelRadius");
      tree_->addProperty(pieName, qpie, "exploded"   );

      CQGnuPlotFill *qfill = qpie->fill();

      QString fillName = QString("%1/Fill").arg(pieName);

      tree_->addProperty(fillName, qfill, "color");
      tree_->addProperty(fillName, qfill, "background");
      tree_->addProperty(fillName, qfill, "type");
      tree_->addProperty(fillName, qfill, "pattern");

      CQGnuPlotStroke *qstroke = qpie->stroke();

      QString strokeName = QString("%1/Stroke").arg(pieName);

      tree_->addProperty(strokeName, qstroke, "enabled");
      tree_->addProperty(strokeName, qstroke, "color");
      tree_->addProperty(strokeName, qstroke, "width");
      tree_->addProperty(strokeName, qstroke, "lineDash");
      tree_->addProperty(strokeName, qstroke, "mitreLimit");

      ++i;
    }
  }

  if (! plot->pointObjects().empty()) {
    int i = 0;

    for (const auto &point : plot->pointObjects()) {
      QString pointName = QString("%1/Points/Point%2").arg(plotName).arg(i + 1);

      CQGnuPlotPointObject *qpoint = static_cast<CQGnuPlotPointObject *>(point);

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

  int np = plot->polygonObjects().size();

  if (np > 0 && np < 1000) {// TODO: limits
    int i = 0;

    for (const auto &polygon : plot->polygonObjects()) {
      QString polygonName = QString("%1/Polygons/Polygon%2").arg(plotName).arg(i + 1);

      CQGnuPlotPolygonObject *qpolygon = static_cast<CQGnuPlotPolygonObject *>(polygon);

      tree_->addProperty(polygonName, qpolygon, "text");

      CQGnuPlotFill *qfill = qpolygon->fill();

      QString fillName = QString("%1/Fill").arg(polygonName);

      tree_->addProperty(fillName, qfill, "color");
      tree_->addProperty(fillName, qfill, "background");
      tree_->addProperty(fillName, qfill, "type");
      tree_->addProperty(fillName, qfill, "pattern");

      CQGnuPlotStroke *qstroke = qpolygon->stroke();

      QString strokeName = QString("%1/Stroke").arg(polygonName);

      tree_->addProperty(strokeName, qstroke, "enabled");
      tree_->addProperty(strokeName, qstroke, "color");
      tree_->addProperty(strokeName, qstroke, "width");
      tree_->addProperty(strokeName, qstroke, "lineDash");
      tree_->addProperty(strokeName, qstroke, "mitreLimit");

      ++i;
    }
  }

  if (! plot->rectObjects().empty()) {
    int i = 0;

    for (const auto &rect : plot->rectObjects()) {
      QString rectName = QString("%1/Rects/Rect%2").arg(plotName).arg(i + 1);

      CQGnuPlotRectObject *qrect = static_cast<CQGnuPlotRectObject *>(rect);

      tree_->addProperty(rectName, qrect, "text");

      CQGnuPlotFill *qfill = qrect->fill();

      QString fillName = QString("%1/Fill").arg(rectName);

      tree_->addProperty(fillName, qfill, "color");
      tree_->addProperty(fillName, qfill, "background");
      tree_->addProperty(fillName, qfill, "type");
      tree_->addProperty(fillName, qfill, "pattern");

      CQGnuPlotStroke *qstroke = qrect->stroke();

      QString strokeName = QString("%1/Stroke").arg(rectName);

      tree_->addProperty(strokeName, qstroke, "enabled");
      tree_->addProperty(strokeName, qstroke, "color");
      tree_->addProperty(strokeName, qstroke, "width");
      tree_->addProperty(strokeName, qstroke, "lineDash");
      tree_->addProperty(strokeName, qstroke, "mitreLimit");

      ++i;
    }
  }
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
saveSVG()
{
  app()->pushDevice();

  app()->setDevice("svg");

  app()->setOutputFile("temp.svg");

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
savePNG()
{
  app()->pushDevice();

  app()->setDevice("png");

  app()->setOutputFile("temp.png");

  CQGnuPlotPNGRenderer *renderer =
    dynamic_cast<CQGnuPlotPNGRenderer *>(app()->device()->renderer());

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
  mode_ = Mode::SELECT;

  selectAction_->setChecked(true);
  moveAction_  ->setChecked(false);
  zoomAction_  ->setChecked(false);
}

void
CQGnuPlotMainWindow::
moveMode(bool)
{
  mode_ = Mode::MOVE;

  selectAction_->setChecked(false);
  moveAction_  ->setChecked(true);
  zoomAction_  ->setChecked(false);
}


void
CQGnuPlotMainWindow::
zoomMode(bool)
{
  mode_ = Mode::ZOOM;

  selectAction_->setChecked(false);
  moveAction_  ->setChecked(false);
  zoomAction_  ->setChecked(true);
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
getGroupAt(const QPoint &pos)
{
  for (auto group : groups()) {
    CQGnuPlotGroup *qgroup = static_cast<CQGnuPlotGroup *>(group);

    if (qgroup->inside(pos))
      return qgroup;
  }

  return 0;
}

void
CQGnuPlotMainWindow::
mousePress(const QPoint &qp)
{
  escape_ = false;

  if      (mode_ == Mode::SELECT) {
    for (auto group : groups()) {
      CQGnuPlotGroup *qgroup = static_cast<CQGnuPlotGroup *>(group);

      qgroup->mousePress(qp);
    }
  }
  else if (mode_ == Mode::MOVE) {
  }
  else if (mode_ == Mode::ZOOM) {
    zoomPress_   = qp;
    zoomRelease_ = zoomPress_;
    zoomGroup_   = currentGroup();
  }
}

void
CQGnuPlotMainWindow::
mouseMove(const QPoint &qp, bool pressed)
{
  if (mode_ == Mode::SELECT) {
    for (auto group : groups()) {
      CQGnuPlotGroup *qgroup = static_cast<CQGnuPlotGroup *>(group);

      qgroup->mouseMove(qp);
    }
  }
  else if (mode_ == Mode::MOVE) {
  }
  else if (mode_ == Mode::ZOOM) {
    if (pressed) {
      zoomRelease_ = qp;

      int x = std::min(zoomPress_.x(), zoomRelease_.x());
      int y = std::min(zoomPress_.y(), zoomRelease_.y());

      int w = std::abs(zoomRelease_.x() - zoomPress_.x());
      int h = std::abs(zoomRelease_.y() - zoomPress_.y());

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
  }
}

void
CQGnuPlotMainWindow::
mouseRelease(const QPoint &)
{
  if      (mode_ == Mode::SELECT) {
  }
  else if (mode_ == Mode::MOVE) {
  }
  else if (mode_ == Mode::ZOOM) {
    int dx = abs(zoomRelease_.x() - zoomPress_.x());
    int dy = abs(zoomRelease_.y() - zoomPress_.y());

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

void
CQGnuPlotMainWindow::
keyPress(int key, Qt::KeyboardModifiers /*modifiers*/)
{
  CQGnuPlotGroup *group = currentGroup();
  if (! group) return;

  CGnuPlotCameraP camera = group->camera();

  if      (mode_ == Mode::SELECT) {
  }
  else if (mode_ == Mode::MOVE) {
    if (key == Qt::Key_Left || key == Qt::Key_Right ||
        key == Qt::Key_Up   || key == Qt::Key_Down) {
      group->moveObjects(key);

      redraw();
    }
  }
  else if (mode_ == Mode::ZOOM) {
    double xmin = group->getXMin(), ymin = group->getYMin();
    double xmax = group->getXMax(), ymax = group->getYMax();

    CPoint2D p1 = group->mapLogPoint(1, 1, 1, CPoint2D(xmin, ymin));
    CPoint2D p2 = group->mapLogPoint(1, 1, 1, CPoint2D(xmax, ymax));

    double w = p2.x - p1.x;
    double h = p2.y - p1.y;

    double xc = (p1.x + p2.x)/2;
    double yc = (p1.y + p2.y)/2;

    if      (key == Qt::Key_Left || key == Qt::Key_Right) {
      if (! group->is3D()) {
        double dx = w/4;

        if (key == Qt::Key_Left)
          dx = -dx;

        CPoint2D p3 = group->unmapLogPoint(1, 1, 1, CPoint2D(p1.x + dx, p1.y));
        CPoint2D p4 = group->unmapLogPoint(1, 1, 1, CPoint2D(p2.x + dx, p1.y));

        group->setAxisRange(CGnuPlotTypes::AxisType::X, 1, p3.x, p4.x);
      }
      else {
        camera->moveDX(key == Qt::Key_Left ? -0.01 : 0.01);

        group->reset3D();
      }

      redraw();
    }
    else if (key == Qt::Key_Down || key == Qt::Key_Up) {
      if (! group->is3D()) {
        double dy = h/4;

        if (key == Qt::Key_Down)
          dy = -dy;

        CPoint2D p3 = group->unmapLogPoint(1, 1, 1, CPoint2D(p1.x, p1.y + dy));
        CPoint2D p4 = group->unmapLogPoint(1, 1, 1, CPoint2D(p1.x, p2.y + dy));

        group->setAxisRange(CGnuPlotTypes::AxisType::Y, 1, p3.y, p4.y);
      }
      else {
        camera->moveDY(key == Qt::Key_Down ? -0.01 : 0.01);

        group->reset3D();
      }

      redraw();
    }
    else if (key == Qt::Key_Plus) {
      if (! group->is3D()) {
        CPoint2D p3 = group->unmapLogPoint(1, 1, 1, CPoint2D(xc - w/4, yc - h/4));
        CPoint2D p4 = group->unmapLogPoint(1, 1, 1, CPoint2D(xc + w/4, yc + h/4));

        group->setAxisRange(CGnuPlotTypes::AxisType::X, 1, p3.x, p4.x);
        group->setAxisRange(CGnuPlotTypes::AxisType::Y, 1, p3.y, p4.y);
      }
      else {
        camera->zoomIn();

        group->reset3D();
      }

      redraw();
    }
    else if (key == Qt::Key_Minus) {
      if (! group->is3D()) {
        CPoint2D p3 = group->unmapLogPoint(1, 1, 1, CPoint2D(xc - w, yc - h));
        CPoint2D p4 = group->unmapLogPoint(1, 1, 1, CPoint2D(xc + w, yc + h));

        group->setAxisRange(CGnuPlotTypes::AxisType::X, 1, p3.x, p4.x);
        group->setAxisRange(CGnuPlotTypes::AxisType::Y, 1, p3.y, p4.y);
      }
      else {
        camera->zoomOut();

        group->reset3D();
      }

      redraw();
    }
    else if (key == Qt::Key_Home) {
      group->restoreRange();

      redraw();
    }
    else if (key == Qt::Key_Escape)
      escape_ = true;
  }
}

bool
CQGnuPlotMainWindow::
mouseTip(const QPoint &qp, CGnuPlotTipData &tip)
{
  for (auto group : groups()) {
    CQGnuPlotGroup *qgroup = static_cast<CQGnuPlotGroup *>(group);

    if (qgroup->mouseTip(qp, tip))
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
    CQGnuPlotGroup *qgroup = static_cast<CQGnuPlotGroup *>(group);

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
  std::string xstr, ystr;

  if (currentGroup_) {
    const CGnuPlotAxisData &xaxis = currentGroup_->xaxis(1);
    const CGnuPlotAxisData &yaxis = currentGroup_->yaxis(1);

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
