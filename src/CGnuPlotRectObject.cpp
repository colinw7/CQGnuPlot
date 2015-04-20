#include <CGnuPlotRectObject.h>
#include <CGnuPlotPlot.h>
#include <CGnuPlotRenderer.h>

CGnuPlotRectObject::
CGnuPlotRectObject(CGnuPlotPlot *plot) :
 CGnuPlotPlotObject(plot)
{
}

bool
CGnuPlotRectObject::
inside(const CPoint2D &p) const
{
  return rect_.inside(p);
}

void
CGnuPlotRectObject::
draw(CGnuPlotRenderer *renderer) const
{
  if (fillColor_.isValid())
    renderer->fillRect(rect_, fillColor_.getValue());

  if (lineColor_.isValid())
    renderer->drawRect(rect_, lineColor_.getValue(), lineWidth_);

  double tw = renderer->pixelWidthToWindowWidth  (renderer->getFont()->getStringWidth(text_));
  double th = renderer->pixelHeightToWindowHeight(renderer->getFont()->getCharAscent());

  renderer->drawText(rect_.getCenter() - CPoint2D(tw/2, th/2), text_, CRGBA(0,0,0));
}
