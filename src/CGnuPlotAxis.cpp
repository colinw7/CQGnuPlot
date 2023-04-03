#include <CGnuPlotAxis.h>
#include <CGnuPlotWindow.h>
#include <CGnuPlotGroup.h>
#include <CGnuPlotPlot.h>
#include <CGnuPlotRenderer.h>
#include <CGnuPlotUtil.h>

#include <CMathGen.h>
#include <CStrUtil.h>

#include <cstring>
#include <algorithm>

#define MIN_GOOD_TICKS 4
#define MAX_GOOD_TICKS 12
#define OPTIMUM_TICKS  10

#define MAX_GAP_TESTS 14

struct CGnuPlotAxisIncrementTest {
  double factor;
  int    numTicks;
  double incFactor;
};

static CGnuPlotAxisIncrementTest
axesIncrementTests[MAX_GAP_TESTS] = {
  {  1.0, 5, 0 },
  {  1.2, 3, 0 },
  {  2.0, 4, 0 },
  {  2.5, 5, 0 },
  {  4.0, 4, 0 },
  {  5.0, 5, 0 },
  {  6.0, 3, 0 },
  {  8.0, 4, 0 },
  { 10.0, 5, 0 },
  { 12.0, 3, 0 },
  { 20.0, 4, 0 },
  { 25.0, 5, 0 },
  { 40.0, 4, 0 },
  { 50.0, 5, 0 }
};

CGnuPlotAxis::
CGnuPlotAxis(CGnuPlotGroup *group, const CGnuPlotAxisData &data, double start, double end) :
 group_ (group),
 data_  (data),
 start_ (start),
 end_   (end),
 start1_(start),
 end1_  (end)
{
  direction_ = data_.direction();

  // set direction vector
  if      (direction_ == AxisDirection::X) { v_ = CPoint3D(1, 0, 0); iv_ = CPoint3D(0, 1, 0); }
  else if (direction_ == AxisDirection::Y) { v_ = CPoint3D(0, 1, 0); iv_ = CPoint3D(1, 0, 0); }
  else if (direction_ == AxisDirection::Z) { v_ = CPoint3D(0, 0, 1); iv_ = CPoint3D(1, 0, 0); }
  else                                     assert(false);

  calc();
}

CGnuPlotAxis::
~CGnuPlotAxis()
{
}

CGnuPlot *
CGnuPlotAxis::
app() const
{
  return group()->app();
}

std::string
CGnuPlotAxis::
id() const
{
  std::string id;

  switch (type()) {
    case AxisType::X : id = "x" ; break;
    case AxisType::Y : id = "y" ; break;
    case AxisType::Z : id = "z" ; break;
    case AxisType::R : id = "r" ; break;
    case AxisType::P : id = "p" ; break;
    case AxisType::T : id = "t" ; break;
    case AxisType::U : id = "u" ; break;
    case AxisType::V : id = "v" ; break;
    case AxisType::CB: id = "cb"; break;
  }

  return id + std::to_string(ind());
}

void
CGnuPlotAxis::
setRange(double start, double end)
{
  start_ = mapLogValue(start);
  end_   = mapLogValue(end);

  calc();
}

void
CGnuPlotAxis::
setDataRange(double start, double end)
{
  dataStart_ = start;
  dataEnd_   = end;
}

void
CGnuPlotAxis::
setMajorIncrement(double i)
{
  majorIncrement_ = i;

  calc();
}

void
CGnuPlotAxis::
setTickIncrement(int tickIncrement)
{
  tickIncrement_ = tickIncrement;

  calc();
}

void
CGnuPlotAxis::
setTickSpaces(double *tickSpaces, int numTickSpaces)
{
  tickSpaces_.resize(numTickSpaces);

  memcpy(&tickSpaces_[0], tickSpaces, numTickSpaces*sizeof(double));
}

bool
CGnuPlotAxis::
calc()
{
  reverse_ = (start_ > end_);

  if (logBase().isValid()) {
    start1_ = CMathRound::RoundDown(start_);
    end1_   = CMathRound::RoundUp  (end_);

    numTicks1_ = int(end1_ - start1_);
    numTicks2_ = 10;

    return true;
  }

  return calcTics(start_, end_, tickIncrement_, majorIncrement_,
                  start1_, end1_, numTicks1_, numTicks2_);
}

bool
CGnuPlotAxis::
calcTics(double start, double end, double &start1, double &end1, int &numTicks1, int &numTicks2)
{
  return calcTics(start, end, 0, 0, start1, end1, numTicks1, numTicks2);
}

bool
CGnuPlotAxis::
calcTics(double start, double end, double inc, double &start1, double &end1,
         int &numTicks1, int &numTicks2)
{
  return calcTics(start, end, 0, inc, start1, end1, numTicks1, numTicks2);
}

