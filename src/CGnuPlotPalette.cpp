#include <CGnuPlotPalette.h>
#include <CGnuPlotUtil.h>
#include <CUnixFile.h>
#include <CStrUtil.h>
#include <CAngle.h>

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
    case  9: return sin(CAngle::Deg2Rad(90*x));
    case 10: return cos(CAngle::Deg2Rad(90*x));
    case 11: return fabs(x - 0.5);
    case 12: return pow(2*x - 1, 2);
    case 13: return sin(CAngle::Deg2Rad(180*x));
    case 14: return fabs(cos(CAngle::Deg2Rad(180*x)));
    case 15: return sin(CAngle::Deg2Rad(360*x));
    case 16: return cos(CAngle::Deg2Rad(360*x));
    case 17: return fabs(sin(CAngle::Deg2Rad(360*x)));
    case 18: return fabs(cos(CAngle::Deg2Rad(360*x)));
    case 19: return fabs(sin(CAngle::Deg2Rad(720*x)));
    case 20: return fabs(cos(CAngle::Deg2Rad(720*x)));
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

bool
CGnuPlotPalette::
readFile(const std::string &filename)
{
  CUnixFile file(filename);

  if (! file.open()) {
    std::cerr << "can't open file \"" << filename << "\"" << std::endl;
    return false;
  }

  setColorType(CGnuPlotPalette::ColorType::DEFINED);

  resetDefinedColors();

  std::string line;

  int i = 0;

  while (file.readLine(line)) {
    if (line.empty()) continue;

    StringArray words;

    CStrUtil::addWords(line, words);

    double x = i;

    int j = 0;

    if (words.size() >= 4) {
      x = CStrUtil::toReal(words[0]);

      ++j;
    }

    double r, g, b;

    if (words.size() >= 3) {
      r = CStrUtil::toReal(words[j + 0]);
      g = CStrUtil::toReal(words[j + 1]);
      b = CStrUtil::toReal(words[j + 2]);
    }
    else
      continue;

    addDefinedColor(x, CRGBA(r, g, b));

    ++i;
  }

  return true;
}

void
CGnuPlotPalette::
unset()
{
}

void
CGnuPlotPalette::
show(std::ostream &os) const
{
  os << "palette is " << (gray_ ? "GRAY" : "COLOR") << std::endl;

  if (! gray_) {
    if      (colorType_ == ColorType::MODEL)
      os << "rgb color mapping by rgbformulae are " <<
            rModel_ << "," << gModel_ << "," << bModel_ << std::endl;
    else if (colorType_ == ColorType::DEFINED)
      os << "color mapping by defined gradient" << std::endl;
    else if (colorType_ == ColorType::FUNCTIONS)
      os << "color mapping is done by user defined functions " <<
            "A-formula: " << rf_ <<
            "B-formula: " << gf_ <<
            "C-formula: " << bf_ << std::endl;
    else if (colorType_ == ColorType::CUBEHELIX)
      os << "Cubehelix color palette: start " << cbStart_ <<
            " cycles " << cbCycles_ << " saturation " << cbSaturation_ << std::endl;
  }

  os << "figure is " << (negative_ ? "NEGATIVE" : "POSITIVE") << std::endl;

  if (psAllcF_)
    os << "all color formulae ARE written into output postscript file" << std::endl;
  else
    os << "all color formulae ARE NOT written into output postscript file" << std::endl;

  if (maxColors_ <= 0)
    os << "allocating ALL remaining" << std::endl;
  else
    os << "allocating MAX " << maxColors_;
  os << " color positions for discrete palette terminals" << std::endl;

  os << "Color-Model: ";
  if      (colorModel_ == ColorModel::RGB) os << "RGB";
  else if (colorModel_ == ColorModel::HSV) os << "HSV";
  else if (colorModel_ == ColorModel::CMY) os << "CMY";
  else if (colorModel_ == ColorModel::YIQ) os << "YIQ";
  else if (colorModel_ == ColorModel::XYZ) os << "XYZ";
  os << std::endl;

  os << "gamma is " << gamma_ << std::endl;
}

void
CGnuPlotPalette::
showGradient(std::ostream &os) const
{
  if (colorType_ != ColorType::DEFINED) {
    int i = 0;

    for (const auto &cc : colors_) {
      const CRGBA &c = cc.second;

      os << i << ". gray=" << cc.first << ", (r,g,b)=(" <<
            c.getRed() << "," << c.getGreen() << "," << c.getBlue() <<
            "), " << c.getRGB().stringEncode() << " = " <<
            c.getRedI() << " " << c.getGreenI() << " " << c.getBlueI() << std::endl;

      ++i;
    }
  }
  else
    os << "color mapping *not* done by defined gradient." << std::endl;
}

void
CGnuPlotPalette::
showRGBFormulae(std::ostream &os) const
{
  os << "set palette rgbformulae " <<
        rModel_ << "," << gModel_ << "," << bModel_ << std::endl;
}

void
CGnuPlotPalette::
showPaletteValues(std::ostream &os, int n, bool is_float, bool is_int)
{
  if (n <= 1) n = 128;

  os << "Color palette with " << n << " discrete colors." << std::endl;

  double x = 0.0;
  double d = 1.0/(n - 1);

  for (int i = 0; i < n; ++i, x += d) {
    CRGBA c = getColor(x);

    os << "  ";

    if      (is_float) {
      os << c.getRed() << " " << c.getGreen() << " " << c.getBlue() << std::endl;
    }
    else if (is_int) {
      os << c.getRedI() << " " << c.getGreenI() << " " << c.getBlueI() << std::endl;
    }
    else {
      os << i << ". gray=" << x << ", (r,g,b)=(" <<
            c.getRed() << "," << c.getGreen() << "," << c.getBlue() <<
            "), " << c.getRGB().stringEncode() << " = " <<
            c.getRedI() << " " << c.getGreenI() << " " << c.getBlueI() << std::endl;
    }
  }
}