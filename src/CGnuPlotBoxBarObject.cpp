#include <CGnuPlotBoxBarObject.h>
#include <CGnuPlotPlot.h>
#include <CGnuPlotRenderer.h>

CGnuPlotBoxBarObject::
CGnuPlotBoxBarObject(CGnuPlotPlot *plot) :
 CGnuPlotPlotObject(plot)
{
  fill_   = plot->createFill  ();
  stroke_ = plot->createStroke();
}

void
CGnuPlotBoxBarObject::
clearEndBars()
{
  endBars_.clear();
}

CGnuPlotEndBarP
CGnuPlotBoxBarObject::
addEndBar(const CPoint2D &start, const CPoint2D &end)
{
  CGnuPlotEndBarP endBar(plot_->createEndBar());

  endBar->setBarObject(this);

  endBar->setStart(start);
  endBar->setEnd  (end);

  endBars_.push_back(endBar);

  return endBars_.back();
}

bool
CGnuPlotBoxBarObject::
inside(const CGnuPlotTypes::InsideData &data) const
{
  return bbox_.inside(data.window);
}

CGnuPlotTipData
CGnuPlotBoxBarObject::
tip() const
{
  CGnuPlotTipData tip;

  if (x_str_ != "")
    tip.setXStr(x_str_);
  else
    tip.setXStr(CStrUtil::strprintf("%g", x_));

  if (y_str_ != "")
    tip.setYStr(y_str_);
  else
    tip.setYStr(CStrUtil::strprintf("%g", y_));

  tip.setRect(bbox_);

  tip.setXColor(CRGBA(0,0,0));
  tip.setYColor(CRGBA(0,0,0));

  if (fill_->type() == CGnuPlotTypes::FillType::SOLID) {
    tip.setBorderColor(fill_->color());
    tip.setXColor     (fill_->color());
  }

  return tip;
}

void
CGnuPlotBoxBarObject::
draw(CGnuPlotRenderer *renderer) const
{
  if (! isUsed()) return;

  bool highlighted = (isHighlighted() || isSelected());

  CGnuPlotFillP   fill   = fill_;
  CGnuPlotStrokeP stroke = stroke_;

  if (highlighted) {
    fill   = fill_  ->dup();
    stroke = stroke_->dup();

    if      (fill->type() == CGnuPlotTypes::FillType::PATTERN) {
      CRGBA fc = fill->color();

      double g = fc.getGray();

      if (g < 0.5)
        fc = CRGBA(1, 1, 1);
      else
        fc = CRGBA(0, 0, 0);

      fill->setColor(fc);
    }
    else if (fill->type() == CGnuPlotTypes::FillType::SOLID) {
      fill->setColor(fill->color().getLightRGBA());
    }

    stroke->setEnabled(true);
    stroke->setColor(CRGBA(1,0,0));
    stroke->setWidth(2);
  }

  renderer->fillRect  (bbox_, *fill  );
  renderer->strokeRect(bbox_, *stroke);

  //---

  for (const auto &endBar : endBars_)
    endBar->draw(renderer);
}
