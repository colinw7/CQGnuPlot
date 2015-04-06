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

  camera_ = CGnuPlotCamera();

  hidden3D_  = false;
  surface3D_ = false;
  contour3D_ = false;

  pm3D_.unset();
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
  camera_.setEnabled(b);

  reset3D();
}

void
CGnuPlotWindow::
setCameraRotateX(double a)
{
  camera_.setRotateX(a);

  reset3D();
}

void
CGnuPlotWindow::
setCameraRotateY(double a)
{
  camera_.setRotateY(a);

  reset3D();
}

void
CGnuPlotWindow::
setCameraRotateZ(double a)
{
  camera_.setRotateZ(a);

  reset3D();
}

void
CGnuPlotWindow::
setCameraXMin(double x)
{
  camera_.setXMin(x);

  reset3D();
}

void
CGnuPlotWindow::
setCameraXMax(double x)
{
  camera_.setXMax(x);

  reset3D();
}

void
CGnuPlotWindow::
setCameraYMin(double y)
{
  camera_.setYMin(y);

  reset3D();
}

void
CGnuPlotWindow::
setCameraYMax(double y)
{
  camera_.setYMax(y);

  reset3D();
}

void
CGnuPlotWindow::
setCameraNear(double z)
{
  camera_.setNear(z);

  reset3D();
}

void
CGnuPlotWindow::
setCameraFar(double z)
{
  camera_.setFar(z);

  reset3D();
}

void
CGnuPlotWindow::
reset3D()
{
  for (auto &group : groups_)
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
fitGroups()
{
  double y  = 0.0;
  double dy = 1.0/groups_.size();

  for (auto &group : groups_) {
    group->setRegion(CBBox2D(0.0, y, 1.0, y + dy));

    y += dy;
  }
}

void
CGnuPlotWindow::
draw()
{
  CGnuPlotRenderer *renderer = app()->renderer();

  renderer->setWindow(this);

  app()->device()->drawInit(this);

  renderer->clear(backgroundColor());

  for (auto group : groups_)
    group->draw();

  app()->device()->drawTerm();
}
