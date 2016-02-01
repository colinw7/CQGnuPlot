#include <CQGnuPlotPNGRenderer.h>
#include <CQGnuPlotPNGDevice.h>

CQGnuPlotPNGRenderer::
CQGnuPlotPNGRenderer(CQGnuPlotPNGDevice *device) :
 CQGnuPlotRenderer(), device_(device)
{
}

CQGnuPlotPNGRenderer::
~CQGnuPlotPNGRenderer()
{
}

void
CQGnuPlotPNGRenderer::
setOutputSize(const QSize &size)
{
  device_->setOutputSize(size);
}
