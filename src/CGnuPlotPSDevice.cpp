#include <CGnuPlotPSDevice.h>
#include <CGnuPlotPSRenderer.h>
#include <CGnuPlotWindow.h>
#include <CGnuPlotGroup.h>
#include <CGnuPlotPlot.h>

CGnuPlotPSDevice::
CGnuPlotPSDevice() :
 CGnuPlotDevice("PS")
{
  renderer_ = new CGnuPlotPSRenderer(this);
}

CGnuPlotPSDevice::
~CGnuPlotPSDevice()
{
}

void
CGnuPlotPSDevice::
stateChanged(CGnuPlotWindow *window, CGnuPlotTypes::ChangeState state)
{
  if (state == CGnuPlotTypes::ChangeState::PLOT_ADDED)
    window->draw();
}

void
CGnuPlotPSDevice::
timeout()
{
}

void
CGnuPlotPSDevice::
drawInit(CGnuPlotWindow *)
{
  const std::string &name = app()->getOutputFile();

  if (name != "")
    print_.setFilename(name);
  else
    print_.setFile(stdout);

  print_.setPageSize(size_.width, size_.height);

  print_.setSize(0, 0, size_.width, size_.height);

  print_.init();

  renderer_->setWidth (size_.width );
  renderer_->setHeight(size_.height);
}

void
CGnuPlotPSDevice::
drawTerm()
{
  print_.term();
}

CGnuPlotRenderer *
CGnuPlotPSDevice::
renderer()
{
  return renderer_;
}

bool
CGnuPlotPSDevice::
parseArgs(CParseLine &)
{
  return false;
}

void
CGnuPlotPSDevice::
show(std::ostream &os) const
{
  os << "size " << size_.width << "," << size_.height;
  os << " " << (enhanced_ ? "enhanced" : "noenhanced");
}
