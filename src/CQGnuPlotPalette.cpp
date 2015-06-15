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

CQGnuPlotEnum::PaletteColorType
CQGnuPlotPalette::
colorType() const
{
  return CQGnuPlotUtil::paletteColorTypeConv(CGnuPlotPalette::colorType());
}

void
CQGnuPlotPalette::
setColorType(CQGnuPlotEnum::PaletteColorType c)
{
  CGnuPlotPalette::setColorType(CQGnuPlotUtil::paletteColorTypeConv(c));
}

CQGnuPlotEnum::ColorModelType
CQGnuPlotPalette::
colorModel() const
{
  return CQGnuPlotUtil::colorModelTypeConv(CGnuPlotPalette::colorModel());
}

void
CQGnuPlotPalette::
setColorModel(CQGnuPlotEnum::ColorModelType c)
{
  CGnuPlotPalette::setColorModel(CQGnuPlotUtil::colorModelTypeConv(c));
}
