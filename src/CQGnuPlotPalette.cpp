#include <CQGnuPlotPalette.h>
#include <CQGnuPlotGroup.h>
#include <CQGnuPlotEnum.h>

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
  return CQGnuPlotEnum::paletteColorTypeConv(CGnuPlotPalette::colorType());
}

void
CQGnuPlotPalette::
setColorType(CQGnuPlotEnum::PaletteColorType c)
{
  CGnuPlotPalette::setColorType(CQGnuPlotEnum::paletteColorTypeConv(c));
}

CQGnuPlotEnum::ColorModelType
CQGnuPlotPalette::
colorModel() const
{
  return CQGnuPlotEnum::colorModelTypeConv(CGnuPlotPalette::colorModel());
}

void
CQGnuPlotPalette::
setColorModel(CQGnuPlotEnum::ColorModelType c)
{
  CGnuPlotPalette::setColorModel(CQGnuPlotEnum::colorModelTypeConv(c));
}