bool
CGnuPlotAxis::
calcTics(double start, double end, int tickIncrement, double majorIncrement,
         double &start1, double &end1, int &numTicks1, int &numTicks2)
{
  numTicks1 = 1;
  numTicks2 = 0;

  //------

  // Ensure Axis Start and End are in the Correct Order
  double minAxis = std::min(start, end);
  double maxAxis = std::max(start, end);

  //------

  // Calculate Length

  double length = fabs(maxAxis - minAxis);

  if (length == 0.0)
    return false;

  //------

  // Calculate nearest Power of Ten to Length

  int power = CMathRound::RoundDown(log10(length));

  //------

  if (majorIncrement <= 0.0) {
    // Set Default Increment to 0.1 * Power of Ten
    double increment = 0.1;

    if      (power < 0) {
      for (int i = 0; i < -power; ++i)
        increment /= 10.0;
    }
    else if (power > 0) {
      for (int i = 0; i <  power; ++i)
        increment *= 10.0;
    }

    //------

    // Calculate other test Increments

    for (int i = 0; i < MAX_GAP_TESTS; ++i) {
      CGnuPlotAxisIncrementTest &tval = axesIncrementTests[i];

      tval.incFactor = increment*tval.factor;
    }

    //------

    // Set Default Start/End to Force Update

    start1 = 0.0;
    end1   = 0.0;

    //------

    // Test each Increment in turn

    int numGaps, numGapTicks;

    for (int i = 0; i < MAX_GAP_TESTS; ++i) {
      const CGnuPlotAxisIncrementTest &tval = axesIncrementTests[i];

      if (tickIncrement > 0) {
        int incFactor1 = int(tval.incFactor);

        if (double(incFactor1) != tval.incFactor)
          continue;

        if (incFactor1 % tickIncrement != 0)
          continue;
      }

      testAxisGaps(minAxis, maxAxis, tval.incFactor, tval.numTicks,
                   &start1, &end1, &increment, &numGaps, &numGapTicks);
    }

    //------

    // Set the Gap Positions

    numTicks1 = CMathRound::RoundDown((end1 - start1)/increment + 0.5);
    numTicks2 = numGapTicks;
  }
  else {
    start1    = CMathRound::RoundDown(start/majorIncrement)*majorIncrement;
    end1      = CMathRound::RoundUp  (end  /majorIncrement)*majorIncrement;
    numTicks1 = CMathRound::RoundDown((end1 - start1)/majorIncrement + 0.5);
    numTicks2 = 5;
  }

  numTicks1 = std::min(numTicks1, 256);
  numTicks2 = std::min(numTicks1, 10);

  return true;
}

bool
CGnuPlotAxis::
testAxisGaps(double start, double end, double testIncrement, int testNumGapTicks,
             double *start1, double *end1, double *increment, int *numGaps, int *numGapTicks)
{
  // Calculate New Start and End implied by the Test Increment

  errno = 0;
  double newStart = CMathRound::RoundDown(start/testIncrement)*testIncrement;
  if (errno == ERANGE) return false;

  errno = 0;
  double newEnd   = CMathRound::RoundUp  (end  /testIncrement)*testIncrement;
  if (errno == ERANGE) return false;

  while (newStart > start)
    newStart -= testIncrement;

  while (newEnd < end)
    newEnd += testIncrement;

  errno = 0;
  int testNumGaps = CMathRound::RoundUp((newEnd - newStart)/testIncrement);
  if (errno == ERANGE) return false;

  //------

  // If nothing set yet just update values and return

  if (*start1 == 0.0 && *end1 == 0.0) {
    *start1 = newStart;
    *end1   = newEnd;

    *increment   = testIncrement;
    *numGaps     = testNumGaps;
    *numGapTicks = testNumGapTicks;

    return true;
  }

  //------

  // If the current number of gaps is not within the acceptable range
  // and the new number of gaps is within the acceptable range then
  // update current

  if ((   *numGaps <  MIN_GOOD_TICKS ||    *numGaps >  MAX_GOOD_TICKS) &&
      (testNumGaps >= MIN_GOOD_TICKS && testNumGaps <= MAX_GOOD_TICKS)) {
    *start1 = newStart;
    *end1   = newEnd;

    *increment    = testIncrement;
    *numGaps     = testNumGaps;
    *numGapTicks = testNumGapTicks;

    return true;
  }

  //------

  // If the current number of gaps is not within the acceptable range
  // and the new number of gaps is not within the acceptable range then
  // consider it for update of current if better fit

  if ((   *numGaps < MIN_GOOD_TICKS ||    *numGaps > MAX_GOOD_TICKS) &&
      (testNumGaps < MIN_GOOD_TICKS || testNumGaps > MAX_GOOD_TICKS)) {
    // Calculate how close fit is to required range

    double delta1 = fabs(newStart - start) + fabs(newEnd - end);

    //------

    // If better fit than current fit or equally good fit and
    // number of gaps is nearer to optimum (OPTIMUM_TICKS) then
    // update current

    double delta2 = fabs(*start1 - start) + fabs(*end1 - end);

    if (((fabs(delta1 - delta2) < 1E-6) &&
         (abs(int(testNumGaps) - OPTIMUM_TICKS) < abs(int(*numGaps) - OPTIMUM_TICKS))) ||
        delta1 < delta2) {
      *start1 = newStart;
      *end1   = newEnd;

      *increment   = testIncrement;
      *numGaps     = testNumGaps;
      *numGapTicks = testNumGapTicks;

      return true;
    }
  }

  //------

  // If the current number of gaps is within the acceptable range
  // and the new number of gaps is within the acceptable range then
  // consider it for update of current if better fit

  if ((   *numGaps >= MIN_GOOD_TICKS &&    *numGaps <= MAX_GOOD_TICKS) &&
      (testNumGaps >= MIN_GOOD_TICKS && testNumGaps <= MAX_GOOD_TICKS)) {
    // Calculate how close fit is to required range

    double delta1 = fabs(newStart - start) + fabs(newEnd - end);

    //------

    // If better fit than current fit or equally good fit and
    // number of gaps is nearer to optimum (OPTIMUM_TICKS) then
    // update current

    double delta2 = fabs(*start1 - start) + fabs(*end1 - end);

    if (((fabs(delta1 - delta2) < 1E-6) &&
         (abs(int(testNumGaps) - OPTIMUM_TICKS) < abs(int(*numGaps) - OPTIMUM_TICKS))) ||
        delta1 < delta2) {
      *start1 = newStart;
      *end1   = newEnd;

      *increment   = testIncrement;
      *numGaps     = testNumGaps;
      *numGapTicks = testNumGapTicks;

      return true;
    }
  }

  return false;
}

