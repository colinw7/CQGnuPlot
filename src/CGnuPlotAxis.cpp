#include <CGnuPlotAxis.h>
#include <CGnuPlotWindow.h>
#include <CGnuPlotPlot.h>
#include <CGnuPlotRenderer.h>

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
  uint   numTicks;
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
CGnuPlotAxis(CGnuPlotPlot *plot, COrientation direction, double start, double end) :
 plot_      (plot),
 direction_ (direction),
 start_     (start),
 end_       (end),
 start1_    (start),
 end1_      (end),
 tickSpaces_()
{
  calc();
}

CGnuPlotAxis::
~CGnuPlotAxis()
{
}

void
CGnuPlotAxis::
setRange(double start, double end)
{
  start_ = start;
  end_   = end;

  calc();
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
setTickIncrement(uint tickIncrement)
{
  tickIncrement_ = tickIncrement;

  calc();
}

void
CGnuPlotAxis::
setTickSpaces(double *tickSpaces, uint numTickSpaces)
{
  tickSpaces_.resize(numTickSpaces);

  memcpy(&tickSpaces_[0], tickSpaces, numTickSpaces*sizeof(double));
}

void
CGnuPlotAxis::
setLabel(const std::string &str)
{
  label_ = str;
}

void
CGnuPlotAxis::
setTimeFormat(const std::string &fmt)
{
  timeFmt_ = fmt;
}

bool
CGnuPlotAxis::
calc()
{
  numTicks1_ = 1;
  numTicks2_ = 0;

  //------

  // Ensure Axis Start and End are in the Correct Order

  double minAxis = std::min(start_, end_);
  double maxAxis = std::max(start_, end_);

  //------

  // Calculate Length

  double length = fabs(maxAxis - minAxis);

  if (length == 0.0)
    return false;

  //------

  // Calculate nearest Power of Ten to Length

  int power = CMathGen::RoundDown(log10(length));

  //------

  if (majorIncrement_ <= 0.0) {
    // Set Default Increment to 0.1 * Power of Ten
    double increment = 0.1;

    if      (power < 0) {
      for (int i = 0; i < -power; i++)
        increment /= 10.0;
    }
    else if (power > 0) {
      for (int i = 0; i <  power; i++)
        increment *= 10.0;
    }

    //------

    // Calculate other test Increments

    for (int i = 0; i < MAX_GAP_TESTS; i++)
      axesIncrementTests[i].incFactor = increment*axesIncrementTests[i].factor;

    //------

    // Set Default Start/End to Force Update

    start1_ = 0.0;
    end1_   = 0.0;

    //------

    // Test each Increment in turn

    uint numGaps, numGapTicks;

    for (int i = 0; i < MAX_GAP_TESTS; i++) {
      if (tickIncrement_ > 0) {
        int incFactor1 = (int) axesIncrementTests[i].incFactor;

        if (((double) incFactor1) != axesIncrementTests[i].incFactor)
          continue;

        if (incFactor1 % tickIncrement_ != 0)
          continue;
      }

      testAxisGaps(minAxis, maxAxis,
                   axesIncrementTests[i].incFactor,
                   axesIncrementTests[i].numTicks,
                   &start1_, &end1_, &increment,
                   &numGaps, &numGapTicks);
    }

    //------

    // Set the Gap Positions

    numTicks1_ = CMathGen::RoundDown((end1_ - start1_)/increment + 0.5);
    numTicks2_ = numGapTicks;
  }
  else {
    start1_    = start_;
    end1_      = end_;
    numTicks1_ = CMathGen::RoundDown((end1_ - start1_)/majorIncrement_ + 0.5);
    numTicks2_ = 5;
  }

  numTicks1_ = std::min(numTicks1_, 256U);
  numTicks2_ = std::min(numTicks1_, 10U);

  return true;
}

bool
CGnuPlotAxis::
testAxisGaps(double start, double end, double testIncrement, uint testNumGapTicks,
             double *start1, double *end1, double *increment, uint *numGaps, uint *numGapTicks)
{
  // Calculate New Start and End implied by the Test Increment

  double newStart = CMathGen::RoundDown(start/testIncrement)*testIncrement;
  double newEnd   = CMathGen::RoundUp  (end  /testIncrement)*testIncrement;

  while (newStart > start)
    newStart -= testIncrement;

  while (newEnd < end)
    newEnd += testIncrement;

  uint testNumGaps = CMathGen::RoundUp((newEnd - newStart)/testIncrement);

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
         (abs(testNumGaps - OPTIMUM_TICKS) < abs(*numGaps - OPTIMUM_TICKS))) ||
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
         (abs(testNumGaps - OPTIMUM_TICKS) < abs(*numGaps - OPTIMUM_TICKS))) ||
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

