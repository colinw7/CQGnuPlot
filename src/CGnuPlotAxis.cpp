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
CGnuPlotAxis(CGnuPlotGroup *group, const std::string &id, Direction direction,
             double start, double end) :
 group_    (group),
 id_       (id),
 direction_(direction),
 start_    (start),
 end_      (end),
 start1_   (start),
 end1_     (end)
{
  // set direction vector
  if      (direction == Direction::X) { v_ = CPoint3D(1, 0, 0); iv_ = CPoint3D(0, 1, 0); }
  else if (direction == Direction::Y) { v_ = CPoint3D(0, 1, 0); iv_ = CPoint3D(1, 0, 0); }
  else if (direction == Direction::Z) { v_ = CPoint3D(0, 0, 1); iv_ = CPoint3D(1, 0, 0); }
  else                                assert(false);

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
  reverse_ = (start_ > end_);

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

  int power = CMathGen::RoundDown(log10(length));

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

    for (int i = 0; i < MAX_GAP_TESTS; ++i)
      axesIncrementTests[i].incFactor = increment*axesIncrementTests[i].factor;

    //------

    // Set Default Start/End to Force Update

    start1 = 0.0;
    end1   = 0.0;

    //------

    // Test each Increment in turn

    int numGaps, numGapTicks;

    for (int i = 0; i < MAX_GAP_TESTS; ++i) {
      if (tickIncrement > 0) {
        int incFactor1 = (int) axesIncrementTests[i].incFactor;

        if (((double) incFactor1) != axesIncrementTests[i].incFactor)
          continue;

        if (incFactor1 % tickIncrement != 0)
          continue;
      }

      testAxisGaps(minAxis, maxAxis,
                   axesIncrementTests[i].incFactor,
                   axesIncrementTests[i].numTicks,
                   &start1, &end1, &increment,
                   &numGaps, &numGapTicks);
    }

    //------

    // Set the Gap Positions

    numTicks1 = CMathGen::RoundDown((end1 - start1)/increment + 0.5);
    numTicks2 = numGapTicks;
  }
  else {
    start1    = start;
    end1      = end;
    numTicks1 = CMathGen::RoundDown((end1 - start1)/majorIncrement + 0.5);
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

  double newStart = CMathGen::RoundDown(start/testIncrement)*testIncrement;
  double newEnd   = CMathGen::RoundUp  (end  /testIncrement)*testIncrement;

  while (newStart > start)
    newStart -= testIncrement;

  while (newEnd < end)
    newEnd += testIncrement;

  int testNumGaps = CMathGen::RoundUp((newEnd - newStart)/testIncrement);

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
hasTicLabels() const
{
  return group_->hasTicLabels(id_);
}

const CGnuPlotAxisData::RTicLabels &
CGnuPlotAxis::
getTicLabels() const
{
  return group_->ticLabels(id_);
}

std::string
CGnuPlotAxis::
getValueStr(int i, double pos) const
{
  return group_->getAxisValueStr(id_, i, pos);
}

void
CGnuPlotAxis::
drawAxis(CGnuPlotRenderer *renderer, bool first)
{
  renderer_ = renderer;

  if (! isDisplayed())
    return;

  //---

  // calc bounding box (TODO: 3D box)
  CPoint3D p1 = valueToPoint(getStart(), first);
  CPoint3D p2 = valueToPoint(getEnd  (), first);

  double px = renderer_->pixelWidthToWindowWidth(2);
  double py = renderer_->pixelHeightToWindowHeight(2);

  if      (direction_ == Direction::X)
    bbox_ = CBBox2D(renderer_->transform(p1) - CPoint2D(0, py/2),
                    renderer_->transform(p2) + CPoint2D(0, py/2));
  else if (direction_ == Direction::Y)
    bbox_ = CBBox2D(renderer_->transform(p1) - CPoint2D(px/2, 0),
                    renderer_->transform(p2) + CPoint2D(px/2, 0));
  else
    bbox_ = CBBox2D(renderer_->transform(p1) - CPoint2D(0, py/2),
                    renderer_->transform(p2) + CPoint2D(0, py/2));

  //---

  CFontPtr font = renderer_->getFont();

  CRGBA c(0,0,0);

  double maxW = 0;
  double maxH = font->getCharHeight();

  std::string str;

  //---

  // Draw Axis Line

  if (isDrawLine())
    drawLine(p1, p2, c);

  //---

  // Draw Ticks Marks and Labels

  double increment = getMajorIncrement();

  double pos1 = getStart();
  double pos2 = pos1;

  for (int i = 0; i < getNumMajorTicks(); ++i) {
    // Calculate Next Tick Point

    pos2 = (i + 1)*increment + getStart();

    //------*/

    // Draw Major Tick

    if (pos1 >= getStart() && pos1 <= getEnd()) {
      // Draw Tick Mark

      if (isDrawTickMark(first))
        drawAxisTick(pos1, first, true);

      //------*/

      // Create Tick Label (Override with application supplied string if required)

      if (! hasTicLabels()) {
        str = getValueStr(i, pos1);

        maxW = std::max(maxW, font->getStringWidth(str));

        //------*/

        // Draw Tick Label

        if (isDrawTickLabel(first))
          drawTickLabel(pos1, str, first);
      }
    }

    //------*/

    // Draw Minor Ticks (use user defined distribution if defined)

    if (getTickSpaces() == 0) {
      double dt = (pos2 - pos1)/getNumMinorTicks();

      if (checkMinorTickSize(dt)) {
        for (int j = 1; j < getNumMinorTicks(); j++) {
          double pos3 = j*dt + pos1;

          if (isDrawTickMark(first) && isDrawMinorTickMark()) {
            if (pos3 >= getStart() && pos3 <= getEnd())
              drawAxisTick(pos3, first, false);
          }
        }
      }
    }
    else {
      for (int j = 1; j <= getNumTickSpaces() - 2; j++) {
        double pos3 = getTickSpace(j)*(pos2 - pos1) + pos1;

        if (isDrawTickMark(first) && isDrawMinorTickMark()) {
          if (pos3 >= getStart() && pos3 <= getEnd())
            drawAxisTick(pos3, first, false);
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

    if (isDrawTickMark(first))
      drawAxisTick(pos1, first, true);

    //------*/

    // Create Tick Label (Override with application supplied string if required)

    if (! hasTicLabels()) {
      str = getValueStr(getNumMajorTicks(), pos1);

      maxW = std::max(maxW, font->getStringWidth(str));

      //------*/

      // Draw Tick Label

      if (isDrawTickLabel(first))
        drawTickLabel(pos1, str, first);
    }
  }

  //------*/

  // Draw Custom Tic Labels

  if (hasTicLabels()) {
    const CGnuPlotAxisData::RTicLabels &ticLabels = getTicLabels();

    for (const auto &label : ticLabels) {
      double      r = label.first;
      std::string s = label.second;

      maxW = std::max(maxW, font->getStringWidth(s));

      drawTickLabel(r, s, first);
    }
  }

  //------*/

  // Draw Axis Label

  if (isDrawLabel(first)) {
    CPoint3D p1 = valueToPoint(getStart(), first);
    CPoint3D p2 = valueToPoint(getEnd  (), first);

    group_->mapLogPoint(p1);
    group_->mapLogPoint(p2);

    CPoint3D pm = (p1 + p2)/2;

    group_->unmapLogPoint(pm);

    const std::string &astr = getLabel();

    if      (direction_ == Direction::X)
      drawAxisLabel(pm.x, astr, maxH, first);
    else if (direction_ == Direction::Y)
      drawAxisLabel(pm.y, astr, maxH, first);
    else
      drawAxisLabel(pm.z, astr, maxH, first);
  }
}

void
CGnuPlotAxis::
drawAxisTick(double pos, bool first, bool large)
{
  CPoint3D p;

  if (reverse_)
    p = valueToPoint(end_ - (pos - start_), first);
  else
    p = valueToPoint(pos, first);

  if (clip_ && ! renderer_->clip().inside(CPoint2D(p.x, p.y)))
    return;

  double psize = 6;

  if (! large)
    psize = 3;

  double xsize = renderer_->pixelWidthToWindowWidth  (psize);
  double ysize = renderer_->pixelHeightToWindowHeight(psize);

  CPoint3D p1;

  if (isTickInside(first))
    p1 = p + ivalueToPoint(xsize, ysize);
  else
    p1 = p - ivalueToPoint(xsize, ysize);

  CRGBA c(0,0,0);

  drawLine(p, p1, c);
}

void
CGnuPlotAxis::
drawTickLabel(double pos, const std::string &str, bool first)
{
  CPoint3D p;

  if (reverse_)
    p = valueToPoint(end_ - (pos - start_), first);
  else
    p = valueToPoint(pos, first);

  if (clip_ && ! renderer_->clip().inside(CPoint2D(p.x, p.y)))
    return;

  double xsize = renderer_->pixelWidthToWindowWidth  (8);
  double ysize = renderer_->pixelHeightToWindowHeight(8);

  // bool rotatedText = (direction_ == Direction::Y);
  bool rotatedText = false;

  if      (direction_ == Direction::X) {
    CPoint3D p1 = perpPoint(p, (isLabelInside(first) ? ysize : -ysize));

    if (isLabelInside(first))
      drawHAlignedText(p1, CHALIGN_TYPE_CENTER, 0, CVALIGN_TYPE_BOTTOM, 0, str);
    else
      drawHAlignedText(p1, CHALIGN_TYPE_CENTER, 0, CVALIGN_TYPE_TOP   , 0, str);
  }
  else if (direction_ == Direction::Y) {
    if (rotatedText) {
      CPoint3D p1 = perpPoint(p, (isLabelInside(first) ? ysize : -ysize));

      if (isLabelInside(first))
        drawVAlignedText(p1, CHALIGN_TYPE_LEFT , 0, CVALIGN_TYPE_CENTER, 0, str);
      else
        drawVAlignedText(p1, CHALIGN_TYPE_RIGHT, 0, CVALIGN_TYPE_CENTER, 0, str);
    }
    else {
      CPoint3D p1 = perpPoint(p, (isLabelInside(first) ? xsize : -xsize));

      if (isLabelInside(first))
        drawHAlignedText(p1, CHALIGN_TYPE_LEFT , 0, CVALIGN_TYPE_CENTER, 0, str);
      else
        drawHAlignedText(p1, CHALIGN_TYPE_RIGHT, 0, CVALIGN_TYPE_CENTER, 0, str);
    }
  }
  else {
    CPoint3D p1 = perpPoint(p, (isLabelInside(first) ? xsize : -xsize));

    if (isLabelInside(first))
      drawHAlignedText(p1, CHALIGN_TYPE_LEFT , 0, CVALIGN_TYPE_CENTER, 0, str);
    else
      drawHAlignedText(p1, CHALIGN_TYPE_RIGHT, 0, CVALIGN_TYPE_CENTER, 0, str);
  }
}

void
CGnuPlotAxis::
drawAxisLabel(double pos, const std::string &str, int maxSize, bool first)
{
  CPoint3D p;

  if (reverse_)
    p = valueToPoint(end_ - (pos - start_), first);
  else
    p = valueToPoint(pos, first);

  double xsize = renderer_->pixelWidthToWindowWidth  (maxSize + 12);
  double ysize = renderer_->pixelHeightToWindowHeight(maxSize + 12);

  if      (direction_ == Direction::X) {
    CPoint3D p1 = perpPoint(p, (isLabelInside(first) ? ysize : -ysize));

    if (isLabelInside(first))
      drawHAlignedText(p1, CHALIGN_TYPE_CENTER, 0, CVALIGN_TYPE_BOTTOM, 0, str);
    else
      drawHAlignedText(p1, CHALIGN_TYPE_CENTER, 0, CVALIGN_TYPE_TOP   , 0, str);
  }
  else if (direction_ == Direction::Y) {
    CPoint3D p1 = perpPoint(p, (isLabelInside(first) ? xsize : -xsize));

    if (group_->is3D()) {
      if (isLabelInside(first))
        drawHAlignedText(p1, CHALIGN_TYPE_CENTER, 0, CVALIGN_TYPE_BOTTOM, 0, str);
      else
        drawHAlignedText(p1, CHALIGN_TYPE_CENTER, 0, CVALIGN_TYPE_TOP   , 0, str);
    }
    else {
      if (isLabelInside(first))
        drawVAlignedText(p1, CHALIGN_TYPE_LEFT ,  0, CVALIGN_TYPE_CENTER, 0, str);
      else
        drawVAlignedText(p1, CHALIGN_TYPE_RIGHT , 0, CVALIGN_TYPE_CENTER, 0, str);
    }
  }
  else {
    CPoint3D p1 = perpPoint(p, (isLabelInside(first) ? xsize : -xsize));

    if (isLabelInside(first))
      drawVAlignedText(p1, CHALIGN_TYPE_LEFT ,  0, CVALIGN_TYPE_CENTER, 0, str);
    else
      drawVAlignedText(p1, CHALIGN_TYPE_RIGHT , 0, CVALIGN_TYPE_CENTER, 0, str);
  }
}

void
CGnuPlotAxis::
drawGrid(CGnuPlotRenderer *renderer)
{
  static double gridDashes[] = {1, 3};
  static int    numGridDashes = 2;

  renderer_ = renderer;

  //------*/

  lineDash_ = CLineDash(gridDashes, numGridDashes);

  CRGBA c(0,0,0);

  //------*/

  if (gridMajor_ || gridMinor_) {
    double mi = getMajorIncrement();

    for (int i = 0; i <= getNumMajorTicks(); ++i) {
      double v = i*mi + getStart();

      if (gridMajor_) {
        if (v >= getStart() && v <= getEnd()) {
          CPoint3D p1 = valueToPoint(v, false);
          CPoint3D p2 = valueToPoint(v, true );

          drawClipLine(p1, p2, c);
        }
      }

      if (gridMinor_) {
        double mi1 = getMinorIncrement();

        if (checkMinorTickSize(mi1)) {
          for (int j = 0; j < getNumMinorTicks(); ++j) {
            double v1 = v + j*mi1;

            if (v1 >= getStart() && v1 <= getEnd()) {
              CPoint3D p11 = valueToPoint(v1, false);
              CPoint3D p21 = valueToPoint(v1, true );

              drawClipLine(p11, p21, c);
            }
          }
        }
      }
    }
  }

  lineDash_ = CLineDash();
}

void
CGnuPlotAxis::
drawRadialGrid(CGnuPlotRenderer *renderer)
{
  static double gridDashes[] = {1, 3};
  static int    numGridDashes = 2;

  renderer_ = renderer;

  //------*/

  lineDash_ = CLineDash(gridDashes, numGridDashes);

  //------*/

  CRGBA c(0,0,0);

  double da = M_PI/6;
  int    na = int(2*M_PI/da + 0.5);

  double r1 = 0.0;
  double r2 = getEnd();

  for (int i = 0; i < na; ++i) {
    double a = i*da;

    double x1 = r1*cos(a);
    double y1 = r1*sin(a);
    double x2 = r2*cos(a);
    double y2 = r2*sin(a);

    drawClipLine(CPoint3D(x1, y1, 0), CPoint3D(x2, y2, 0), c);
  }

  if (gridMajor_) {
    double xincrement = getMajorIncrement();

    for (int i = 0; i <= getNumMajorTicks(); ++i) {
      double x = i*xincrement + getStart();

      if (x >= getStart() && x <= getEnd())
        renderer->drawEllipse(CPoint3D(0, 0, 0), x, x, 0, c, 1, lineDash_);

      if (gridMinor_) {
        double xincrement1 = getMinorIncrement();

        if (checkMinorTickSize(xincrement1)) {
          for (int j = 0; j < getNumMinorTicks(); ++j) {
            double x1 = x + j*xincrement1;

            renderer->drawEllipse(CPoint3D(0, 0, 0), x1, x1, 0, c, 1, lineDash_);
          }
        }
      }
    }
  }

  lineDash_ = CLineDash();
}

bool
CGnuPlotAxis::
checkMinorTickSize(double d) const
{
  double dp = 0;

  if (direction_ == Direction::X || direction_ == Direction::Z)
    dp = renderer_->windowWidthToPixelWidth(d);
  else
    dp = renderer_->windowHeightToPixelHeight(d);

  return (dp > 2);
}

void
CGnuPlotAxis::
drawClipLine(const CPoint3D &p1, const CPoint3D &p2, const CRGBA &c)
{
  if (CGnuPlotUtil::realEq(p1.z, p2.z)) {
    CPoint2D p11(p1.x, p1.y);
    CPoint2D p21(p2.x, p2.y);

    if (! clip_ || renderer_->clipLine(p11, p21))
      drawLine(CPoint3D(p11.x, p11.y, p1.z), CPoint3D(p21.x, p21.y, p2.z), c);
  }
  else
    drawLine(p1, p2, c);
}

void
CGnuPlotAxis::
drawLine(const CPoint3D &p1, const CPoint3D &p2, const CRGBA &c)
{
  CPoint3D pm1 = group_->mapLogPoint(p1);
  CPoint3D pm2 = group_->mapLogPoint(p2);

  // TODO: custom width and color
  renderer_->drawLine(pm1, pm2, 1, c, lineDash_);
}

void
CGnuPlotAxis::
drawHAlignedText(const CPoint3D &pos, CHAlignType halign, double xOffset,
                 CVAlignType valign, double yOffset, const std::string &str)
{
  double x = pos.x, y = pos.y, z = pos.z;

  group_->mapLogPoint(&x, &y, &z);

  CPoint3D pos1(x, y, z);

  CRGBA c(0,0,0);

  if (enhanced_ && ! group_->is3D()) {
    //CFontPtr font = renderer_->getFont();

    CGnuPlotText text(str);

    CBBox2D bbox = text.calcBBox(renderer_);

    double dx = 0.0, dy = 0.0;

    if      (halign == CHALIGN_TYPE_LEFT  ) dx = 0;
    else if (halign == CHALIGN_TYPE_CENTER) dx = -bbox.getWidth()/2;
    else if (halign == CHALIGN_TYPE_RIGHT ) dx = -bbox.getWidth();

    if      (valign == CVALIGN_TYPE_BOTTOM) dy = bbox.getHeight();
    else if (valign == CVALIGN_TYPE_CENTER) dy = bbox.getHeight()/2;
    else if (valign == CVALIGN_TYPE_TOP   ) dy = 0;

    //dy -= renderer_->pixelHeightToWindowHeight(font->getCharAscent());

    double xo = renderer_->pixelWidthToWindowWidth  (xOffset);
    double yo = renderer_->pixelHeightToWindowHeight(yOffset);

    dx += xo;
    dy -= yo;

    CBBox2D bbox1 = bbox.moveBy(CPoint2D(pos1.x, pos1.y) + CPoint2D(dx, dy));

    bbox1.setYMax(pos1.y + dy);

    text.draw(renderer_, bbox1, CHALIGN_TYPE_CENTER, c);
  }
  else {
    renderer_->drawHAlignedText(pos1, halign, xOffset, valign, yOffset, str, c);
  }
}

void
CGnuPlotAxis::
drawVAlignedText(const CPoint3D &pos, CHAlignType halign, double xOffset,
                 CVAlignType valign, double yOffset, const std::string &str)
{
  double x = pos.x, y = pos.y, z = pos.y;

  CRGBA c(0,0,0);

  group_->mapLogPoint(&x, &y, &z);

  CPoint3D pos1(x, y, z);

  renderer_->drawVAlignedText(pos1, halign, xOffset, valign, yOffset, str, c);
}

CPoint3D
CGnuPlotAxis::
valueToPoint(double v, bool first) const
{
  return v*v_ + (first ? position_ : position1_);
}

CPoint3D
CGnuPlotAxis::
ivalueToPoint(double dx, double dy) const
{
  return CPoint3D(dx*iv_.x, dy*iv_.y, iv_.z);
}

CPoint3D
CGnuPlotAxis::
perpPoint(const CPoint3D &p, double d) const
{
  return p + d*iv_;
}