double
CGnuPlotAxis::
getStart2() const
{
  if (data_.isRangeLimited())
    return getDataStart();
  else
    return getStart1();
}

double
CGnuPlotAxis::
getEnd2() const
{
  if (data_.isRangeLimited())
    return getDataEnd();
  else
    return getEnd1();
}

double
CGnuPlotAxis::
getMajorIncrement() const
{
  if (majorIncrement_ > 0.0)
    return majorIncrement_;
  else {
    if (numTicks1_ > 0)
      return (end1_ - start1_)/numTicks1_;
    else
      return 0.0;
  }
}

double
CGnuPlotAxis::
getMinorIncrement() const
{
  if (numTicks1_ > 0 && numTicks2_ > 0)
    return (end1_ - start1_)/(numTicks1_*numTicks2_);
  else
    return 0.0;
}

bool
CGnuPlotAxis::
hasMajorTicLabels() const
{
  return data_.hasRTicLabels(0);
}

bool
CGnuPlotAxis::
hasMinorTicLabels() const
{
  return data_.hasRTicLabels(1);
}

CGnuPlotAxisData::RTicLabels
CGnuPlotAxis::
getMajorTicLabels() const
{
  return data_.rticLabels(0);
}

CGnuPlotAxisData::RTicLabels
CGnuPlotAxis::
getMinorTicLabels() const
{
  return data_.rticLabels(1);
}

std::string
CGnuPlotAxis::
getValueStr(int i, double pos) const
{
  double pos1 = unmapLogValue(pos);

  if (type() == AxisType::Y && group_->isPolar()) {
    CGnuPlotAxis *plotRAxis = group_->getPlotAxis(AxisType::R, 1, true);

    if (pos1 > 0)
      pos1 = pos1 + plotRAxis->getStart();
    else
      pos1 = plotRAxis->getStart() - pos1;
  }

  return data_.getAxisValueStr(i, pos1);
}

void
CGnuPlotAxis::
drawAxes(CGnuPlotRenderer *renderer)
{
  renderer_ = renderer;

  setBoundingBox();

  //---

  if (! isDisplayed())
    return;

  drawAxisLine();

  //---

  drawAxis();

  //---

  drawAxisLabel();
}

void
CGnuPlotAxis::
setBoundingBox()
{
  bool first = (ind() == 1);

  // calc bounding box (TODO: 3D box)
  CPoint3D p1 = valueToPoint(getStart1(), first, isZeroAxisDisplayed());
  CPoint3D p2 = valueToPoint(getEnd1  (), first, isZeroAxisDisplayed());

  double px = renderer_->pixelWidthToWindowWidth  (2);
  double py = renderer_->pixelHeightToWindowHeight(2);

  if (! group_->is3D()) {
    CPoint2D pt1 = renderer_->transform2D(p1);
    CPoint2D pt2 = renderer_->transform2D(p2);

    if      (direction_ == AxisDirection::X) {
      CPoint2D d(0, py/2);

      if (type() == AxisType::R)
        bbox2D_ = CBBox2D(pt1 - d, pt2 + d);
      else
        bbox2D_ = CBBox2D(pt1 - d, pt2 + d);
    }
    else if (direction_ == AxisDirection::Y) {
      CPoint2D d(px/2, 0);

      bbox2D_ = CBBox2D(pt1 - d, pt2 + d);
    }
    else if (direction_ == AxisDirection::Z) {
      CPoint2D d(0, py/2);

      bbox2D_ = CBBox2D(pt1 - d, pt2 + d);
    }
  }
  else {
    double pz = px;

    CPoint3D d(px/2, py/2, pz/2);

    bbox3D_ = CBBox3D(p1 - d, p2 + d);
  }
}

void
CGnuPlotAxis::
drawAxisLine()
{
  bool first = (ind() == 1);

  // Draw Axis Line (radial or zero axis)
  if (type() != AxisType::R) {
    if (isZeroAxisDisplayed())
      drawZeroLine(renderer_, zeroAxisLineColor(), zeroAxisLineWidth(), zeroAxisLineDash());
  }
  else {
    CPoint3D p1 = valueToPoint(getStart(), first, false);
    CPoint3D p2 = valueToPoint(getEnd  (), first, false);

    drawLine(p1, p2, labelColor().color(), 1, CLineDash());
  }
}

void
CGnuPlotAxis::
drawLowerLine(CGnuPlotRenderer *renderer, const CRGBA &c, double w, const CLineDash &dash)
{
  renderer_ = renderer;

  CPoint3D p1 = valueToPoint(getStart2(), true, false);
  CPoint3D p2 = valueToPoint(getEnd2  (), true, false);

  drawLine(p1, p2, c, w, dash);
}

