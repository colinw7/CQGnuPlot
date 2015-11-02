#include <CGnuPlotEndBar.h>
#include <CGnuPlotPlot.h>
#include <CGnuPlotRenderer.h>
#include <CGnuPlotBoxBarObject.h>

CGnuPlotEndBar::
CGnuPlotEndBar(CGnuPlotPlot *plot) :
 plot_(plot)
{
  stroke_ = plot->createStroke();
}

void
CGnuPlotEndBar::
draw(CGnuPlotRenderer *renderer) const
{
  bool highlighted = (bar_->isHighlighted() || bar_->isSelected());

  CGnuPlotStrokeP stroke = stroke_;

  if (highlighted) {
    stroke = stroke_->dup();

    stroke->setEnabled(true);
    stroke->setColor(CRGBA(1,0,0));
    stroke->setWidth(2);
  }

  renderer->strokeClipLine(start(), end(), *stroke);

  //double dx = fabs(start().x - end().x);
  //double dy = fabs(start().y - end().y);

  CPoint2D dp;

  if (bar_->isVertical())
    dp = CPoint2D(endWidth()/2, 0);
  else
    dp = CPoint2D(0, endWidth()/2);

  if (isStartLine())
    renderer->strokeClipLine(start() - dp, start() + dp, *stroke);

  if (isEndLine())
    renderer->strokeClipLine(end  () - dp, end  () + dp, *stroke);
}
