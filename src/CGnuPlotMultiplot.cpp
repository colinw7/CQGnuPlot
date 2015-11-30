#include <CGnuPlotMultiplot.h>

CGnuPlotMultiplot::
CGnuPlotMultiplot(CGnuPlot *plot) :
 plot_(plot)
{
}

CGnuPlotMultiplot::
~CGnuPlotMultiplot()
{
}

CIPoint2D
CGnuPlotMultiplot::
pos(int n) const
{
  int r = 0, c = 0;

  if (cols_ > 0 && rows_ > 0) {
    if (rowsFirst_) {
      r = n % rows_;
      c = n / rows_;
    }
    else {
      c = n % cols_;
      r = n / cols_;
    }

    if (downward_)
      r = rows_ - 1 - r;
  }

  return CIPoint2D(c, r);
}
