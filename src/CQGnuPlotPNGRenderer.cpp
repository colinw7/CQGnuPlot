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
