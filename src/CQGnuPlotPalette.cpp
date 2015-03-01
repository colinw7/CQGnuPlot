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
