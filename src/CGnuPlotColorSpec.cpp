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
  // bgnd: color -3
  // black: color -1
  if      (isBackground())
    os << "bgnd"; // -3
  else if (isRGB())
    os << "rgb \"" << c_.getRGB().stringEncode() << "\"";
  else if (isVariable())
    os << "variable";
  else if (isRGBVariable())
    os << "rgb variable";
  else if (isPaletteFrac())
    os << "palette frac";
  else if (isPaletteCB())
    os << "palette cb";
  else if (isPaletteZ())
    os << "palette z";
  else if (isPaletteVariable())
    os << "palette var";
  else if (isIndex())
    os << i_;
  else
    os << "rgb \"" << c_.getRGB().stringEncode() << "\"";
}

CRGBA
CGnuPlotColorSpec::
calcColor(CGnuPlotGroup *group, double x) const
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
    double x1;

    if (! group->is3D()) {
      const CBBox2D &bbox = group->bbox2D();

      x1 = CGnuPlotUtil::map(x, bbox.getXMin(), bbox.getXMax(), 0, 1);
    }
    else {
      CGnuPlotAxisData &zaxis = group->zaxis(1);

      x1 = CGnuPlotUtil::map(x, zaxis.min().getValue(), zaxis.max().getValue(), 0, 1);
    }

    CColor c = group->palette()->getColor(x1);

    return c.rgba();
  }
  else if (isPaletteVariable()) {
    const CGnuPlotColorBoxP &cb = group->colorBox();

    return cb->valueToColor(x).rgba();
  }
  else if (isRGB()) {
    return c_;
  }
  else
    return CRGBA(0,0,0);
}
