#include <CGnuPlotAxisData.h>
#include <CGnuPlotGroup.h>
#include <CParseLine.h>

CGnuPlotAxisData::
CGnuPlotAxisData(AxisType type, int ind) :
 type_(type), ind_(ind)
{
  setDisplayed(ind_ == 1);

  setShowTics(ind_ == 1);
  setMirror  (ind_ == 1);
}

CGnuPlotTypes::AxisDirection
CGnuPlotAxisData::
direction() const
{
  if      (type_ == AxisType::X) return CGnuPlotTypes::AxisDirection::X;
  else if (type_ == AxisType::Y) return CGnuPlotTypes::AxisDirection::Y;
  else if (type_ == AxisType::Z) return CGnuPlotTypes::AxisDirection::Z;
  else if (type_ == AxisType::P) return CGnuPlotTypes::AxisDirection::Y;
  else if (type_ == AxisType::R) return CGnuPlotTypes::AxisDirection::X;
  else if (type_ == AxisType::T) return CGnuPlotTypes::AxisDirection::Y;
  else                           assert(false);
}

CRGBA
CGnuPlotAxisData::
getZeroAxisColor(CGnuPlotGroup *group) const
{
  CGnuPlot *plot = group->app();

  if (zeroAxis_.lineStyle) {
    auto lineStyle = plot->getLineStyleInd(zeroAxis_.lineStyle.value());

    if (lineStyle)
      return lineStyle->calcColor(group, CRGBA(0,0,0));
  }

  if (zeroAxis_.lineType) {
    auto lineType = plot->getLineTypeInd(zeroAxis_.lineType.value());

    if (lineType)
      return lineType->calcColor(group, CRGBA(0,0,0));
  }

  return CGnuPlotStyleInst->indexColor(0);
}

double
CGnuPlotAxisData::
getZeroAxisWidth() const
{
  if (zeroAxis_.lineWidth)
    return zeroAxis_.lineWidth.value();
  else
    return CGnuPlotStyleInst->indexWidth(0);
}

CLineDash
CGnuPlotAxisData::
getZeroAxisDash() const
{
  if (zeroAxis_.lineDash)
    return zeroAxis_.lineDash.value();
  else
    return CGnuPlotStyleInst->indexDash(0);
}

bool
CGnuPlotAxisData::
hasRTicLabels(int level) const
{
  auto p = rticLabels_.find(level);

  if (p == rticLabels_.end())
    return false;

  return ! (*p).second.empty();
}

const std::string &
CGnuPlotAxisData::
iticLabel(int i) const
{
  auto p = iticLabels_.find(i);

  return (*p).second;
}

CGnuPlotAxisData::RTicLabels
CGnuPlotAxisData::
rticLabels(int level) const
{
  auto p = rticLabels_.find(level);

  if (p == rticLabels_.end())
    return RTicLabels();

  return (*p).second;
}

void
CGnuPlotAxisData::
setTicScale(double majorScale, double minorScale)
{
  majorScale_ = majorScale;
  minorScale_ = minorScale;
}

void
CGnuPlotAxisData::
setTicLabel(double r, const std::string &s, int level)
{
  if (level == 0)
    setITicLabel(int(r + 0.5), s);

  setRTicLabel(r, s, level);
}

void
CGnuPlotAxisData::
unset()
{
  displayed_ = false;

  unsetRange();

  text_ = "";

  ticOffset_   = CPoint2D(0,0);
  labelOffset_ = CPoint3D(0,0,0);
}

void
CGnuPlotAxisData::
unsetRange()
{
  min_ = std::optional<double>();
  max_ = std::optional<double>();

  reverse_   = false;
  writeback_ = false;
}

void
CGnuPlotAxisData::
unsetZeroAxis()
{
  zeroAxis_.displayed = false;
  zeroAxis_.lineStyle = -1;
  zeroAxis_.lineType  = -1;
  zeroAxis_.lineWidth = 0;
  zeroAxis_.lineDash  = CLineDash();
}

void
CGnuPlotAxisData::
reset()
{
  unsetRange();

  borderTics_ = true;

  text_ = "";

  iticLabels_.clear();
  rticLabels_.clear();

  majorScale_ = 1.0;
  minorScale_ = 1.0;

  dummyVar_ = "";

  format_ .reset();
  timeFmt_.reset();

  geographic_ = false;
}

std::string
CGnuPlotAxisData::
getAxisValueStr(int i, double r) const
{
  if (hasITicLabels()) {
    if (hasITicLabel(i))
      return iticLabel(i);
    else
      return "";
  }
  else
    return formatAxisValue(r);
}

std::string
CGnuPlotAxisData::
formatAxisValue(double r) const
{
  if (COSNaN::is_nan(r))
    return "NaN";

  if (isTime()) {
    static char buffer[512];

    auto t = time_t(r);

    struct tm *tm1 = localtime(&t);

    (void) strftime(buffer, 512, timeFmt().value_or("%d/%m/%y,%H:%M").c_str(), tm1);

    return buffer;
  }
  else if (format()) {
    std::string fmt = format().value();

    if (fmt != "") {
      if (isGeographic()) {
        return geographicFormat(fmt, r);
      }
      else {
        if (fabs(r) < 1E-6)
          return CStrUtil::strprintf(fmt.c_str(), 0.0);
        else
          return CStrUtil::strprintf(fmt.c_str(), r);
      }
    }
    else
      return "";
  }
  else {
    if (isGeographic()) {
      return geographicFormat("%g", r);
    }
    else {
      if (fabs(r) < 1E-6)
        return CStrUtil::strprintf("%g", 0.0);
      else
        return CStrUtil::strprintf("%g", r);
    }
  }
}

