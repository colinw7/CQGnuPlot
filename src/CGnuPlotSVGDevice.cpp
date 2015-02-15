#include <CGnuPlotSVGDevice.h>
#include <CGnuPlotSVGRenderer.h>
#include <CGnuPlotWindow.h>
#include <CGnuPlotGroup.h>
#include <CGnuPlotPlot.h>
#include <CGnuPlotPie.h>

CGnuPlotSVGDevice::
CGnuPlotSVGDevice() :
 CGnuPlotDevice("SVG"), os_(&std::cout)
{
  renderer_ = new CGnuPlotSVGRenderer(this);
}

CGnuPlotSVGDevice::
~CGnuPlotSVGDevice()
{
}

void
CGnuPlotSVGDevice::
stateChanged(CGnuPlotWindow *window, CGnuPlotTypes::ChangeState state)
{
  if (state == CGnuPlotTypes::ChangeState::PLOT_ADDED)
    window->draw();
}

void
CGnuPlotSVGDevice::
timeout()
{
}

void
CGnuPlotSVGDevice::
drawInit(CGnuPlotWindow *window)
{
  const std::string &name = app()->getOutputFile();

  if (name != "") {
    file_.open(name.c_str(), std::ios_base::out);

    os_ = &file_;
  }
  else
    os_ = &std::cout;

  const CISize2D &s = window->size();

  os() << "<svg width=\"" << s.width << "\" height=\"" << s.height << "\"" <<
          " viewBox=\"0 0 " << s.width << " " << s.height << "\">\n";
}

void
CGnuPlotSVGDevice::
drawTerm()
{
  os() << "</svg>\n";

  file_.close();
}

CGnuPlotRenderer *
CGnuPlotSVGDevice::
renderer()
{
  return renderer_;
}
