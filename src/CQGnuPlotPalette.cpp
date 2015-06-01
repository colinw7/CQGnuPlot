#include <CQGnuPlotPalette.h>
#include <CQGnuPlotGroup.h>
#include <CQGnuPlotUtil.h>

CQGnuPlotPalette::
CQGnuPlotPalette(CQGnuPlotGroup *group) :
 CGnuPlotPalette(group)
{
}

CQGnuPlotPalette::
~CQGnuPlotPalette()
{
}

CQGnuPlot::PaletteColorType
CQGnuPlotPalette::
colorType() const
{
  return CQGnuPlotUtil::paletteColorTypeConv(CGnuPlotPalette::colorType());
}

void
CQGnuPlotPalette::
setColorType(CQGnuPlot::PaletteColorType c)
{
  CGnuPlotPalette::setColorType(CQGnuPlotUtil::paletteColorTypeConv(c));
}

CQGnuPlot::ColorModelType
CQGnuPlotPalette::
colorModel() const
{
  return CQGnuPlotUtil::colorModelTypeConv(CGnuPlotPalette::colorModel());
}

void
CQGnuPlotPalette::
setColorModel(CQGnuPlot::ColorModelType c)
{
  CGnuPlotPalette::setColorModel(CQGnuPlotUtil::colorModelTypeConv(c));
}
