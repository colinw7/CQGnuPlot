#include <CGnuPlotSVGDevice.h>
#include <CGnuPlotSVGRenderer.h>
#include <CGnuPlotWindow.h>
#include <CGnuPlotGroup.h>
#include <CGnuPlotPlot.h>

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
drawInit(CGnuPlotWindow *)
{
  const std::string &name = app()->getOutputFile();

  if (name != "") {
    file_.open(name.c_str(), std::ios_base::out);

    os_ = &file_;
  }
  else
    os_ = &std::cout;

  os() << "<svg width=\"" << size_.width << "\" height=\"" << size_.height << "\"" <<
          " viewBox=\"0 0 " << size_.width << " " << size_.height << "\">\n";

  renderer_->setWidth (size_.width );
  renderer_->setHeight(size_.height);
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

bool
CGnuPlotSVGDevice::
parseArgs(CParseLine &)
{
  return false;
}

void
CGnuPlotSVGDevice::
show(std::ostream &os) const
{
  os << "size " << size_.width << "," << size_.height;
  os << " " << (enhanced_ ? "enhanced" : "noenhanced");
}
