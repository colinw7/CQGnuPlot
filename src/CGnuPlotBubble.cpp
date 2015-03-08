#include <CGnuPlotBubble.h>
#include <CGnuPlotPlot.h>
#include <CGnuPlotRenderer.h>
#include <CGnuPlotUtil.h>

CGnuPlotBubble::
CGnuPlotBubble(CGnuPlotPlot *plot) :
 CGnuPlotPlotObject(plot)
{
}

bool
CGnuPlotBubble::
inside(const CPoint2D &p) const
{
  double r = p.distanceTo(c_);

  if (r > xr_ || r > yr_)
    return false;

  return true;
}

void
CGnuPlotBubble::
draw(CGnuPlotRenderer *renderer) const
{
  renderer->fillEllipse(c_, xr_, yr_, 0, color_.getValue(CRGBA(1,0,0)));

  double s = renderer->fontSize();

  renderer->setFontSize(6);

  renderer->drawHAlignedText(c_, CHALIGN_TYPE_CENTER, 0, CVALIGN_TYPE_CENTER, 0,
                             name_, CRGBA(0,0,0));

  renderer->setFontSize(s);
}
