#include <CGnuPlotLabelData.h>
#include <CGnuPlotRenderer.h>
#include <CGnuPlotGroup.h>
#include <CFontMgr.h>

CFontPtr CGnuPlotLabelData::defFont_;

CGnuPlotLabelData::
CGnuPlotLabelData()
{
  if (! defFont_.isValid())
    defFont_ = CFontMgrInst->lookupFont("helvetica", CFONT_STYLE_NORMAL, 12);
}

void
CGnuPlotLabelData::
draw(CGnuPlotRenderer *renderer, CGnuPlotGroup *group, bool highlighted) const
{
  // get font, align and angle
  if (getFont().isValid())
    renderer->setFont(getFont());
  else
    renderer->setFont(defFont_);

  double a = angle_.getValue(0);

  CHAlignType halign = getAlign();
  CVAlignType valign = CVALIGN_TYPE_BOTTOM;

  //---

  // calc bbox

  CPoint3D pos = getPos().getPoint3D(renderer);

  drawPos_ = renderer->transform(pos);

  //---

  // get text color
  if (textColor_.isPaletteZ())
    drawColor_ = textColor_.calcColor(group, pos.z);
  else
    drawColor_ = textColor_.color();

  CRGBA c = drawColor_;

  if (highlighted)
    c = CRGBA(1,0,0);

  //---

  CBBox2D bbox, rbbox;

  renderer->calcTextRectAtPoint(drawPos_, getText(), isEnhanced(), halign, valign, a, bbox, rbbox);

  bbox_ = rbbox;

  COptPoint2D o;

  o = rbbox.getCenter();

  //---

  // fill box background and border
  if (isBoxFill()) {
    renderer->fillRotatedRect(bbox, a, getBoxFillColor(), o);
  }

  //---

  if (hasBox() && bbox.isSet()) {
    //renderer->drawRect(bbox, getBoxStrokeColor(), 1);
    //renderer->drawRotatedRect(bbox, a, c, 1, o);

    renderer->drawRotatedRect(bbox, a, getBoxStrokeColor(), getBoxStrokeWidth(), o);
  }

  //---

  // draw point symbol
  if (showPoint()) {
    //renderer->drawSymbol(drawPos_, CGnuPlotTypes::SymbolType::CROSS, 1, c, 1, true);
    renderer->drawSymbol(drawPos_, (CGnuPlotTypes::SymbolType) pointType(),
                         pointSize(), c, pointWidth(), true);
  }

  //---

  // draw text
  renderer->drawTextAtPoint(drawPos_, getText(), isEnhanced(), halign, valign, c, a);

//renderer->drawSymbol(o.getValue(), CGnuPlotTypes::SymbolType::PLUS, 1, CRGBA(1,0,0), 1, true);
}

bool
CGnuPlotLabelData::
inside(const CGnuPlotTypes::InsideData &data) const
{
  return bbox_.inside(data.window);
}

CGnuPlotTipData
CGnuPlotLabelData::
tip() const
{
  CGnuPlotTipData tip;

  tip.setXStr(CStrUtil::strprintf("%g, %g", drawPos_.x, drawPos_.y));
  tip.setYStr(CStrUtil::strprintf("%s", getText().c_str()));

  CRGBA c = this->drawColor();

  tip.setBorderColor(c);
  tip.setXColor(c);

  tip.setRect(bbox_);

  return tip;
}