std::string
CGnuPlotAxis::
getValueStr(int i, double pos) const
{
  if (direction_ == CORIENTATION_HORIZONTAL)
    return plot_->getXAxisValueStr(i, pos);
  else
    return plot_->getYAxisValueStr(i, pos);
}

void
CGnuPlotAxis::
drawAxis(double pos)
{
  std::string str;

  if (! isDisplayed())
    return;

  double maxW = 0;
  double maxH = font_->getCharHeight();

  //------*/

  // Draw Axis Line

  if (drawLine()) {
    if (direction_ == CORIENTATION_HORIZONTAL)
      drawLine(CPoint2D(getStart(), pos), CPoint2D(getEnd(), pos));
    else
      drawLine(CPoint2D(pos, getStart()), CPoint2D(pos, getEnd()));
  }

  //------*/

  // Draw Ticks Marks and Labels

  double increment = getMajorIncrement();

  double pos1 = getStart();
  double pos2 = pos1;

  for (uint i = 0; i < getNumMajorTicks(); i++) {
    // Calculate Next Tick Point

    pos2 = (i + 1)*increment + getStart();

    //------*/

    // Draw Major Tick

    if (pos1 >= getStart() && pos1 <= getEnd()) {
      // Draw Tick Mark

      if (drawTickMark()) {
        if (direction_ == CORIENTATION_HORIZONTAL)
          drawAxisTick(CPoint2D(pos1, pos),
           (tickInside_ ? CDIRECTION_TYPE_UP : CDIRECTION_TYPE_DOWN), true);
        else
          drawAxisTick(CPoint2D(pos, pos1),
           (tickInside_ ? CDIRECTION_TYPE_RIGHT : CDIRECTION_TYPE_LEFT), true);
      }

      //------*/

      // Create Tick Label (Override with application supplied string if required)

      str = getValueStr(i, pos1);

      maxW = std::max(maxW, font_->getStringWidth(str));

      //------*/

      // Draw Tick Label

      if (drawTickLabel()) {
        if (direction_ == CORIENTATION_HORIZONTAL)
          drawTickLabel(CPoint2D(pos1, pos), str);
        else
          drawTickLabel(CPoint2D(pos, pos1), str);
      }
    }

    //------*/

    // Draw Minor Ticks (use user defined distribution if defined)

    if (getTickSpaces() == 0) {
      for (uint j = 1; j < getNumMinorTicks(); j++) {
        double pos3 = j*(pos2 - pos1)/getNumMinorTicks() + pos1;

        if (drawTickMark() && drawMinorTickMark()) {
          if (pos3 >= getStart() && pos3 <= getEnd()) {
            if (direction_ == CORIENTATION_HORIZONTAL)
              drawAxisTick(CPoint2D(pos3, pos),
                (tickInside_ ? CDIRECTION_TYPE_UP : CDIRECTION_TYPE_DOWN), false);
            else
              drawAxisTick(CPoint2D(pos, pos3),
                (tickInside_ ? CDIRECTION_TYPE_RIGHT : CDIRECTION_TYPE_LEFT), false);
          }
        }
      }
    }
    else {
      for (uint j = 1; j <= getNumTickSpaces() - 2; j++) {
        double pos3 = getTickSpace(j)*(pos2 - pos1) + pos1;

        if (drawTickMark() && drawMinorTickMark()) {
          if (pos3 >= getStart() && pos3 <= getEnd()) {
            if (direction_ == CORIENTATION_HORIZONTAL)
              drawAxisTick(CPoint2D(pos3, pos),
                (tickInside_ ? CDIRECTION_TYPE_UP : CDIRECTION_TYPE_DOWN), false);
            else
              drawAxisTick(CPoint2D(pos, pos3),
                (tickInside_ ? CDIRECTION_TYPE_RIGHT : CDIRECTION_TYPE_LEFT), false);
          }
        }
      }
    }

    //------*/

    pos1 = pos2;
  }

  //------*/

  // Draw Last Major Tick

  if (pos1 >= getStart() && pos1 <= getEnd()) {
    // Draw Tick Mark

    if (drawTickMark()) {
      if (direction_ == CORIENTATION_HORIZONTAL)
        drawAxisTick(CPoint2D(pos1, pos),
          (tickInside_ ? CDIRECTION_TYPE_UP : CDIRECTION_TYPE_DOWN), true);
      else
        drawAxisTick(CPoint2D(pos, pos1),
          (tickInside_ ? CDIRECTION_TYPE_RIGHT : CDIRECTION_TYPE_LEFT), true);
    }

    //------*/

    // Create Tick Label (Override with application supplied string if required)

    str = getValueStr(getNumMajorTicks(), pos1);

    //------*/

    // Draw Tick Label

    if (drawTickLabel()) {
      if (direction_ == CORIENTATION_HORIZONTAL)
        drawTickLabel(CPoint2D(pos1, pos), str);
      else
        drawTickLabel(CPoint2D(pos, pos1), str);
    }
  }

  //------*/

  // Draw Axis Label

  if (drawLabel()) {
    double x1 = getStart(), y1 = pos;
    double x2 = getEnd  (), y2 = pos;

    plot_->mapLogPoint(&x1, &y1);
    plot_->mapLogPoint(&x2, &y2);

    double mid = (x1 + x2)/2;

    plot_->unmapLogPoint(&mid, &y1);

    const std::string &astr = getLabel();

    if (direction_ == CORIENTATION_HORIZONTAL)
      drawAxisLabel(CPoint2D(mid, pos), astr, maxH);
    else
      drawAxisLabel(CPoint2D(pos, mid), astr, maxW);
  }
}

