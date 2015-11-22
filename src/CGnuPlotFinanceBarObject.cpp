#include <CGnuPlotFinanceBarObject.h>
#include <CGnuPlotPlot.h>
#include <CGnuPlotGroup.h>
#include <CGnuPlotRenderer.h>

CGnuPlotFinanceBarObject::
CGnuPlotFinanceBarObject(CGnuPlotPlot *plot) :
 CGnuPlotPlotObject(plot)
{
  stroke_ = plot->createStroke();
}

bool
CGnuPlotFinanceBarObject::
inside(const CGnuPlotMouseEvent &mouseEvent) const
{
  // TODO: distance to lines
  return bbox_.inside(mouseEvent.window());
}

CGnuPlotTipData
CGnuPlotFinanceBarObject::
tip() const
{
  CGnuPlotTipData tip;

  tip.setXStr(CStrUtil::strprintf("%g", value()));
  tip.setYStr(CStrUtil::strprintf("%g,%g", low(), high()));

  tip.setBBox(bbox_);

  tip.setBorderColor(stroke_->color());
  tip.setXColor     (stroke_->color());

  return tip;
}

void
CGnuPlotFinanceBarObject::
draw(CGnuPlotRenderer *renderer) const
{
  if (! isUsed()) return;

  bool highlighted = (isHighlighted() || isSelected());

  CGnuPlotStrokeP stroke = stroke_;

  if (highlighted) {
    stroke = stroke_->dup();

    stroke->setEnabled(true);
    stroke->setColor(CRGBA(1,0,0));
    stroke->setWidth(2);
  }

  bbox_.reset();

  CPoint2D p1(value(), low()  );
  CPoint2D p2(value(), high() );
  CPoint2D p3(value(), open() );
  CPoint2D p4(value(), close());

  CPoint2D db(barWidth(), 0);

  bbox_.add(p1);
  bbox_.add(p2);
  bbox_.add(p3 - db);
  bbox_.add(p4 + db);

  CGnuPlotGroup *group = plot_->group();

  p1 = group->mapLogPoint(plot_->xind(), plot_->yind(), 1, p1);
  p2 = group->mapLogPoint(plot_->xind(), plot_->yind(), 1, p2);
  p3 = group->mapLogPoint(plot_->xind(), plot_->yind(), 1, p3);
  p4 = group->mapLogPoint(plot_->xind(), plot_->yind(), 1, p4);

  renderer->strokeClipLine(p1, p2     , *stroke);
  renderer->strokeClipLine(p3, p3 - db, *stroke);
  renderer->strokeClipLine(p4, p4 + db, *stroke);
}
