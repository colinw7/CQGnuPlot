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
  uint   num_ticks;
  double inc_factor;
};

static CGnuPlotAxisIncrementTest
axes_increment_tests[MAX_GAP_TESTS] = {
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
 plot_         (plot),
 direction_    (direction),
 start_        (start),
 end_          (end),
 start1_       (start),
 end1_         (end),
 logarithmic_  (0),
 numTicks1_    (1),
 numTicks2_    (0),
 tickIncrement_(0),
 tickSpaces_   (),
 tickInside_   (false),
 label_        (""),
 displayed_    (true),
 drawLine_     (true),
 drawTicMark_  (true),
 drawTicLabel_ (true),
 drawLabel_    (true)
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
setTickIncrement(uint tick_increment)
{
  tickIncrement_ = tick_increment;

  calc();
}

void
CGnuPlotAxis::
setTickSpaces(double *tick_spaces, uint num_tick_spaces)
{
  tickSpaces_.resize(num_tick_spaces);

  memcpy(&tickSpaces_[0], tick_spaces, num_tick_spaces*sizeof(double));
}

void
CGnuPlotAxis::
setLabel(const std::string &str)
{
  label_ = str;
}

bool
CGnuPlotAxis::
calc()
{
  numTicks1_ = 1;
  numTicks2_ = 0;

  //------

  /* Ensure Axis Start and End are in the Correct Order */

  double min_axis = std::min(start_, end_);
  double max_axis = std::max(start_, end_);

  //------

  /* Calculate Length */

  double length = fabs(max_axis - min_axis);

  if (length == 0.0)
    return false;

  //------

  /* Calculate nearest Power of Ten to Length */

  int power = CMathGen::RoundDown(log10(length));

  //------

  /* Set Default Increment to 0.1 * Power of Ten */

  double increment = 0.1;

  if      (power < 0)
    for (int i = 0; i < -power; i++)
      increment /= 10.0;
  else if (power > 0)
    for (int i = 0; i <  power; i++)
      increment *= 10.0;

  //------

  /* Calculate other test Increments */

  for (int i = 0; i < MAX_GAP_TESTS; i++)
    axes_increment_tests[i].inc_factor = increment*axes_increment_tests[i].factor;

  //------

  /* Set Default Start/End to Force Update */

  start1_ = 0.0;
  end1_   = 0.0;

  //------

  /* Test each Increment in turn */

  uint num_gaps, num_gap_ticks;

  for (int i = 0; i < MAX_GAP_TESTS; i++) {
    if (tickIncrement_ > 0) {
      int inc_factor1 = (int) axes_increment_tests[i].inc_factor;

      if (((double) inc_factor1) != axes_increment_tests[i].inc_factor)
        continue;

      if (inc_factor1 % tickIncrement_ != 0)
         continue;
    }

    testAxisGaps(min_axis, max_axis,
                 axes_increment_tests[i].inc_factor,
                 axes_increment_tests[i].num_ticks,
                 &start1_, &end1_, &increment,
                 &num_gaps, &num_gap_ticks);
  }

  //------

  /* Set the Gap Positions */

  numTicks1_ = CMathGen::RoundDown((end1_ - start1_)/increment + 0.5);
  numTicks2_ = num_gap_ticks;

  return true;
}

bool
CGnuPlotAxis::
testAxisGaps(double start, double end, double test_increment, uint test_num_gap_ticks,
             double *start1, double *end1, double *increment, uint *num_gaps, uint *num_gap_ticks)
{
  /* Calculate New Start and End implied by the Test Increment */

  double new_start = CMathGen::RoundDown(start/test_increment)*test_increment;
  double new_end   = CMathGen::RoundUp  (end  /test_increment)*test_increment;

  while (new_start > start)
    new_start -= test_increment;

  while (new_end < end)
    new_end += test_increment;

  uint test_num_gaps = CMathGen::RoundUp((new_end - new_start)/test_increment);

  //------

  /* If nothing set yet just update values and return */

  if (*start1 == 0.0 && *end1 == 0.0) {
    *start1 = new_start;
    *end1   = new_end;

    *increment     = test_increment;
    *num_gaps      = test_num_gaps;
    *num_gap_ticks = test_num_gap_ticks;

    return true;
  }

  //------

  /* If the current number of gaps is not within the acceptable range
     and the new number of gaps is within the acceptable range then
     update current */

  if ((    *num_gaps <  MIN_GOOD_TICKS ||     *num_gaps >  MAX_GOOD_TICKS) &&
      (test_num_gaps >= MIN_GOOD_TICKS && test_num_gaps <= MAX_GOOD_TICKS)) {
    *start1 = new_start;
    *end1   = new_end;

    *increment     = test_increment;
    *num_gaps      = test_num_gaps;
    *num_gap_ticks = test_num_gap_ticks;

    return true;
  }

  //------

  /* If the current number of gaps is not within the acceptable range
     and the new number of gaps is not within the acceptable range then
     consider it for update of current if better fit */

  if ((    *num_gaps < MIN_GOOD_TICKS ||     *num_gaps > MAX_GOOD_TICKS) &&
      (test_num_gaps < MIN_GOOD_TICKS || test_num_gaps > MAX_GOOD_TICKS)) {
    /* Calculate how close fit is to required range */

    double delta1 = fabs(new_start - start) + fabs(new_end - end);

    //------

    /* If better fit than current fit or equally good fit and
       number of gaps is nearer to optimum (OPTIMUM_TICKS) then
       update current */

    double delta2 = fabs(*start1 - start) + fabs(*end1 - end);

    if (((fabs(delta1 - delta2) < 1E-6) &&
         (abs(test_num_gaps - OPTIMUM_TICKS) < abs(*num_gaps - OPTIMUM_TICKS))) ||
        delta1 < delta2) {
      *start1 = new_start;
      *end1   = new_end;

      *increment     = test_increment;
      *num_gaps      = test_num_gaps;
      *num_gap_ticks = test_num_gap_ticks;

      return true;
    }
  }

  //------

  /* If the current number of gaps is within the acceptable range
     and the new number of gaps is within the acceptable range then
     consider it for update of current if better fit */

  if ((    *num_gaps >= MIN_GOOD_TICKS &&     *num_gaps <= MAX_GOOD_TICKS) &&
      (test_num_gaps >= MIN_GOOD_TICKS && test_num_gaps <= MAX_GOOD_TICKS)) {
    /* Calculate how close fit is to required range */

    double delta1 = fabs(new_start - start) + fabs(new_end - end);

    //------

    /* If better fit than current fit or equally good fit and
       number of gaps is nearer to optimum (OPTIMUM_TICKS) then
       update current */

    double delta2 = fabs(*start1 - start) + fabs(*end1 - end);

    if (((fabs(delta1 - delta2) < 1E-6) &&
         (abs(test_num_gaps - OPTIMUM_TICKS) < abs(*num_gaps - OPTIMUM_TICKS))) ||
        delta1 < delta2) {
      *start1 = new_start;
      *end1   = new_end;

      *increment     = test_increment;
      *num_gaps      = test_num_gaps;
      *num_gap_ticks = test_num_gap_ticks;

      return true;
    }
  }

  return false;
}

double
CGnuPlotAxis::
getMajorIncrement() const
{
  return (end1_ - start1_)/numTicks1_;
}

double
CGnuPlotAxis::
getMinorIncrement() const
{
  return (end1_ - start1_)/(numTicks1_*numTicks2_);
}

std::string
CGnuPlotAxis::
getValueStr(double pos) const
{
  return CStrUtil::toString(pos);
}

void
CGnuPlotAxis::
drawAxis(double pos)
{
  std::string str;

  if (! isDisplayed())
    return;

  // bool rotatedText = (direction_ == CORIENTATION_VERTICAL);
  bool rotatedText = false;

  double maxW = 0;
  double maxH = font_->getCharHeight();

  /*------*/

  /* Draw Axis Line */

  if (drawLine()) {
    if (direction_ == CORIENTATION_HORIZONTAL)
      drawLine(CPoint2D(getStart(), pos), CPoint2D(getEnd(), pos));
    else
      drawLine(CPoint2D(pos, getStart()), CPoint2D(pos, getEnd()));
  }

  /*------*/

  /* Draw Ticks Marks and Labels */

  double increment = getMajorIncrement();

  double pos1 = getStart();
  double pos2 = pos1;

  for (uint i = 0; i < getNumMajorTicks(); i++) {
    /* Calculate Next Tick Point */

    pos2 = (i + 1)*increment + getStart();

    /*------*/

    /* Draw Major Tick */

    if (pos1 >= getStart() && pos1 <= getEnd()) {
      /* Draw Tick Mark */

      if (drawTicMark()) {
        if (direction_ == CORIENTATION_HORIZONTAL)
          drawAxisTick(CPoint2D(pos1, pos),
           (tickInside_ ? CDIRECTION_TYPE_UP : CDIRECTION_TYPE_DOWN), true);
        else
          drawAxisTick(CPoint2D(pos, pos1),
           (tickInside_ ? CDIRECTION_TYPE_RIGHT : CDIRECTION_TYPE_LEFT), true);
      }

      /*------*/

      /* Create Tick Label (Override with application supplied string if required) */

      str = getValueStr(pos1);

      maxW = std::max(maxW, font_->getStringWidth(str));

      /*------*/

      /* Draw Tick Label */

      if (drawTicLabel()) {
        if (direction_ == CORIENTATION_HORIZONTAL)
          drawHAlignedText(CPoint2D(pos1, pos ), CHALIGN_TYPE_CENTER, 0,
                           CVALIGN_TYPE_TOP, 8, str);
        else {
          if (rotatedText)
            drawVAlignedText(CPoint2D(pos, pos1), CHALIGN_TYPE_RIGHT,  0,
                             CVALIGN_TYPE_CENTER, -8, str);
          else
            drawHAlignedText(CPoint2D(pos, pos1), CHALIGN_TYPE_RIGHT, -8,
                             CVALIGN_TYPE_CENTER,  0, str);
        }
      }
    }

    /*------*/

    /* Draw Minor Ticks (use user defined distribution if defined) */

    if (getTickSpaces() == 0) {
      for (uint j = 1; j < getNumMinorTicks(); j++) {
        double pos3 = j*(pos2 - pos1)/getNumMinorTicks() + pos1;

        if (drawTicMark()) {
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

        if (drawTicMark()) {
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

    /*------*/

    pos1 = pos2;
  }

  /*------*/

  /* Draw Last Major Tick */

  if (pos1 >= getStart() && pos1 <= getEnd()) {
    /* Draw Tick Mark */

    if (drawTicMark()) {
      if (direction_ == CORIENTATION_HORIZONTAL)
        drawAxisTick(CPoint2D(pos1, pos),
          (tickInside_ ? CDIRECTION_TYPE_UP : CDIRECTION_TYPE_DOWN), true);
      else
        drawAxisTick(CPoint2D(pos, pos1),
          (tickInside_ ? CDIRECTION_TYPE_RIGHT : CDIRECTION_TYPE_LEFT), true);
    }

    /*------*/

    /* Create Tick Label (Override with application supplied string if required) */

    str = getValueStr(pos1);

    /*------*/

    /* Draw Tick Label */

    if (drawTicLabel()) {
      if (direction_ == CORIENTATION_HORIZONTAL)
        drawHAlignedText(CPoint2D(pos1, pos ), CHALIGN_TYPE_CENTER, 0,
                         CVALIGN_TYPE_TOP, 8, str);
      else {
        if (rotatedText)
          drawVAlignedText(CPoint2D(pos, pos1), CHALIGN_TYPE_RIGHT,  0,
                           CVALIGN_TYPE_CENTER, -8, str);
        else
          drawHAlignedText(CPoint2D(pos, pos1), CHALIGN_TYPE_RIGHT, -8,
                           CVALIGN_TYPE_CENTER,  0, str);
      }
    }
  }

  /*------*/

  /* Draw Axis Label */

  if (drawLabel()) {
    double x1 = getStart(), y1 = pos;
    double x2 = getEnd  (), y2 = pos;

    plot_->mapLogPoint(&x1, &y1);
    plot_->mapLogPoint(&x2, &y2);

    double mid = (x1 + x2)/2;

    plot_->unmapLogPoint(&mid, &y1);

    const std::string &astr = getLabel();

    if (direction_ == CORIENTATION_HORIZONTAL)
      drawHAlignedText(CPoint2D(mid, pos), CHALIGN_TYPE_CENTER, 0,
                       CVALIGN_TYPE_TOP, maxH + 12, astr);
    else
      drawVAlignedText(CPoint2D(pos, mid), CHALIGN_TYPE_RIGHT , -maxW - 12,
                       CVALIGN_TYPE_CENTER, 0, astr);
  }
}

void
CGnuPlotAxis::
drawAxisTick(const CPoint2D &pos, CDirectionType type, bool large)
{
  int psize = 6;

  if (! large)
    psize = 3;

  if (type == CDIRECTION_TYPE_LEFT || type == CDIRECTION_TYPE_RIGHT) {
    int dx = (type == CDIRECTION_TYPE_LEFT ? -1 : 1);

    double x1 = pos.x + dx*plot_->pixelWidthToWindowWidth(psize);

    drawLine(pos, CPoint2D(x1, pos.y));
  }
  else {
    int dy = (type == CDIRECTION_TYPE_DOWN  ? -1 : 1);

    double y1 = pos.y + dy*plot_->pixelHeightToWindowHeight(psize);

    drawLine(pos, CPoint2D(pos.x, y1));
  }
}

void
CGnuPlotAxis::
drawGrid(double start, double end)
{
  static double grid_dashes[]   = {1, 3};
  static uint   num_grid_dashes = 2;

  CGnuPlotRenderer *renderer = plot_->renderer();

  renderer->setLineDash(CLineDash(grid_dashes, num_grid_dashes));

  /*------*/

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

  /*------*/

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
drawHAlignedText(const CPoint2D &pos, CHAlignType halign, int x_offset,
                 CVAlignType valign, int y_offset, const std::string &str)
{
  double x = pos.x, y = pos.y;

  plot_->mapLogPoint(&x, &y);

  plot_->drawHAlignedText(CPoint2D(x, y), halign, x_offset, valign, y_offset, str);
}

void
CGnuPlotAxis::
drawVAlignedText(const CPoint2D &pos, CHAlignType halign, int x_offset,
                 CVAlignType valign, int y_offset, const std::string &str)
{
  double x = pos.x, y = pos.y;

  plot_->mapLogPoint(&x, &y);

  plot_->drawVAlignedText(CPoint2D(x, y), halign, x_offset, valign, y_offset, str);
}
