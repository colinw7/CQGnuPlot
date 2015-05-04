#include <CGnuPlotEllipseObject.h>
#include <CGnuPlotPlot.h>
#include <CGnuPlotRenderer.h>
#include <CMathGeom2D.h>

CGnuPlotEllipseObject::
CGnuPlotEllipseObject(CGnuPlotPlot *plot) :
 CGnuPlotPlotObject(plot)
{
}

void
CGnuPlotEllipseObject::
setCenter(const CPoint2D &center)
{
  center_ = center;

  update();
}

void
CGnuPlotEllipseObject::
setSize(const CSize2D &size)
{
  size_ = size;

  update();
}

void
CGnuPlotEllipseObject::
update()
{
  rect_ = CBBox2D();

  double w = size_.getWidth ();
  double h = size_.getHeight();

  rect_ += center_ - CPoint2D(w, h);
  rect_ += center_ + CPoint2D(w, h);
}

bool
CGnuPlotEllipseObject::
inside(const CPoint2D &p) const
{
  if (! rect_.inside(p))
    return false;

  // inside ellipse
  return true;
}

void
CGnuPlotEllipseObject::
draw(CGnuPlotRenderer *renderer) const
{
  double w = size_.getWidth ();
  double h = size_.getHeight();

  if (fillColor_.isValid())
    renderer->fillEllipse(center_, w, h, 0, fillColor_.getValue());

  if (lineColor_.isValid())
    renderer->drawEllipse(center_, w, h, 0, lineColor_.getValue(), lineWidth_);

  if (text_ != "") {
    double tw = renderer->pixelWidthToWindowWidth  (renderer->getFont()->getStringWidth(text_));
    double th = renderer->pixelHeightToWindowHeight(renderer->getFont()->getCharAscent());

    renderer->drawText(center_ - CPoint2D(tw/2, th/2), text_, CRGBA(0,0,0));
  }
}