void
CGnuPlotAxis::
drawUpperLine(CGnuPlotRenderer *renderer, const CRGBA &c, double w, const CLineDash &dash)
{
  renderer_ = renderer;

  CPoint3D p1 = valueToPoint(getStart2(), false, false);
  CPoint3D p2 = valueToPoint(getEnd2  (), false, false);

  drawLine(p1, p2, c, w, dash);
}

void
CGnuPlotAxis::
drawZeroLine(CGnuPlotRenderer *renderer, const CRGBA &c, double w, const CLineDash &dash)
{
  renderer_ = renderer;

  CPoint3D p1 = valueToPoint(getStart1(), true, true);
  CPoint3D p2 = valueToPoint(getEnd1  (), true, true);

  drawLine(p1, p2, c, w, dash);
}

void
CGnuPlotAxis::
drawAxis()
{
  CFontPtr font = renderer_->getFont();

  maxW_ = 0;
  maxH_ = font->getCharHeight();

  //---

  // Draw Ticks Marks and Labels

  double increment = getMajorIncrement();

  double pos1 = getStart1();
  double pos2 = pos1;

  for (int i = 0; i < getNumMajorTicks(); ++i) {
    // Calculate Next Tick Point
    pos2 = (i + 1)*increment + getStart1();

    //------*/

    // Draw Major Tick

    if (! hasMajorTicLabels() && isVisibleValue(pos1)) {
      // Draw Tick Mark
      drawAxisTicks(pos1, true);

      //------*/

      // Create Major Tick Label (Override with application supplied string if required)
      std::string str = getValueStr(i, pos1);

      maxW_ = std::max(maxW_, font->getStringWidth(str));

      //------*/

      // Draw Tick Labels
      drawTickLabels(pos1, str);
    }

    //------*/

    // Draw Minor Ticks (use user defined distribution if defined)

    if      (getTickSpaces() == nullptr) {
      if (! hasMinorTicLabels()) {
        double mpos1 = unmapLogValue(pos1);
        double mpos2 = unmapLogValue(pos2);

        double dt = (mpos2 - mpos1)/(getNumMinorTicks() - 1);

        if (checkMinorTickSize(dt)) {
          for (int j = 1; j < getNumMinorTicks(); j++) {
            double mpos3 = j*dt + mpos1;

            if (isDrawMinorTickMark()) {
              double pos3 = mapLogValue(mpos3);

              if (! hasMinorTicLabels() && isVisibleValue(pos3)) {
                // Draw Tick Mark
                drawAxisTicks(pos3, false);
              }
            }
          }
        }
      }
    }
    else {
      for (int j = 1; j <= getNumTickSpaces() - 2; j++) {
        double pos3 = getTickSpace(j)*(pos2 - pos1) + pos1;

        if (isDrawMinorTickMark()) {
          if (! hasMinorTicLabels() && isVisibleValue(pos3)) {
            // Draw Tick Mark
            drawAxisTicks(pos3, false);
          }
        }
      }
    }

    //------*/

    pos1 = pos2;
  }

  //------*/

  // Draw Last Major Tick
  if (! hasMajorTicLabels() && isVisibleValue(pos1)) {
    // Draw Tick Mark
    drawAxisTicks(pos1, true);

    //------*/

    // Create Major Tick Label (Override with application supplied string if required)
    std::string str = getValueStr(getNumMajorTicks(), pos1);

    maxW_ = std::max(maxW_, font->getStringWidth(str));

    //------*/

    // Draw Tick Label
    drawTickLabels(pos1, str);
  }

  //------*/

  // Draw Custom Major Tic Labels
  if (hasMajorTicLabels()) {
    const CGnuPlotAxisData::RTicLabels &ticLabels = getMajorTicLabels();

    for (const auto &label : ticLabels) {
      double      r = mapLogValue(label.first);
      std::string s = label.second;

      maxW_ = std::max(maxW_, font->getStringWidth(s));

      // Draw Tick Mark
      drawAxisTicks(r, true);

      // Draw Tick Label
      drawTickLabels(r, s);
    }
  }

  if (hasMinorTicLabels()) {
    const CGnuPlotAxisData::RTicLabels &ticLabels = getMinorTicLabels();

    for (const auto &label : ticLabels) {
      double      r = mapLogValue(label.first);
      std::string s = label.second;

      maxW_ = std::max(maxW_, font->getStringWidth(s));

      // Draw Tick Mark
      drawAxisTicks(r, false);

      // Draw Tick Label
      drawTickLabels(r, s);
    }
  }
}

void
CGnuPlotAxis::
drawAxisLabel()
{
  if (! isDrawLabel())
    return;

  // Draw Axis Label

  double m = 0.0;

  if (logBase().isValid())
    m = (getStart () + getEnd ())/2;
  else
    m = (getStart1() + getEnd1())/2;

  bool first = (ind() == 1);

  const std::string &astr = getLabel();

  if      (direction_ == AxisDirection::X)
    drawAxisLabelStr(m, astr, int(maxH_), first);
  else if (direction_ == AxisDirection::Y)
    drawAxisLabelStr(m, astr, int(maxH_), first);
  else
    drawAxisLabelStr(m, astr, int(maxH_), first);
}

void
CGnuPlotAxis::
drawAxisTicks(double pos, bool large)
{
  if (! isDrawTickMark()) return;

  if (isBorderTics()) {
    if (ind() == 1) {
      drawAxisTick(pos, true, large);

      if (isMirror())
        drawAxisTick(pos, false, large);
    }
    else
      drawAxisTick(pos, false, large);
  }
  else {
    if (ind() == 1) {
      drawAxisTick(pos, true, large);
    }
  }
}

