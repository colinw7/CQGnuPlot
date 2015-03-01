#include <CGnuPlotPalette.h>
#include <CGnuPlotUtil.h>

CRGBA
CGnuPlotPalette::
getColor(double x) const
{
  if (colorType_ == ColorType::DEFINED) {
    if (colors_.empty()) {
      CGnuPlotPalette *th = const_cast<CGnuPlotPalette *>(this);

      th->addDefinedColor(0.0, CRGBA(0,0,0));
      th->addDefinedColor(1.0, CRGBA(1,1,1));
    }

    double min = colors_. begin()->first;
    double max = colors_.rbegin()->first;

    auto p = colors_.begin();

    double x1    = ((*p).first - min)/(max - min);
    CRGBA  rgba1 = (*p).second;

    if (x <= x1) return rgba1;

    for (++p; p != colors_.end(); ++p) {
      double x2    = ((*p).first - min)/(max - min);
      CRGBA  rgba2 = (*p).second;

      if (x <= x2) {
        double m = (x - x1)/(x2 - x1);

        return (1.0 - m)*rgba1 + m*rgba2;
      }

      x1    = x2;
      rgba1 = rgba2;
    }

    return rgba1;
  }
  else {
    double r = CGnuPlotUtil::clamp(interp(rModel_, x), 0.0, 1.0);
    double g = CGnuPlotUtil::clamp(interp(gModel_, x), 0.0, 1.0);
    double b = CGnuPlotUtil::clamp(interp(bModel_, x), 0.0, 1.0);

    return CRGBA(r, g, b);
  }
}

double
CGnuPlotPalette::
interp(int ind, double x) const
{
  switch (ind) {
    case  0: return 0;
    case  1: return 0.5;
    case  2: return 1;
    case  3: return x;
    case  4: return pow(x, 2);
    case  5: return pow(x, 3);
    case  6: return pow(x, 4);
    case  7: return sqrt(x);
    case  8: return sqrt(sqrt(x));
    case  9: return sin(Deg2Rad(90*x));
    case 10: return cos(Deg2Rad(90*x));
    case 11: return fabs(x - 0.5);
    case 12: return pow(2*x - 1, 2);
    case 13: return sin(Deg2Rad(180*x));
    case 14: return fabs(cos(Deg2Rad(180*x)));
    case 15: return sin(Deg2Rad(360*x));
    case 16: return cos(Deg2Rad(360*x));
    case 17: return fabs(sin(Deg2Rad(360*x)));
    case 18: return fabs(cos(Deg2Rad(360*x)));
    case 19: return fabs(sin(Deg2Rad(720*x)));
    case 20: return fabs(cos(Deg2Rad(720*x)));
    case 21: return 3*x;
    case 22: return 3*x - 1;
    case 23: return 3*x - 2;
    case 24: return fabs(3*x - 1);
    case 25: return fabs(3*x - 2);
    case 26: return (3*x - 1)/2;
    case 27: return (3*x - 2)/2;
    case 28: return fabs((3*x - 1)/2);
    case 29: return fabs((3*x - 2)/2);
    case 30: return x/0.32 - 0.78125;
    case 31: return 2*x - 0.84;
    case 32: {
      if      (x < 0.00) return 0;
      else if (x < 0.25) return 4*x;
      else if (x < 0.42) return 1;
      else if (x < 0.92) return -2*x + 1.84;
      else if (x < 1.00) return x/0.08 - 11.5;
      else               return 1;
    }
    case 33: return fabs(2*x - 0.5);
    case 34: return 2*x;
    case 35: return 2*x - 0.5;
    case 36: return 2*x - 1;
    default: return x;
  }
}
