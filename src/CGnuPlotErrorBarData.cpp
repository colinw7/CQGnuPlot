#include <CGnuPlotErrorBarData.h>
#include <CGnuPlotRenderer.h>

void
CGnuPlotErrorBarData::
draw(CGnuPlotRenderer *renderer) const
{
  if (isYDirection()) {
    // vertical line with bars
    renderer->drawClipLine(yLowPoint(), yHighPoint(), lineWidth(), lineColor());

    CPoint2D db(barWidth(), 0);

    renderer->drawClipLine(yLowPoint () - db, yLowPoint () + db, lineWidth(), lineColor());
    renderer->drawClipLine(yHighPoint() - db, yHighPoint() + db, lineWidth(), lineColor());
  }

  if (isXDirection()) {
    // horizontal line with bars
    renderer->drawClipLine(xLowPoint(), xHighPoint(), lineWidth(), lineColor());

    CPoint2D db(0, barHeight());

    renderer->drawClipLine(xLowPoint () - db, xLowPoint () + db, lineWidth(), lineColor());
    renderer->drawClipLine(xHighPoint() - db, xHighPoint() + db, lineWidth(), lineColor());
  }

  // draw symbol at value
  renderer->drawSymbol(point(), pointType(), pointSize(),
                       lineColor(), lineWidth(), /*pixelSize*/true);
}
