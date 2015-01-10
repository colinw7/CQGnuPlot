#include <CGnuPlotWindow.h>
#include <CGnuPlotGroup.h>
#include <CGnuPlotPlot.h>
#include <CGnuPlotDevice.h>
#include <CGnuPlotRenderer.h>

CGnuPlotWindow::
CGnuPlotWindow(CGnuPlot *plot) :
 plot_(plot), size_(plot->getTerminalSize())
{
}

CGnuPlotWindow::
~CGnuPlotWindow()
{
}

void
CGnuPlotWindow::
clear()
{
  is3D_ = false;

  groups_.clear();

  title_  = CGnuPlot::Title ();
  camera_ = CGnuPlot::Camera();

  hidden3D_  = false;
  surface3D_ = false;
  contour3D_ = false;
  pm3D_      = false;
}

void
CGnuPlotWindow::
set3D(bool b)
{
  is3D_ = b;
}

void
CGnuPlotWindow::
setCameraEnabled(bool b)
{
  camera_.enabled = b;

  reset3D();
}

void
CGnuPlotWindow::
setCameraRotateX(double a)
{
  camera_.rotateX = a;

  camera_.init();

  reset3D();
}

void
CGnuPlotWindow::
setCameraRotateY(double a)
{
  camera_.rotateY = a;

  camera_.init();

  reset3D();
}

void
CGnuPlotWindow::
setCameraRotateZ(double a)
{
  camera_.rotateZ = a;

  camera_.init();

  reset3D();
}

void
CGnuPlotWindow::
setCameraXMin(double x)
{
  camera_.xmin = x;

  camera_.init();

  reset3D();
}

void
CGnuPlotWindow::
setCameraXMax(double x)
{
  camera_.xmax = x;

  camera_.init();

  reset3D();
}

void
CGnuPlotWindow::
setCameraYMin(double y)
{
  camera_.ymin = y;

  camera_.init();

  reset3D();
}

void
CGnuPlotWindow::
setCameraYMax(double y)
{
  camera_.ymax = y;

  camera_.init();

  reset3D();
}

void
CGnuPlotWindow::
setCameraNear(double z)
{
  camera_.near = z;

  camera_.init();

  reset3D();
}

void
CGnuPlotWindow::
setCameraFar(double z)
{
  camera_.far = z;

  camera_.init();

  reset3D();
}

void
CGnuPlotWindow::
reset3D()
{
  for (auto group : groups_)
    group->reset3D();
}

void
CGnuPlotWindow::
addGroup(CGnuPlotGroup *group)
{
  if (plot_->isDebug())
    std::cerr << "Add Group" << std::endl;

  group->setInd(groups_.size() + 1);

  groups_.push_back(group);
}

void
CGnuPlotWindow::
draw()
{
  app()->renderer()->setWindow(this);

  app()->device()->drawInit(this);

  drawTitle();

  for (auto group : groups_)
    group->draw();

  app()->device()->drawTerm();
}

void
CGnuPlotWindow::
drawTitle()
{
  CGnuPlotRenderer *renderer = app()->renderer();

  renderer->setMargin(CRange2D(0,0,0,0));
  renderer->setRange (CBBox2D(0,0,1,1));
  renderer->setRegion(CBBox2D(0,0,1,1));

  if (! title().str.empty()) {
    renderer->drawHAlignedText(CPoint2D(0.5, 1), CHALIGN_TYPE_CENTER, 0,
                               CVALIGN_TYPE_TOP, 8, title().str);
  }
}
