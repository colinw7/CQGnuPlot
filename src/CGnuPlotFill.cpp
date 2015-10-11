#include <CGnuPlotFill.h>
#include <CGnuPlotLineStyle.h>
#include <CGnuPlotFillStyle.h>
#include <CGnuPlotPlot.h>
#include <CGnuPlotGroup.h>
#include <CGnuPlotWindow.h>

CGnuPlotFill::
CGnuPlotFill(CGnuPlotPlot *plot) :
 plot_(plot)
{
  if (plot)
    init(plot);
}

CGnuPlotFill *
CGnuPlotFill::
dup() const
{
  return new CGnuPlotFill(*this);
}

void
CGnuPlotFill::
init(CGnuPlotPlot *plot)
{
  const CGnuPlotLineStyle &lineStyle = plot->lineStyle();
  const CGnuPlotFillStyle &fillStyle = plot->fillStyle();

  CRGBA fc = (type_ == CGnuPlotTypes::FillType::PATTERN ? CRGBA(0,0,0) : CRGBA(1,1,1));

  bg_    = plot->group()->window()->backgroundColor();
  color_ = lineStyle.calcColor(plot->group(), fc);

  if      (fillStyle.isTransparent())
    color_.setAlpha(fillStyle.density());
  else if (fillStyle.density() < 1.0)
    color_ = fillStyle.density()*color_ + bg_*(1 - fillStyle.density());

  type_    = fillStyle.style();
  pattern_ = fillStyle.pattern();
}