void
CGnuPlotAxis::
drawAxisTick(double pos, bool first, bool large)
{
  // tick on border:
  //  in  draw towards center (up bottom, down top)
  //  out draw away from center (down bottom, up top)
  // tick on axis
  //  in/out draw down
  //  mirror draw up/down
  CPoint3D p = valueToPoint(reverse_ ? end_ - (pos - start_) : pos, first, ! isBorderTics());

  if (clip() && renderer_->clip().isValid() &&
      ! renderer_->clip().getValue().inside(CPoint2D(p.x, p.y)))
    return;

  double psize = 6*(large ? getTicMajorScale() : getTicMinorScale());
  if (psize <= 0.0) return;

  double xsize = renderer_->pixelWidthToWindowWidth  (psize);
  double ysize = renderer_->pixelHeightToWindowHeight(psize);

  CPoint3D p1 = p;
  CPoint3D p2 = p;

  CPoint3D pd = ivalueToPoint(CPoint3D(xsize, ysize, 0));

  if (isBorderTics()) {
    if (first) {
      if (isTickInside())
        p1 += pd;
      else
        p2 -= pd;
    }
    else {
      if (! isTickInside())
        p1 += pd;
      else
        p2 -= pd;
    }
  }
  else {
    if (! isMirror()) {
      p2 -= pd;
    }
    else {
      p1 += pd;
      p2 -= pd;
    }
  }


  CRGBA c = ticColor().color();

  renderer_->drawLine(p1, p2, c, 1, CLineDash());

  //---

  group_->updateAxisBBox(xind(), yind(), zind(), p1);
  group_->updateAxisBBox(xind(), yind(), zind(), p2);
}

void
CGnuPlotAxis::
drawTickLabels(double pos, const std::string &str)
{
  if (isBorderTics()) {
    if (ind() == 1) {
      if (isDrawTickLabel())
        drawTickLabel(pos, str, true);

      if (isMirror()) {
        if (isDrawTickLabel1())
          drawTickLabel(pos, str, false);
      }
    }
    else {
      if (isDrawTickLabel())
        drawTickLabel(pos, str, false);
    }
  }
  else {
    if (ind() == 1) {
      if (isDrawTickLabel())
        drawTickLabel(pos, str, true);
    }
  }
}

void
CGnuPlotAxis::
drawTickLabel(double pos, const std::string &str, bool first)
{
  if (ticFont())
    renderer_->setFont(ticFont());

  CPoint3D p = valueToPoint(reverse_ ? end_ - (pos - start_) : pos, first, ! isBorderTics());

  if (clip() && renderer_->clip().isValid() &&
      ! renderer_->clip().getValue().inside(CPoint2D(p.x, p.y)))
    return;

  CRGBA c = ticColor().color();

  double psize = 6*getTicMajorScale();

  double xsize = renderer_->pixelWidthToWindowWidth  (psize + 2);
  double ysize = renderer_->pixelHeightToWindowHeight(psize + 2);

//bool rotatedText = (direction_ == AxisDirection::Y);
  bool rotatedText = false;

  if      (direction_ == AxisDirection::X) {
    CPoint3D    p1;
    CVAlignType valign;

    if (first) {
      p1     = perpPoint(p, (isLabelInside() ? ysize : -ysize));
      valign = (isLabelInside() ? CVALIGN_TYPE_BOTTOM : CVALIGN_TYPE_TOP);
    }
    else {
      p1     = perpPoint(p, (! isLabelInside() ? ysize : -ysize));
      valign = (! isLabelInside() ? CVALIGN_TYPE_BOTTOM : CVALIGN_TYPE_TOP);
    }

    drawHAlignedText(p1, HAlignPos(CHALIGN_TYPE_CENTER, 0), VAlignPos(valign, 0),
                     str, c, ticsRotate());
  }
  else if (direction_ == AxisDirection::Y) {
    CHAlignType halign;

    if (first)
      halign = (isLabelInside() ? CHALIGN_TYPE_LEFT : CHALIGN_TYPE_RIGHT);
    else
      halign = (! isLabelInside() ? CHALIGN_TYPE_LEFT : CHALIGN_TYPE_RIGHT);

    if (rotatedText) {
      CPoint3D p1;

      if (first)
        p1 = perpPoint(p, (isLabelInside() ? ysize : -ysize));
      else
        p1 = perpPoint(p, (! isLabelInside() ? ysize : -ysize));

      drawVAlignedText(p1, HAlignPos(halign, 0), VAlignPos(CVALIGN_TYPE_CENTER, 0),
                       str, c, ticsRotate());
    }
    else {
      CPoint3D p1;

      if (first)
        p1 = perpPoint(p, (isLabelInside() ? xsize : -xsize));
      else
        p1 = perpPoint(p, (! isLabelInside() ? xsize : -xsize));

      drawHAlignedText(p1, HAlignPos(halign, 0), VAlignPos(CVALIGN_TYPE_CENTER, 0),
                       str, c, ticsRotate());
    }
  }
  else {
    CPoint3D    p1;
    CHAlignType halign;

    if (first) {
      p1     = perpPoint(p, (isLabelInside() ? xsize : -xsize));
      halign = (isLabelInside() ? CHALIGN_TYPE_LEFT : CHALIGN_TYPE_RIGHT);
    }
    else {
      p1     = perpPoint(p, (! isLabelInside() ? xsize : -xsize));
      halign = (! isLabelInside() ? CHALIGN_TYPE_LEFT : CHALIGN_TYPE_RIGHT);
    }

    drawHAlignedText(p1, HAlignPos(halign, 0), VAlignPos(CVALIGN_TYPE_CENTER, 0),
                     str, c, ticsRotate());
  }
}

