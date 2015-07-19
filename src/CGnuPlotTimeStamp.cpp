#include <CGnuPlotTimeStamp.h>
#include <CGnuPlotGroup.h>
#include <CGnuPlotRenderer.h>

CGnuPlotTimeStamp::
CGnuPlotTimeStamp(CGnuPlotGroup *group) :
 group_(group)
{
}

CGnuPlotTimeStamp::
~CGnuPlotTimeStamp()
{
}

void
CGnuPlotTimeStamp::
draw(CGnuPlotRenderer *renderer)
{
  if (data_.format() == "") return;

  CBBox2D bbox = group_->getRegionBBox();

  double pw = renderer->pixelWidthToWindowWidth  (8);
  double ph = renderer->pixelHeightToWindowHeight(8);

  std::string s;

  time_t t = time(0);

  struct tm *tm1 = localtime(&t);

  static char buffer[256];

  strftime(buffer, 256, data_.format().c_str(), tm1);

  CRGBA c = data_.textColor().color();

  if (isTop())
    renderer->drawHAlignedText(CPoint2D(bbox.getXMin() + pw, bbox.getYMax() - ph),
                               CHALIGN_TYPE_LEFT, 0, CVALIGN_TYPE_TOP   , 0, buffer, c);
  else
    renderer->drawHAlignedText(CPoint2D(bbox.getXMin() + pw, bbox.getYMin() + ph),
                               CHALIGN_TYPE_LEFT, 0, CVALIGN_TYPE_BOTTOM, 0, buffer, c);
}
