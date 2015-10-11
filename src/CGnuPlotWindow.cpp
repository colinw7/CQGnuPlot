#include <CGnuPlotWindow.h>
#include <CGnuPlotGroup.h>
#include <CGnuPlotPlot.h>
#include <CGnuPlotDevice.h>
#include <CGnuPlotRenderer.h>

CGnuPlotWindow::
CGnuPlotWindow(CGnuPlot *plot) :
 plot_(0), size_(640, 480)
{
  setApp(plot);
}

CGnuPlotWindow::
~CGnuPlotWindow()
{
}

void
CGnuPlotWindow::
setApp(CGnuPlot *plot)
{
  plot_ = plot;

  if (plot_ && plot_->device())
    size_ = plot_->device()->size();
}

void
CGnuPlotWindow::
clear()
{
  is3D_ = false;

  groups_.clear();
}

void
CGnuPlotWindow::
set3D(bool b)
{
  is3D_ = b;
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

CRGBA
CGnuPlotWindow::
getLightRGBA(const CRGBA &c) const
{
  double f = 0.7;

  return f*c + (1 - f)*backgroundColor();
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
