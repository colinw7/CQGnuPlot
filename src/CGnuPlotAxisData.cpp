#include <CGnuPlotAxisData.h>
#include <CGnuPlotGroup.h>

CGnuPlotAxisData::
CGnuPlotAxisData(int ind) :
 ind_(ind)
{
  displayed_ = (ind_ == 1);
  gridTics_  = (ind_ == 1);
  minorTics_ = (ind_ == 1);
  showTics_  = (ind_ == 1);
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

  offset_ = CPoint2D(0,0);
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
    os << "  offset (character " << offset_.x << ", " << offset_.y << ", 0)" << std::endl;
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
        "offset at ((character units) " << offset_.x << ", " << offset_.y << ", 0)" << std::endl;
}
