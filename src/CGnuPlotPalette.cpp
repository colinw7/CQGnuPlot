#include <CGnuPlotPalette.h>
#include <CGnuPlotGroup.h>
#include <CGnuPlot.h>

CGnuPlotPalette::
CGnuPlotPalette(CGnuPlotGroup *group)
{
  pal_ = new CGradientPalette(group ? group->app()->expr() : nullptr);
}

CGnuPlotPalette::
~CGnuPlotPalette()
{
  delete pal_;
}

void
CGnuPlotPalette::
setGradientPalette(const CGradientPalette &pal)
{
  *pal_ = pal;
}

CColor
CGnuPlotPalette::
getColor(double x) const
{
  return pal_->getColor(x);
}

bool
CGnuPlotPalette::
readFile(CGnuPlot *plot, const std::string &filename)
{
  if (filename == "-") {
    std::vector<std::string> lines;

    plot->readFileLines(lines);

    return pal_->readFileLines(lines);
  }
  else {
    return pal_->readFile(filename);
  }
}
