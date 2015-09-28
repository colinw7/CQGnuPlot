#include <CGnuPlotLogDevice.h>
#include <CGnuPlotLogRenderer.h>
#include <CGnuPlotWindow.h>
#include <CGnuPlotGroup.h>
#include <CGnuPlotPlot.h>

CGnuPlotLogDevice::
CGnuPlotLogDevice() :
 CGnuPlotDevice("Log"), os_(&std::cout)
{
  renderer_ = new CGnuPlotLogRenderer(this);
}

CGnuPlotLogDevice::
~CGnuPlotLogDevice()
{
}

void
CGnuPlotLogDevice::
stateChanged(CGnuPlotWindow *window, CGnuPlotTypes::ChangeState state)
{
  if (state == CGnuPlotTypes::ChangeState::PLOT_ADDED)
    window->draw();
}

void
CGnuPlotLogDevice::
timeout()
{
}

void
CGnuPlotLogDevice::
drawInit(CGnuPlotWindow *)
{
  const std::string &name = app()->getOutputFile();

  if (name != "") {
    file_.open(name.c_str(), std::ios_base::out);

    os_ = &file_;
  }
  else
    os_ = &std::cout;

  renderer_->setWidth (size_.width );
  renderer_->setHeight(size_.height);
}

void
CGnuPlotLogDevice::
drawTerm()
{
  file_.close();
}

CGnuPlotRenderer *
CGnuPlotLogDevice::
renderer()
{
  return renderer_;
}

bool
CGnuPlotLogDevice::
parseArgs(CParseLine &)
{
  return false;
}

void
CGnuPlotLogDevice::
show(std::ostream &) const
{
}
