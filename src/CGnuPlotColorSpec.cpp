#include <CGnuPlotColorSpec.h>
#include <CGnuPlotGroup.h>
#include <CGnuPlotPlot.h>
#include <CGnuPlotUtil.h>

void
CGnuPlotColorSpec::
reset()
{
  type_ = Type::NONE;
  c_    = CRGBA(0,0,0);
  i_    = 0;
  r_    = 0.0;
}

void
CGnuPlotColorSpec::
print(std::ostream &os) const
{
  if (isBackground())
    os << "bgnd";
  else
    os << "rgb \"" << c_.getRGB().stringEncode() << "\"";
}

CRGBA
CGnuPlotColorSpec::
calcColor(CGnuPlotPlot *plot, double x) const
{
  if      (isVariable())
    return CGnuPlotStyleInst->indexColor(int(x));
  else if (isRGBVariable()) {
    int ix = int(x);

    int a = (ix >> 24) & 0xFF; a = 255 - a;
    int r = (ix >> 16) & 0xFF;
    int g = (ix >>  8) & 0xFF;
    int b = (ix >>  0) & 0xFF;

    a = CGnuPlotUtil::clamp(a, 0, 255);
    r = CGnuPlotUtil::clamp(r, 0, 255);
    g = CGnuPlotUtil::clamp(g, 0, 255);
    b = CGnuPlotUtil::clamp(b, 0, 255);

    return CRGBA(r/255.0, g/255.0, b/255.0, a/255.0);
  }
  else if (isPaletteZ()) {
    CGnuPlotGroup *group = plot->group();

    double x1 = CGnuPlotUtil::map(x, group->getBBox().getXMin(), group->getBBox().getXMax(), 0, 1);

    CColor c = group->palette()->getColor(x1);

    return c.rgba();
  }
  else if (isPaletteVariable()) {
#if 0
    CGnuPlotGroup *group = plot->group();

    double x1 = CGnuPlotUtil::map(x, group->getBBox().getXMin(), group->getBBox().getXMax(), 0, 1);

    return group->palette()->getColor(x1);
#else
    return CGnuPlotStyleInst->indexColor(int(x) + 1);
#endif
  }
  else
    return CRGBA(0,0,0);
}
