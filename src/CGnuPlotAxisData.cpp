#include <CGnuPlotAxisData.h>
#include <CGnuPlotGroup.h>

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

  if (zeroAxis_.lineStyle.isValid()) {
    CGnuPlotLineStyleP lineStyle = plot->getLineStyleInd(zeroAxis_.lineStyle.getValue());

    if (lineStyle.isValid())
      return lineStyle->calcColor(group, CRGBA(0,0,0));
  }

  if (zeroAxis_.lineType.isValid()) {
    CGnuPlotLineTypeP lineType = plot->getLineTypeInd(zeroAxis_.lineType.getValue());

    if (lineType.isValid())
      return lineType->calcColor(group, CRGBA(0,0,0));
  }

  return CGnuPlotStyleInst->indexColor(0);
}

double
CGnuPlotAxisData::
getZeroAxisWidth() const
{
  if (zeroAxis_.lineWidth.isValid())
    return zeroAxis_.lineWidth.getValue();
  else
    return CGnuPlotStyleInst->indexWidth(0);
}

CLineDash
CGnuPlotAxisData::
getZeroAxisDash() const
{
  if (zeroAxis_.lineDash.isValid())
    return zeroAxis_.lineDash.getValue();
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
  labelOffset_ = CPoint2D(0,0);
}

void
CGnuPlotAxisData::
unsetRange()
{
  min_ = COptReal();
  max_ = COptReal();

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

  majorScale_ = 1.0;
  minorScale_ = 1.0;

  dummyVar_ = "";
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
  if (isTime()) {
    static char buffer[512];

    time_t t(r);

    struct tm *tm1 = localtime(&t);

    (void) strftime(buffer, 512, format().c_str(), tm1);

    return buffer;
  }
  else if (format() != "")
    return CStrUtil::strprintf(format().c_str(), r);
  else
    return "";
}

bool
CGnuPlotAxisData::
inside(double x) const
{
  if      (min_.isValid() && max_.isValid())
    return (x >= min_.getValue() && x <= max_.getValue());
  else if (min_.isValid())
    return (x >= min_.getValue());
  else if (max_.isValid())
    return (x <= max_.getValue());
  else
    return true;
}

bool
CGnuPlotAxisData::
mappedInside(double x) const
{
  if      (min_.isValid() && max_.isValid()) {
    double x1 = mapLogValue(min_.getValue());
    double x2 = mapLogValue(max_.getValue());

    return (x >= x1 && x <= x2);
  }
  else if (min_.isValid()) {
    double x1 = mapLogValue(min_.getValue());

    return (x >= x1);
  }
  else if (max_.isValid()) {
    double x2 = mapLogValue(max_.getValue());

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
  os << " range [" << min_.getValue(0) << " : " << max_.getValue(10) << " ] ";
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
  os << " [ "; min_.print(os, "*"); os << " : "; max_.print(os, "*"); os << " ]";

  os << " " << (reverse_   ? "reverse"   : "noreverse" );
  os << " " << (writeback_ ? "writeback" : "nowriteback");

  if (! min_.isValid() || ! max_.isValid()) {
    os << " # (currently [";
    if (! min_.isValid()) os << min_.getValue(-10);
    os << ":";
    if (! max_.isValid()) os << max_.getValue( 10);
    os << "] )";
  }

  os << std::endl;
}

void
CGnuPlotAxisData::
showTics(std::ostream &os, const std::string &prefix) const
{
  os << prefix << " tics are in, major ticscale is 1 and minor ticscale is 0.5" << std::endl;

  if (showTics()) {
    os << prefix << " tics: on axis" << std::endl;
    os << "  labels are justified automatically, format \"" <<
          format_ << "\" and are not rotated," << std::endl;
    os << "  offset (character " <<
             labelOffset_.x << ", " << labelOffset_.y << ", 0)" << std::endl;
    os << "  intervals computed automatically" << std::endl;
  }
  else
    os << prefix << " tics: off" << std::endl;
}

void
CGnuPlotAxisData::
showFormat(std::ostream &os, const std::string &prefix) const
{
  os << "set format " << prefix << " \"" << format_ << "\"" << std::endl;
}

void
CGnuPlotAxisData::
showMinorTics(std::ostream &os, const std::string &str, const std::string &str1) const
{
  if (isMinorTicsDisplayed()) {
    if (getMinorTicsFreq().isValid())
      os << "minor " << str << " are drawn with " << getMinorTicsFreq().getValue() <<
            " subintervals between major " << str1 << " marks" << std::endl;
    else
      os << "minor " << str << " are computed automatically" << std::endl;
  }
  else
    os << "minor " << str << " are off" << std::endl;
}

void
CGnuPlotAxisData::
showZeroAxis(std::ostream &os, const std::string &str)
{
  os << str << "zeroaxis is ";

  if (zeroAxis_.displayed) {
    os << "drawn with";
    os << " lt " << zeroAxis_.lineType;
    os << " linewidth " << zeroAxis_.lineWidth;
  }
  else
    os << "OFF";

  os << std::endl;
}

void
CGnuPlotAxisData::
printLabel(std::ostream &os, const std::string &prefix) const
{
  os << prefix << "label is \"" << text_ << "\", " <<
        "offset at ((character units) " <<
        labelOffset_.x << ", " << labelOffset_.y << ", 0)" << std::endl;
}
