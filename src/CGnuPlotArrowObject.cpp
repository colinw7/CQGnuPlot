#include <CGnuPlotArrowObject.h>
#include <CGnuPlotWindow.h>
#include <CGnuPlotGroup.h>
#include <CGnuPlotPlot.h>
#include <CGnuPlotRenderer.h>
#include <CGnuPlotUtil.h>

CGnuPlotArrowObject::
CGnuPlotArrowObject(CGnuPlotPlot *plot) :
 CGnuPlotPlotObject(plot)
{
}

double
CGnuPlotArrowObject::
calcLineWidth() const
{
  return data_.calcLineWidth(plot_->group());
}

CRGBA
CGnuPlotArrowObject::
calcLineColor() const
{
  return data_.calcLineColor(plot_->group());
}

bool
CGnuPlotArrowObject::
inside(const CGnuPlotTypes::InsideData &data) const
{
  return data_.inside(data);
}

CGnuPlotTipData
CGnuPlotArrowObject::
tip() const
{
  return data_.tip();
}

void
CGnuPlotArrowObject::
draw(CGnuPlotRenderer *renderer) const
{
  bool highlighted = (isHighlighted() || isSelected());

  data_.draw(renderer, plot()->group(), highlighted);
}
