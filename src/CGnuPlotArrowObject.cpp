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
  stroke_ = plot->createStroke();
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

  CGnuPlotStrokeP stroke = stroke_;

  if (highlighted) {
    stroke = stroke_->dup();

    stroke->setEnabled(true);
    stroke->setColor(CRGBA(1,0,0));
    stroke->setWidth(2);
  }

  data_.draw(renderer, *stroke);
}
