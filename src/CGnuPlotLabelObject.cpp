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
  data_ = new CGnuPlotLabelData(plot);
}

void
CGnuPlotLabelObject::
setData(CGnuPlotLabelData *data)
{
  delete data_;

  data_ = data;
}

bool
CGnuPlotLabelObject::
inside(const CGnuPlotMouseEvent &mouseEvent) const
{
  return data_->inside(mouseEvent);
}

CGnuPlotTipData
CGnuPlotLabelObject::
tip() const
{
  return data_->tip();
}

void
CGnuPlotLabelObject::
draw(CGnuPlotRenderer *renderer) const
{
  if (! isDisplayed()) return;

  bool highlighted = (isHighlighted() || isSelected());

  data_->draw(renderer, plot()->group(), highlighted);
}