void
CGnuPlotAxis::
drawAxisLabelStr(double pos, const std::string &str, int maxSize, bool first)
{
  if (labelFont())
    renderer_->setFont(labelFont());

  CRGBA c = labelColor().color();

  CPoint3D p = valueToPoint(reverse_ ? end_ - (pos - start_) : pos, first, isZeroAxisDisplayed());

  double pw = renderer_->pixelWidthToWindowWidth  (1);
  double ph = renderer_->pixelHeightToWindowHeight(1);

  double xsize = (maxSize + 12)*pw;
  double ysize = (maxSize + 12)*ph;

  if      (direction_ == AxisDirection::X) {
    CPoint3D    p1;
    CVAlignType valign;

    if (first) {
      p1     = perpPoint(p, (  isLabelInside() ? ysize : -ysize));
      valign = (  isLabelInside() ? CVALIGN_TYPE_BOTTOM : CVALIGN_TYPE_TOP);
    }
    else {
      p1     = perpPoint(p, (! isLabelInside() ? ysize : -ysize));
      valign = (! isLabelInside() ? CVALIGN_TYPE_BOTTOM : CVALIGN_TYPE_TOP);
    }

    drawHAlignedText(p1, HAlignPos(CHALIGN_TYPE_CENTER, 0), VAlignPos(valign, 0),
                     str, c, labelRotate().getValue(0));
  }
  else if (direction_ == AxisDirection::Y) {
    CPoint3D p1;

    double dx = 4*pw;

    if (first) {
      if (! isLabelInside()) {
        p.x = group_->axisBBox2D().getXMin();

        p1 = perpPoint(p, -dx);
      }
      else {
        p.x = group_->axisBBox2D().getXMax();

        p1 = perpPoint(p, dx);
      }
    }
    else {
      p.x = group_->axisBBox2D().getXMax();

      p1 = perpPoint(p, (! isLabelInside() ? dx : -dx));
    }

    if (group_->is3D()) {
      CVAlignType valign;

      if (first)
        valign = (  isLabelInside() ? CVALIGN_TYPE_BOTTOM : CVALIGN_TYPE_TOP);
      else
        valign = (! isLabelInside() ? CVALIGN_TYPE_BOTTOM : CVALIGN_TYPE_TOP);

      drawHAlignedText(p1, HAlignPos(CHALIGN_TYPE_CENTER, 0), VAlignPos(valign, 0),
                       str, c, labelRotate().getValue(90));
    }
    else {
      CHAlignType halign;

      if (first)
        halign = (  isLabelInside() ? CHALIGN_TYPE_LEFT : CHALIGN_TYPE_RIGHT);
      else
        halign = (! isLabelInside() ? CHALIGN_TYPE_LEFT : CHALIGN_TYPE_RIGHT);

      drawVAlignedText(p1, HAlignPos(halign, 0), VAlignPos(CVALIGN_TYPE_CENTER, 0),
                       str, c, labelRotate().getValue(90));
    }
  }
  else {
    CPoint3D    p1;
    CHAlignType halign;

    if (first) {
      p1     = perpPoint(p, (  isLabelInside() ? xsize : -xsize));
      halign = (  isLabelInside() ? CHALIGN_TYPE_LEFT : CHALIGN_TYPE_RIGHT);
    }
    else {
      p1     = perpPoint(p, (! isLabelInside() ? xsize : -xsize));
      halign = (! isLabelInside() ? CHALIGN_TYPE_LEFT : CHALIGN_TYPE_RIGHT);
    }

    drawVAlignedText(p1, HAlignPos(halign, 0), VAlignPos(CVALIGN_TYPE_CENTER, 0),
                     str, c, labelRotate().getValue(0));
  }
}

void
CGnuPlotAxis::
drawGrid(CGnuPlotRenderer *renderer)
{
  static double gridDashes[] = {1, 3};
  static int    numGridDashes = 2;

  renderer_ = renderer;

  //----

  CLineDash lineDash(gridDashes, numGridDashes);

  CRGBA c(0,0,0);

  double w1 = gridMajorLineWidth();
  double w2 = gridMinorLineWidth();

  //----

  if (hasGridMajor() || hasGridMinor()) {
    double mi = getMajorIncrement();

    // draw grid on major ticks
    for (int i = 0; i <= getNumMajorTicks(); ++i) {
      double pos1 = i*mi + getStart1();
      double pos2 = pos1 + mi;

      if (hasGridMajor()) {
        if (! hasMajorTicLabels() && isVisibleValue(pos1)) {
          CPoint3D p1 = valueToPoint(pos1, /*first*/false, /*zero*/false);
          CPoint3D p2 = valueToPoint(pos1, /*first*/true , /*zero*/false);

          drawClipLine(p1, p2, c, w1, lineDash);
        }
      }

      if (hasGridMinor()) {
        double mpos1 = unmapLogValue(pos1);
        double mpos2 = unmapLogValue(pos2);

        double dt = (mpos2 - mpos1)/(getNumMinorTicks() - 1);

        if (checkMinorTickSize(dt)) {
          for (int j = 1; j < getNumMinorTicks(); j++) {
            double mpos3 = j*dt + mpos1;

            double pos3 = mapLogValue(mpos3);

            if (! hasMinorTicLabels() && isVisibleValue(pos3)) {
              CPoint3D p11 = valueToPoint(pos3, /*first*/false, /*zero*/false);
              CPoint3D p21 = valueToPoint(pos3, /*first*/true , /*zero*/false);

              drawClipLine(p11, p21, c, w2, lineDash);
            }
          }
        }
      }
    }
  }

  // Draw Grid at Custom Major Tic Labels
  if (hasMajorTicLabels()) {
    const CGnuPlotAxisData::RTicLabels &ticLabels = getMajorTicLabels();

    for (const auto &label : ticLabels) {
      double r = mapLogValue(label.first);

      CPoint3D p1 = valueToPoint(r, /*first*/false, /*zero*/false);
      CPoint3D p2 = valueToPoint(r, /*first*/true , /*zero*/false);

      drawClipLine(p1, p2, c, w1, lineDash);
    }
  }
}

