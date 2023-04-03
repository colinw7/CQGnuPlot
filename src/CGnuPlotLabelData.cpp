#include <CGnuPlotLabelData.h>
#include <CGnuPlotRenderer.h>
#include <CGnuPlotGroup.h>
#include <CGnuPlotPlot.h>
#include <CGnuPlotDevice.h>
#include <CFontMgr.h>

CFontPtr CGnuPlotLabelData::defFont_;

CGnuPlotLabelData::
CGnuPlotLabelData(CGnuPlotGroup *group) :
 group_(group)
{
  if (! defFont_)
    defFont_ = CFontMgrInst->lookupFont("helvetica", CFONT_STYLE_NORMAL, 12);

  if (group) {
    boxFill_   = group->app()->device()->createFill  (nullptr);
    boxStroke_ = group->app()->device()->createStroke(nullptr);
  }
}

CGnuPlotLabelData::
CGnuPlotLabelData(CGnuPlotPlot *plot) :
 plot_(plot)
{
  if (! defFont_)
    defFont_ = CFontMgrInst->lookupFont("helvetica", CFONT_STYLE_NORMAL, 12);

  boxFill_   = plot->createFill();
  boxStroke_ = plot->createStroke();
}

CGnuPlotLabelData::
~CGnuPlotLabelData()
{
  delete boxFill_;
  delete boxStroke_;
}

CGnuPlotLabelData *
CGnuPlotLabelData::
dup() const
{
  CGnuPlotLabelData *data = new CGnuPlotLabelData();

  data->plot_       =  plot_;
  data->text_       =  text_;
  data->pos_        =  pos_;
  data->angle_      =  angle_;
  data->enhanced_   =  enhanced_;
  data->labelStyle_ =  labelStyle_;
  data->textColor_  =  textColor_;
  data->lineType_   =  lineType_;
  data->boxFill_    =  (boxFill_   ? boxFill_  ->dup() : nullptr);
  data->boxStroke_  =  (boxStroke_ ? boxStroke_->dup() : nullptr);
  data->hypertext_  =  hypertext_;
  data->pointSize_  =  pointSize_;
  data->pointWidth_ =  pointWidth_;

  return data;
}

void
CGnuPlotLabelData::
draw(CGnuPlotRenderer *renderer, CGnuPlotGroup *group, bool highlighted) const
{
  if (! group_ && ! plot_) {
    CGnuPlotLabelData *th = const_cast<CGnuPlotLabelData *>(this);

    th->boxFill_   = group->app()->device()->createFill  (nullptr);
    th->boxStroke_ = group->app()->device()->createStroke(nullptr);

    th->group_ = group;
  }

  assert(boxFill_ && boxStroke_);

  // get font, align and angle
  CFontPtr font1 = defFont_;

  if (getFont())
    font1 = getFont();

  renderer->setFont(font1);

  double a = angle_.getValue(0);

  CHAlignType halign = getAlign();
  CVAlignType valign = CVALIGN_TYPE_BOTTOM;

  //---

  // calc bbox

  CPoint3D pos = getPos().getPoint3D(renderer);

  drawPos_ = renderer->transform2D(pos);

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

  renderer->calcTextRectAtPoint(drawPos_, getText(), isEnhanced(), HAlignPos(halign, 0),
                                VAlignPos(valign, 0), a, bbox, rbbox);

  bbox_ = rbbox;

  COptPoint2D o;

  if (rbbox.isSet())
    o = rbbox.getCenter();

  //---

  // fill box background and border
  if (boxStroke()->isEnabled() && bbox.isSet()) {
    renderer->fillRotatedRect(bbox, a, boxFill()->color(), o);
  }

  //---

  if (boxStroke()->isEnabled() && bbox.isSet()) {
    //renderer->drawRect(bbox, boxStroke()->Color(), 1);
    //renderer->drawRotatedRect(bbox, a, c, 1, o);

    CLineDash dash;

    renderer->drawRotatedRect(bbox, a, boxStroke()->color(), boxStroke()->width(), dash, o);
  }

  //---

  // draw point symbol
  if (showPoint()) {
    //renderer->drawSymbol(drawPos_, SymbolType::CROSS, 1, c, 1, true);

    if (group->isHidden3D())
      renderer->drawHiddenSymbol(pos, symbolType(), pointSize(), c, pointWidth(), true);
    else
      renderer->drawSymbol(pos, symbolType(), pointSize(), c, pointWidth(), true);
  }

  //---

  // draw text
  if (group->isHidden3D())
    renderer->drawHiddenTextAtPoint(pos, getText(), isEnhanced(),
                                    HAlignPos(halign, 0), VAlignPos(valign, 0), font1, c, a);
  else
    renderer->drawTextAtPoint(pos, getText(), isEnhanced(),
                              HAlignPos(halign, 0), VAlignPos(valign, 0), c, a);

//renderer->drawSymbol(o.getValue(), SymbolType::PLUS, 1, CRGBA(1,0,0), 1, true);
}

bool
CGnuPlotLabelData::
inside(const CGnuPlotMouseEvent &mouseEvent) const
{
  if (! bbox_.isSet()) return false;

  return bbox_.inside(mouseEvent.window());
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

  tip.setBBox(bbox_);

  return tip;
}
