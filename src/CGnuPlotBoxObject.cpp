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
  double lw2 = lineWidth()/2;

  double x11 = x() - lw2;
  double x21 = x() + lw2;

  renderer->strokeClipLine(CPoint2D(x11, minValue()), CPoint2D(x21, minValue()), *stroke);
  renderer->strokeClipLine(CPoint2D(x11, maxValue()), CPoint2D(x21, maxValue()), *stroke);

  // draw box
  double x12 = x() - boxWidth()/2;
  double x22 = x() + boxWidth()/2;

  bbox_ = CBBox2D(x12, lowerValue(), x22, upperValue());

  if (boxType() == BoxType::CandleSticks) {
    // draw box from lower to upper
    renderer->fillRect  (bbox_, *fill);
    renderer->strokeRect(bbox_, *stroke);

    // draw median in box
    renderer->strokeClipLine(CPoint2D(x12, medianValue()), CPoint2D(x22, medianValue()), *stroke);
  }
  else {
    // draw lines at lower and upper
    renderer->strokeClipLine(CPoint2D(x11, lowerValue()), CPoint2D(x21, lowerValue()), *stroke);
    renderer->strokeClipLine(CPoint2D(x11, upperValue()), CPoint2D(x21, upperValue()), *stroke);

    // draw median line
    renderer->strokeClipLine(CPoint2D(x11, medianValue()), CPoint2D(x21, medianValue()), *stroke);
  }

  // draw outliers
  if (isShowOutliers()) {
    if (hasOutliers()) {
      for (const auto &o : outlierValues())
        renderer->drawMark(CPoint2D(x(), value(o)), *mark);
    }
  }

  // add range for pseudo (bbox renderer)
  if (renderer->isPseudo()) {
    CGnuPlotTypes::SymbolType symbol = CGnuPlotTypes::SymbolType::CROSS;

    renderer->drawSymbol(CPoint2D(x() - 1, 0), symbol, 1, CRGBA(0,0,0), 1, true);
    renderer->drawSymbol(CPoint2D(x() + 1, 0), symbol, 1, CRGBA(0,0,0), 1, true);
  }

  // draw value labels
  if (boxLabels() == BoxLabels::Auto) {
    std::string ustr = CStrUtil::strprintf("%g", upperValue ());
    std::string lstr = CStrUtil::strprintf("%g", lowerValue ());
    std::string mstr = CStrUtil::strprintf("%g", medianValue());
    std::string strl = CStrUtil::strprintf("%g", minValue   ());
    std::string strh = CStrUtil::strprintf("%g", maxValue   ());

    if (boxType() == BoxType::CandleSticks) {
      renderer->drawHAlignedText(CPoint2D(x12 - lw2, upperValue ()),
                                 HAlignPos(CHALIGN_TYPE_RIGHT, 0),
                                 VAlignPos(CVALIGN_TYPE_CENTER, 0), ustr, textColor());
      renderer->drawHAlignedText(CPoint2D(x12 - lw2, lowerValue ()),
                                 HAlignPos(CHALIGN_TYPE_RIGHT, 0),
                                 VAlignPos(CVALIGN_TYPE_CENTER, 0), lstr, textColor());
      renderer->drawHAlignedText(CPoint2D(x22 + lw2, medianValue()),
                                 HAlignPos(CHALIGN_TYPE_LEFT, 0),
                                 VAlignPos(CVALIGN_TYPE_CENTER, 0), mstr, textColor());
    }
    else {
      renderer->drawHAlignedText(CPoint2D(x11 - lw2, upperValue ()),
                                 HAlignPos(CHALIGN_TYPE_RIGHT, 0),
                                 VAlignPos(CVALIGN_TYPE_CENTER, 0), ustr, textColor());
      renderer->drawHAlignedText(CPoint2D(x11 - lw2, lowerValue ()),
                                 HAlignPos(CHALIGN_TYPE_RIGHT, 0),
                                 VAlignPos(CVALIGN_TYPE_CENTER, 0), lstr, textColor());
      renderer->drawHAlignedText(CPoint2D(x21 + lw2, medianValue()),
                                 HAlignPos(CHALIGN_TYPE_LEFT, 0),
                                 VAlignPos(CVALIGN_TYPE_CENTER, 0), mstr, textColor());
    }

    renderer->drawHAlignedText(CPoint2D(x21 + lw2, minValue()), HAlignPos(CHALIGN_TYPE_LEFT, 0),
                               VAlignPos(CVALIGN_TYPE_CENTER, 0), strl, textColor());
    renderer->drawHAlignedText(CPoint2D(x21 + lw2, maxValue()), HAlignPos(CHALIGN_TYPE_LEFT, 0),
                               VAlignPos(CVALIGN_TYPE_CENTER, 0), strh, textColor());
  }

  // draw label
  renderer->drawHAlignedText(CPoint2D(x(), y()), HAlignPos(CHALIGN_TYPE_CENTER, 0),
                             VAlignPos(CVALIGN_TYPE_TOP, -8), valueStr(), textColor());
}