void
CGnuPlotAxis::
drawAxisTick(const CPoint2D &pos, CDirectionType type, bool large)
{
  double psize = 6;

  if (! large)
    psize = 3;

  if (type == CDIRECTION_TYPE_LEFT || type == CDIRECTION_TYPE_RIGHT) {
    double dx = (type == CDIRECTION_TYPE_LEFT ? -1 : 1);

    double x1 = pos.x + dx*plot_->pixelWidthToWindowWidth(psize);

    drawLine(pos, CPoint2D(x1, pos.y));
  }
  else {
    double dy = (type == CDIRECTION_TYPE_DOWN  ? -1 : 1);

    double y1 = pos.y + dy*plot_->pixelHeightToWindowHeight(psize);

    drawLine(pos, CPoint2D(pos.x, y1));
  }
}

void
CGnuPlotAxis::
drawTickLabel(const CPoint2D &p, const std::string &str)
{
  // bool rotatedText = (direction_ == CORIENTATION_VERTICAL);
  bool rotatedText = false;

  if (direction_ == CORIENTATION_HORIZONTAL) {
    if (isLabelInside())
      drawHAlignedText(p, CHALIGN_TYPE_CENTER, 0, CVALIGN_TYPE_BOTTOM, -8, str);
    else
      drawHAlignedText(p, CHALIGN_TYPE_CENTER, 0, CVALIGN_TYPE_TOP   ,  8, str);
  }
  else {
    if (rotatedText) {
      if (isLabelInside())
        drawVAlignedText(p, CHALIGN_TYPE_LEFT , 0, CVALIGN_TYPE_CENTER,  8, str);
      else
        drawVAlignedText(p, CHALIGN_TYPE_RIGHT, 0, CVALIGN_TYPE_CENTER, -8, str);
    }
    else
      if (isLabelInside())
        drawHAlignedText(p, CHALIGN_TYPE_LEFT ,  8, CVALIGN_TYPE_CENTER, 0, str);
      else
        drawHAlignedText(p, CHALIGN_TYPE_RIGHT, -8, CVALIGN_TYPE_CENTER, 0, str);
  }
}