void
CGnuPlotAxis::
drawRadialGrid(CGnuPlotRenderer *renderer)
{
  bool first = true;

  static double gridDashes[] = {1, 3};
  static int    numGridDashes = 2;

  renderer_ = renderer;

  //----

  CLineDash lineDash(gridDashes, numGridDashes);

  CRGBA c(0,0,0);

  double w1 = gridMajorLineWidth();
  double w2 = gridMinorLineWidth();

  //----

  double da = CAngle::Deg2Rad(gridPolarAngle());
  int    na = int(2*M_PI/da + 0.5);

  double r1, r2;

  if (! logBase().isValid()) {
    r1 = getStart();
    r2 = getEnd  ();
    //r1 = getStart1();
    //r2 = getEnd1  ();
  }
  else {
    r1 = getStart();
    r2 = getEnd  ();
  }

  CPoint3D pe1(0, 0, 0);

  for (int i = 0; i < na; ++i) {
    CPoint3D pe2 = valueToPoint(r1, first, false);
    CPoint3D pe3 = valueToPoint(r2, first, false);

    double a = i*da;

    double x1 = pe2.x*cos(a);
    double y1 = pe2.x*sin(a);
    double x2 = pe3.x*cos(a);
    double y2 = pe3.x*sin(a);

    drawClipLine(CPoint3D(x1, y1, 0), CPoint3D(x2, y2, 0), c, w1, lineDash);
  }

  if (hasGridMajor()) {
    if (hasMajorTicLabels()) {
      const CGnuPlotAxisData::RTicLabels &ticLabels = getMajorTicLabels();

      for (const auto &label : ticLabels) {
        double x1 = mapLogValue(label.first);

        if (isVisibleValue(x1)) {
          CPoint3D pe2 = valueToPoint(x1, first, false);

          renderer_->drawEllipse(pe1, pe2.x, pe2.x, 0, c, w1, lineDash);
        }
      }
    }
    else {
      double xincrement = getMajorIncrement();

      for (int i = 0; i <= getNumMajorTicks(); ++i) {
        double x1 = i*xincrement + getStart1();
        double x2 = x1 + xincrement;

        if (isVisibleValue(x1)) {
          CPoint3D pe2 = valueToPoint(x1, first, false);

          renderer_->drawEllipse(pe1, pe2.x, pe2.x, 0, c, w1, lineDash);
        }

        if (hasGridMinor()) {
          double mx1 = unmapLogValue(x1);
          double mx2 = unmapLogValue(x2);

          double dt = (mx2 - mx1)/(getNumMinorTicks() - 1);

          if (checkMinorTickSize(dt)) {
            for (int j = 1; j < getNumMinorTicks(); ++j) {
              double mx3 = j*dt + x1;

              double x3 = mapLogValue(mx3);

              if (isVisibleValue(x3)) {
                CPoint3D pe3(x3, x3, 0);

                renderer_->drawEllipse(pe1, pe3.x, pe3.y, 0, c, w2, lineDash);
              }
            }
          }
        }
      }
    }
  }
}

bool
CGnuPlotAxis::
isVisibleValue(double pos) const
{
  //if (logBase().isValid() || type() == AxisType::R)
  //  return (pos >= getStart () && pos <= getEnd ());
  //else
  //  return (pos >= getStart2() && pos <= getEnd2());
  return (pos >= getStart() && pos <= getEnd());
}

bool
CGnuPlotAxis::
checkMinorTickSize(double d) const
{
  double dp = 0;

  if (direction_ == AxisDirection::X || direction_ == AxisDirection::Z)
    dp = renderer_->windowWidthToPixelWidth(d);
  else
    dp = renderer_->windowHeightToPixelHeight(d);

  return (dp > 2);
}

void
CGnuPlotAxis::
drawClipLine(const CPoint3D &p1, const CPoint3D &p2, const CRGBA &c,
             double w, const CLineDash &lineDash)
{
  if (CGnuPlotUtil::realEq(p1.z, p2.z)) {
    CPoint2D p11(p1.x, p1.y);
    CPoint2D p21(p2.x, p2.y);

    if (! clip() || renderer_->clipLine(p11, p21))
      drawLine(CPoint3D(p11.x, p11.y, p1.z), CPoint3D(p21.x, p21.y, p2.z), c, w, lineDash);
  }
  else
    drawLine(p1, p2, c, w, lineDash);
}

