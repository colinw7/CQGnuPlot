#include <CGnuPlotWindow.h>
#include <CGnuPlotPlot.h>
#include <CGnuPlotRenderer.h>

CGnuPlotWindow::
CGnuPlotWindow(CGnuPlot *plot) :
 is3D_(false), plot_(plot), renderer_(0), hidden3D_(false), surface3D_(true),
 contour3D_(false), pm3D_(false)
{
}

CGnuPlotWindow::
~CGnuPlotWindow()
{
}

void
CGnuPlotWindow::
set3D(bool b)
{
  is3D_ = b;
}

void
CGnuPlotWindow::
setRenderer(CGnuPlotRenderer *renderer)
{
  renderer_ = renderer;
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
  for (auto plot : plots_)
    plot->reset3D();
}

void
CGnuPlotWindow::
paintStart()
{
  renderer_->setMargin(CRange2D(0,0,0,0));
  renderer_->setRange (CBBox2D(0,0,1,1));
  renderer_->setRegion(CBBox2D(0,0,1,1));

  if (! title().str.empty()) {
    renderer_->drawHAlignedText(CPoint2D(0.5, 1), CHALIGN_TYPE_CENTER, 0,
                                CVALIGN_TYPE_TOP, 8, title().str);
  }
}

void
CGnuPlotWindow::
paintEnd()
{
}

void
CGnuPlotWindow::
addPlot(CGnuPlotPlot *plot)
{
  if (plot_->isDebug())
    std::cerr << "Add Plot" << std::endl;

  plot->setInd(plots_.size() + 1);

  plots_.push_back(plot);
}

void
CGnuPlotWindow::
draw()
{
  paintStart();

  for (auto plot : plots_)
    plot->draw();

  paintEnd();
}
