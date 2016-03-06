#include <CGnuPlotErrorBarData.h>
#include <CGnuPlotRenderer.h>

void
CGnuPlotErrorBarData::
draw(CGnuPlotRenderer *renderer) const
{
  if (isYDirection()) {
    // vertical line with bars
    renderer->drawClipLine(yLowPoint(), yHighPoint(), lineColor(), lineWidth());

    CPoint2D db(barWidth(), 0);

    renderer->drawClipLine(yLowPoint () - db, yLowPoint () + db, lineColor(), lineWidth());
    renderer->drawClipLine(yHighPoint() - db, yHighPoint() + db, lineColor(), lineWidth());
  }

  if (isXDirection()) {
    // horizontal line with bars
    renderer->drawClipLine(xLowPoint(), xHighPoint(), lineColor(), lineWidth());

    CPoint2D db(0, barHeight());

    renderer->drawClipLine(xLowPoint () - db, xLowPoint () + db, lineColor(), lineWidth());
    renderer->drawClipLine(xHighPoint() - db, xHighPoint() + db, lineColor(), lineWidth());
  }

  // draw symbol at value
  renderer->drawSymbol(point(), pointType(), pointSize(),
                       lineColor(), lineWidth(), /*pixelSize*/true);
}