void
CGnuPlotAxis::
drawAxisLabel(const CPoint2D &p, const std::string &str, int maxSize)
{
  if (direction_ == CORIENTATION_HORIZONTAL) {
    if (isLabelInside())
      drawHAlignedText(p, CHALIGN_TYPE_CENTER, 0, CVALIGN_TYPE_BOTTOM, -(maxSize + 12), str);
    else
      drawHAlignedText(p, CHALIGN_TYPE_CENTER, 0, CVALIGN_TYPE_TOP   ,   maxSize + 12 , str);
  }
  else {
    if (isLabelInside())
      drawVAlignedText(p, CHALIGN_TYPE_LEFT ,    maxSize + 12 , CVALIGN_TYPE_CENTER, 0, str);
    else
      drawVAlignedText(p, CHALIGN_TYPE_RIGHT , -(maxSize + 12), CVALIGN_TYPE_CENTER, 0, str);
  }
}

void
CGnuPlotAxis::
drawGrid(double start, double end)
{
  static double gridDashes[] = {1, 3};
  static uint   numGridDashes = 2;

  CGnuPlotRenderer *renderer = plot_->renderer();

  renderer->setLineDash(CLineDash(gridDashes, numGridDashes));

  //------*/

  if (direction_ == CORIENTATION_HORIZONTAL) { // x-axis
    double xincrement = getMajorIncrement();

    for (uint i = 0; i <= getNumMajorTicks(); i++) {
      double x = i*xincrement + getStart();

      if (x >= getStart() && x <= getEnd())
        drawLine(CPoint2D(x, start), CPoint2D(x, end));
    }
  }
  else {
    double yincrement = getMajorIncrement();

    for (uint i = 0; i <= getNumMajorTicks(); i++) {
      double y = i*yincrement + getStart();

      if (y >= getStart() && y <= getEnd())
        drawLine(CPoint2D(start, y), CPoint2D(end, y));
    }
  }

  //------*/

  renderer->setLineDash(CLineDash());
}

void
CGnuPlotAxis::
drawLine(const CPoint2D &p1, const CPoint2D &p2)
{
  CGnuPlotRenderer *renderer = plot_->renderer();

  double x1 = p1.x, y1 = p1.y;
  double x2 = p2.x, y2 = p2.y;

  plot_->mapLogPoint(&x1, &y1);
  plot_->mapLogPoint(&x2, &y2);

  renderer->drawLine(CPoint2D(x1, y1), CPoint2D(x2, y2));
}

void
CGnuPlotAxis::
drawHAlignedText(const CPoint2D &pos, CHAlignType halign, double xOffset,
                 CVAlignType valign, double yOffset, const std::string &str)
{
  double x = pos.x, y = pos.y;

  plot_->mapLogPoint(&x, &y);

  plot_->drawHAlignedText(CPoint2D(x, y), halign, xOffset, valign, yOffset, str);
}

void
CGnuPlotAxis::
drawVAlignedText(const CPoint2D &pos, CHAlignType halign, double xOffset,
                 CVAlignType valign, double yOffset, const std::string &str)
{
  double x = pos.x, y = pos.y;

  plot_->mapLogPoint(&x, &y);

  plot_->drawVAlignedText(CPoint2D(x, y), halign, xOffset, valign, yOffset, str);
}