std::string
CGnuPlotAxisData::
geographicFormat(const std::string &fmt, double r) const
{
  double ar =fabs(r);

  CParseLine line(fmt);

  std::string str;

  while (line.isValid()) {
    char c = line.getChar();

    std::string fmt1;

    if (c == '%') {
      int pos1 = line.pos();

      if (line.isDigit()) {
        while (line.isDigit())
          line.skipChar();

        if (line.isChar('.')) {
          line.skipChar();

          while (line.isDigit())
            line.skipChar();
        }

        fmt1 += line.substr(pos1, line.pos() - pos1);
      }

      c = line.getChar();

      if      (c == 'D') {
        str += std::to_string(int(ar));
      }
      else if (c == 'd') {
        fmt1 += "d";

        str += CStrUtil::strprintf(fmt1.c_str(), ar);
      }
      else if (c == 'M') {
        double r1 = 60*(ar - int(ar));

        str += std::to_string(int(r1));
      }
      else if (c == 'm') {
        double r1 = 60*(ar - int(ar));

        str += CStrUtil::strprintf(fmt1.c_str(), r1);
      }
      else if (c == 'S') {
        double r1 = 60*ar;
        double r2 = 60*(r1 - int(r1));

        str += CStrUtil::strprintf(fmt1.c_str(), r2);
      }
      else if (c == 's') {
        double r1 = 60*ar;
        double r2 = 60*(r1 - int(r1));

        str += std::to_string(r2);
      }
      else if (c == 'E') {
        if (ar > 1E-6)
          str += (r < 0 ? "W" : "E");
      }
      else if (c == 'N') {
        if (ar > 1E-6)
          str += (r < 0 ? "S" : "N");
      }
      else {
        str += '%';
        str += c;
      }
    }
    else
      str += c;
  }

  return str;
}

bool
CGnuPlotAxisData::
inside(double x) const
{
  if      (min_ && max_)
    return (x >= min_.value() && x <= max_.value());
  else if (min_)
    return (x >= min_.value());
  else if (max_)
    return (x <= max_.value());
  else
    return true;
}

bool
CGnuPlotAxisData::
mappedInside(double x) const
{
  if      (min_ && max_) {
    double x1 = mapLogValue(min_.value());
    double x2 = mapLogValue(max_.value());

    return (x >= x1 && x <= x2);
  }
  else if (min_) {
    double x1 = mapLogValue(min_.value());

    return (x >= x1);
  }
  else if (max_) {
    double x2 = mapLogValue(max_.value());

    return (x <= x2);
  }
  else
    return true;
}

void
CGnuPlotAxisData::
show(std::ostream &os, const std::string &prefix, int n) const
{
  os << "set " << prefix << "axis " << n;
  os << " range [" << min_.value_or(0) << " : " << max_.value_or(10) << " ] ";
  os << " " << (reverse_   ? "reverse"   : "noreverse" );
  os << " " << (writeback_ ? "writeback" : "nowriteback");

  // paxis 1 -axis tics are IN, major ticscale is 1 and minor ticscale is 0.5
  // paxis 1 -axis tics: on axis tics are limited to data range

  os << " ";
  printLabel(os, prefix);

  // intervals computed automatically
}

void
CGnuPlotAxisData::
showRange(std::ostream &os, const std::string &prefix) const
{
  os << "set " << prefix;
  os << " [ "; CUtil::print(os, min_, "*"); os << " : "; CUtil::print(os, max_, "*"); os << " ]";

  os << " " << (reverse_   ? "reverse"   : "noreverse" );
  os << " " << (writeback_ ? "writeback" : "nowriteback");

  if (! min_ || ! max_) {
    os << " # (currently [";
    if (! min_) os << min_.value_or(-10);
    os << ":";
    if (! max_) os << max_.value_or( 10);
    os << "] )";
  }

  os << "\n";
}

void
CGnuPlotAxisData::
showTics(std::ostream &os, const std::string &prefix) const
{
  os << prefix << " tics are in, major ticscale is 1 and minor ticscale is 0.5\n";

  if (isShowTics()) {
    os << prefix << " tics: on axis\n";
    os << "  labels are justified automatically, format \"" <<
          format().value_or("%g") << "\" and are not rotated,\n";
    os << "  offset (character " << labelOffset_.x << ", " <<
          labelOffset_.y << ", " << labelOffset_.z << ")\n";
    os << "  intervals computed automatically\n";
  }
  else
    os << prefix << " tics: off\n";
}

void
CGnuPlotAxisData::
showFormat(std::ostream &os, const std::string &prefix) const
{
  os << "set format " << prefix << " \"" << format().value_or("%g") << "\"\n";
}

void
CGnuPlotAxisData::
showMinorTics(std::ostream &os, const std::string &str, const std::string &str1) const
{
  if (isMinorTicsDisplayed()) {
    if (getMinorTicsFreq())
      os << "minor " << str << " are drawn with " << getMinorTicsFreq().value() <<
            " subintervals between major " << str1 << " marks\n";
    else
      os << "minor " << str << " are computed automatically\n";
  }
  else
    os << "minor " << str << " are off\n";
}

void
CGnuPlotAxisData::
showZeroAxis(std::ostream &os, const std::string &str)
{
  os << str << "zeroaxis is ";

  if (zeroAxis_.displayed) {
    os << "drawn with";
    os << " lt " << zeroAxis_.lineType.value();
    os << " linewidth " << zeroAxis_.lineWidth.value();
  }
  else
    os << "OFF";

  os << "\n";
}

void
CGnuPlotAxisData::
printLabel(std::ostream &os, const std::string &prefix) const
{
  os << prefix << "label is \"" << text_ << "\", " <<
        "offset at ((character units) " << labelOffset_.x << ", " <<
        labelOffset_.y << ", " << labelOffset_.z << ")\n";
}
