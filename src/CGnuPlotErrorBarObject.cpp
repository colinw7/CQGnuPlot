#include <CGnuPlotErrorBarObject.h>
#include <CGnuPlotPlot.h>
#include <CGnuPlotRenderer.h>

CGnuPlotErrorBarObject::
CGnuPlotErrorBarObject(CGnuPlotPlot *plot) :
 CGnuPlotPlotObject(plot)
{
  stroke_ = plot->createStroke();
  mark_   = plot->createMark();
}

bool
CGnuPlotErrorBarObject::
inside(const CGnuPlotMouseEvent &mouseEvent) const
{
  // TODO: distance to lines
  return bbox_.inside(mouseEvent.window());
}

CGnuPlotTipData
CGnuPlotErrorBarObject::
tip() const
{
  CGnuPlotTipData tip;

  tip.setXStr(CStrUtil::strprintf("%g", x_));
  tip.setYStr(CStrUtil::strprintf("%g", y_));

  tip.setBBox(bbox_);

  tip.setBorderColor(stroke_->color());
  tip.setXColor     (stroke_->color());

  return tip;
}

void
CGnuPlotErrorBarObject::
draw(CGnuPlotRenderer *renderer) const
{
  if (! isDisplayed()) return;

  if (! isUsed()) return;

  bool highlighted = (isHighlighted() || isSelected());

  CGnuPlotStrokeP stroke = stroke_;
  CGnuPlotMarkP   mark   = mark_;

  if (highlighted) {
    stroke = stroke_->dup();

    stroke->setEnabled(true);
    stroke->setColor(CRGBA(1,0,0));
    stroke->setWidth(2);

    mark = mark_->dup();

    mark->setColor(CRGBA(1,0,0));
  }

  bbox_.reset();

  // vertical line with bars
  if (isYDirection()) {
    CPoint2D pl(x(), yLow ());
    CPoint2D ph(x(), yHigh());

    renderer->strokeClipLine(pl, ph, *stroke);

    CPoint2D db(barWidth(), 0);

    renderer->strokeClipLine(pl - db, pl + db, *stroke);
    renderer->strokeClipLine(ph - db, ph + db, *stroke);

    bbox_.add(pl - db);
    bbox_.add(ph + db);
  }

  // horizontal line with bars
  if (isXDirection()) {
    CPoint2D pl(xLow (), y());
    CPoint2D ph(xHigh(), y());

    renderer->strokeClipLine(pl, ph, *stroke);

    CPoint2D db(0, barHeight());

    renderer->strokeClipLine(pl - db, pl + db, *stroke);
    renderer->strokeClipLine(ph - db, ph + db, *stroke);

    bbox_.add(pl - db);
    bbox_.add(ph + db);
  }

  // draw symbol at value
  CPoint2D p(x(), y());

  renderer->drawMark(p, *mark);

  bbox_.add(p);
}
