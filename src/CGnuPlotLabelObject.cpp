#include <CGnuPlotLabelObject.h>
#include <CGnuPlotWindow.h>
#include <CGnuPlotGroup.h>
#include <CGnuPlotPlot.h>
#include <CGnuPlotRenderer.h>
#include <CGnuPlotUtil.h>

CGnuPlotLabelObject::
CGnuPlotLabelObject(CGnuPlotPlot *plot) :
 CGnuPlotPlotObject(plot)
{
}

bool
CGnuPlotLabelObject::
inside(const CGnuPlotTypes::InsideData &data) const
{
  return data_.inside(data);
}

CGnuPlotTipData
CGnuPlotLabelObject::
tip() const
{
  return data_.tip();
}

void
CGnuPlotLabelObject::
draw(CGnuPlotRenderer *renderer) const
{
  if (! isDisplayed()) return;

  bool highlighted = (isHighlighted() || isSelected());

  data_.draw(renderer, plot()->group(), highlighted);
}