void
CGnuPlotAxis::
drawLine(const CPoint3D &p1, const CPoint3D &p2, const CRGBA &c,
         double w, const CLineDash &lineDash)
{
  if (! renderer_->isPseudo() && group_->isHidden3D())
    renderer_->drawHiddenLine(p1, p2, c, w, lineDash);
  else
    renderer_->drawLine(p1, p2, c, w, lineDash);
}

void
CGnuPlotAxis::
drawHAlignedText(const CPoint3D &pos, const HAlignPos &halignPos, const VAlignPos &valignPos,
                 const std::string &str, const CRGBA &c, double angle)
{
  if (str == "")
    return;

  CPoint2D pos1 = renderer_->transform2D(pos);

  if (! group_->is3D()) {
    CBBox2D bbox, rbbox;

    renderer_->calcTextRectAtPoint(pos1, str, isEnhanced(), halignPos, valignPos,
                                   angle, bbox, rbbox);

    if (group_->isHidden3D())
      renderer_->drawHiddenTextAtPoint(pos, str, isEnhanced(), halignPos, valignPos,
                                       renderer_->getFont(), c, angle);
    else
      renderer_->drawTextAtPoint(pos, str, isEnhanced(), halignPos, valignPos, c, angle);

    if (bbox.isSet()) {
      maxH_ = std::max(maxH_, renderer_->windowHeightToPixelHeight(bbox.getHeight()));

      group_->updateAxisBBox(xind(), yind(), rbbox);
    }
  }
  else {
    CFontPtr font = renderer_->getFont();

    double tw = renderer_->pixelWidthToWindowWidth  (font->getStringWidth(str));
    double ta = renderer_->pixelHeightToWindowHeight(font->getCharAscent ());
    double td = renderer_->pixelHeightToWindowHeight(font->getCharDescent());

    CPoint2D tpos1 = renderer_->transform2D(pos);

    CBBox2D bbox(CPoint2D(tpos1.x, tpos1.y - td), CPoint2D(tpos1.x + tw, tpos1.y + ta));

    double dx = 0.0, dy = 0.0;

    if      (halignPos.first == CHALIGN_TYPE_LEFT  ) dx = 0;
    else if (halignPos.first == CHALIGN_TYPE_CENTER) dx = -bbox.getWidth()/2;
    else if (halignPos.first == CHALIGN_TYPE_RIGHT ) dx = -bbox.getWidth();

    if      (valignPos.first == CVALIGN_TYPE_BOTTOM) dy = bbox.getHeight();
    else if (valignPos.first == CVALIGN_TYPE_CENTER) dy = bbox.getHeight()/2;
    else if (valignPos.first == CVALIGN_TYPE_TOP   ) dy = 0;

    CBBox2D bbox1 = bbox.moveBy(CPoint2D(dx, dy));

    if (! renderer_->isPseudo() && group_->isHidden3D())
      renderer_->drawHiddenHAlignedText(pos, halignPos, valignPos, str, font, c, angle);
    else
      renderer_->drawHAlignedText(pos, halignPos, valignPos, str, c, angle);

    group_->updateAxisBBox(xind(), yind(), bbox1);
//renderer_->drawRect(bbox1, CRGBA(1,0,0), 1);
  }
}

void
CGnuPlotAxis::
drawVAlignedText(const CPoint3D &pos, const HAlignPos &halignPos, const VAlignPos &valignPos,
                 const std::string &str, const CRGBA &c, double angle)
{
  if (str == "")
    return;

  renderer_->drawVAlignedText(pos, halignPos, valignPos, str, c, angle);

  CFontPtr font = renderer_->getFont();

  double px = renderer_->pixelWidthToWindowWidth(font->getCharAscent());

  group_->updateAxisBBox(xind(), yind(), renderer_->transform2D(pos) - CPoint2D(px, 0));
}

CPoint3D
CGnuPlotAxis::
valueToPoint(double v, bool first, bool zero) const
{
  CPoint3D p = v_*v;

  CBBox3D bbox = group_->getPlotBorderBox(1, 1, 1);

  if (type() == AxisType::R) {
    if (! logBase().isValid()) {
      double s = getEnd() - getStart();

      p.x = CGnuPlotUtil::map(v, getStart(), getEnd(), 0, s);
      p.y = (bbox.getYMin() + bbox.getYMax())/2;
    }
  }
  else {
    if (zero) {
      if      (type() == AxisType::X) p += CPoint3D(0, zposition_, bbox.getZMin());
      else if (type() == AxisType::Y) p += CPoint3D(zposition_, 0, bbox.getZMin());
      else if (type() == AxisType::Z) p += CPoint3D(zposition_, bbox.getYMin(), 0);
    }
    else {
      CPoint3D iv1 = CPoint3D(1, 1, 1) - v_;

      if (first)
        p += iv1*bbox.getMin();
      else
        p += iv1*bbox.getBottomMax();
    }
  }

  return p;
}

CPoint3D
CGnuPlotAxis::
ivalueToPoint(const CPoint3D &d) const
{
  return CPoint3D(d.x*iv_.x, d.y*iv_.y, d.z*iv_.z);
}

CPoint2D
CGnuPlotAxis::
ivalueToPoint(const CPoint2D &d) const
{
  return CPoint2D(d.x*iv_.x, d.y*iv_.y);
}

CPoint3D
CGnuPlotAxis::
perpPoint(const CPoint3D &p, double d) const
{
  return p + d*iv_;
}
