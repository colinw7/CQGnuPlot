#include <CQGnuPlotPNGDevice.h>
#include <CQGnuPlotPNGRenderer.h>
#include <CGnuPlotWindow.h>
#include <CGnuPlotGroup.h>
#include <CGnuPlotPlot.h>
#include <QPainter>
#include <QImage>

CQGnuPlotPNGDevice::
CQGnuPlotPNGDevice() :
 CGnuPlotDevice("PNG"), painter_(0), image_(0), count_(1)
{
  renderer_ = new CQGnuPlotPNGRenderer(this);

  setSize(CISize2D(800, 800));
}

CQGnuPlotPNGDevice::
~CQGnuPlotPNGDevice()
{
}

void
CQGnuPlotPNGDevice::
stateChanged(CGnuPlotWindow *window, CGnuPlotTypes::ChangeState state)
{
  if (state == CGnuPlotTypes::ChangeState::PLOT_ADDED)
    window->draw();
}

void
CQGnuPlotPNGDevice::
timeout()
{
}

void
CQGnuPlotPNGDevice::
drawInit(CGnuPlotWindow *)
{
  renderer_->setWidth (size_.width );
  renderer_->setHeight(size_.height);

  delete painter_;
  delete image_;

  image_   = new QImage(QSize(size_.width, size_.height), QImage::Format_ARGB32);
  painter_ = new QPainter(image_);

  image_->fill(QColor(0,0,0,0));

  painter_->setRenderHint(QPainter::Antialiasing);

  renderer_->setPainter(painter_);
}

void
CQGnuPlotPNGDevice::
drawTerm()
{
  const std::string &name = app()->getOutputFile();
  if (name == "") return;

  auto p = name.find('#');

  std::string name1;

  if (p != std::string::npos)
    name1 = name.substr(0, p) + std::to_string(count_) + name.substr(p + 1);
  else
    name1 = name;

  image_->save(name1.c_str(), "png");

  ++count_;
}

CGnuPlotRenderer *
CQGnuPlotPNGDevice::
renderer()
{
  return renderer_;
}

bool
CQGnuPlotPNGDevice::
parseArgs(CParseLine &)
{
  return false;
}

void
CQGnuPlotPNGDevice::
show(std::ostream &os) const
{
  os << "size " << size_.width << "," << size_.height;
  os << " fixed";
  os << " " << (enhanced_ ? "enhanced" : "noenhanced");
  os << " " << font_;
}
