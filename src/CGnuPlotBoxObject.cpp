#include <CGnuPlotBoxObject.h>
#include <CGnuPlotRenderer.h>
#include <CGnuPlotPlot.h>

CGnuPlotBoxObject::
CGnuPlotBoxObject(CGnuPlotPlot *plot) :
 CGnuPlotPlotObject(plot)
{
  stroke_      = plot->createStroke();
  fill_        = plot->createFill  ();
  outlierMark_ = plot->createMark  ();
}

CGnuPlotTipData
CGnuPlotBoxObject::
tip() const
{
  CGnuPlotTipData tip;

  tip.setXStr(CStrUtil::strprintf("%g", x()));
  tip.setYStr(valueStr());

  tip.setBorderColor(fill_->color());
  tip.setXColor     (fill_->color());

  tip.setBBox(bbox_);

  return tip;
}

void
CGnuPlotBoxObject::
draw(CGnuPlotRenderer *renderer) const
{
  if (! isDisplayed()) return;

  bool highlighted = (isHighlighted() || isSelected());

  CGnuPlotFillP   fill   = fill_;
  CGnuPlotStrokeP stroke = stroke_;
  CGnuPlotMarkP   mark   = outlierMark_;

  if (highlighted) {
    fill   = fill_       ->dup();
    stroke = stroke_     ->dup();
    mark   = outlierMark_->dup();

    fill->setColor(fill->color().getLightRGBA());

    stroke->setEnabled(true);
    stroke->setColor(CRGBA(1,0,0));
    stroke->setWidth(2);

    mark->setColor(CRGBA(1,0,0));
  }

  // vertical line from min to max
  renderer->strokeClipLine(CPoint2D(x(), minValue()), CPoint2D(x(), maxValue()), *stroke);

  // horizontal lines at min and max
  double x11 = x() - lineWidth()/2;
  double x21 = x() + lineWidth()/2;

  renderer->strokeClipLine(CPoint2D(x11, minValue()), CPoint2D(x21, minValue()), *stroke);
  renderer->strokeClipLine(CPoint2D(x11, maxValue()), CPoint2D(x21, maxValue()), *stroke);

  // draw box
  double x12 = x() - boxWidth()/2;
  double x22 = x() + boxWidth()/2;

  bbox_ = CBBox2D(x12, lowerValue(), x22, upperValue());

  renderer->fillRect  (bbox_, *fill);
  renderer->strokeRect(bbox_, *stroke);

  // draw median in box
  renderer->strokeClipLine(CPoint2D(x12, medianValue()), CPoint2D(x22, medianValue()), *stroke);

  // draw outliers
  if (hasOutliers()) {
    for (const auto &o : outlierValues())
      renderer->drawMark(CPoint2D(x(), value(o)), *mark);
  }

  // add range for pseudo (bbox renderer)
  if (renderer->isPseudo()) {
    CGnuPlotTypes::SymbolType symbol = CGnuPlotTypes::SymbolType::CROSS;

    renderer->drawSymbol(CPoint2D(x() - 1, 0), symbol, 1, CRGBA(0,0,0), 1, true);
    renderer->drawSymbol(CPoint2D(x() + 1, 0), symbol, 1, CRGBA(0,0,0), 1, true);
  }

  // draw label
  renderer->drawHAlignedText(CPoint2D(x(), y()), HAlignPos(CHALIGN_TYPE_CENTER, 0),
                             VAlignPos(CVALIGN_TYPE_TOP, -8), valueStr(), textColor());
}
