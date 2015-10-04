#include <CGnuPlotStyleLabels.h>
#include <CGnuPlotLabelObject.h>
#include <CGnuPlotWindow.h>
#include <CGnuPlotGroup.h>
#include <CGnuPlotPlot.h>
#include <CGnuPlotRenderer.h>
#include <CGnuPlotDevice.h>

CGnuPlotStyleLabels::
CGnuPlotStyleLabels() :
 CGnuPlotStyleBase(CGnuPlot::PlotStyle::LABELS)
{
}

void
CGnuPlotStyleLabels::
draw2D(CGnuPlotPlot *plot, CGnuPlotRenderer *renderer)
{
  CGnuPlotGroup *group = plot->group();

  bool enhanced = plot->isEnhanced();

  const CGnuPlotLineStyle    &lineStyle    = plot->lineStyle();
  const CGnuPlotTextStyle    &textStyle    = plot->textStyle();
  const CGnuPlotTextBoxStyle &textBoxStyle = plot->textBoxStyle();
  const CGnuPlotLabelStyle   &labelStyle   = plot->labelStyle();

  bool isCalcColor = lineStyle.isCalcColor();

  CRGBA tc(0, 0, 0);

  if (labelStyle.textColor().isValid())
    tc = labelStyle.textColor().getValue().calcColor(group);

  //CFontPtr font = renderer->getFont();
  //double tw1 = renderer->pixelWidthToWindowWidth(font->getStringWidth("X"));

  typedef std::vector<CGnuPlotLabelData> LabelDatas;

  LabelDatas labelDatas;

  for (const auto &point : plot->getPoints2D()) {
    std::vector<double> reals;

    (void) point.getReals(reals);

    if (reals.size() < 3)
      continue;

    CPoint2D p(reals[0], reals[1]);

    std::string str;

    if (! point.getValue(3, str))
      continue;

    CRGBA tc1 = tc;

    if (isCalcColor && reals.size() > 3) {
      double z = reals[3];

      if (renderer->isPseudo())
        renderer->setCBValue(z);
      else
        tc1 = lineStyle.calcColor(plot, z);
    }

    //---

    CGnuPlotPosition pos(CPoint3D(p.x, p.y, 0));

    //---

    CGnuPlotLabelData labelData;

    labelData.setText(str);
    labelData.setPos (pos);

    CGnuPlotColorSpec cs;

    cs.setRGB(tc1);

    labelData.setTextColor(cs);

    labelData.setAngle(0);

    labelData.setEnhanced(enhanced);

    labelData.setAlign(labelStyle.align());

    if (labelStyle.font().isValid())
      labelData.setFont(labelStyle.font());

    labelData.setOffset(textStyle.offset());

    labelData.setBoxFill(! textBoxStyle.isTransparent());
    labelData.setBoxFillColor(group->window()->backgroundColor());

    labelData.setBox(textStyle.isBoxed());
    labelData.setBoxStrokeColor(CRGBA(0,0,0));
    labelData.setBoxStrokeWidth(1);

    labelData.setShowPoint (labelStyle.showPoint ());
    labelData.setPointType ((int) labelStyle.symbolType());
    labelData.setPointSize (labelStyle.pointSize());
    labelData.setPointWidth(labelStyle.lineWidth());

    labelDatas.push_back(labelData);

    //---

#if 0
    CBBox2D bbox;

    double tw = renderer->pixelWidthToWindowWidth  (font->getStringWidth(str));
    double th = renderer->pixelHeightToWindowHeight(font->getCharHeight());

    if (enhanced) {
      bbox = text.calcBBox(renderer).moveBy(p);

      bbox.setYMax(p.y);
    }
    else {
      bbox = CBBox2D(p, p + CPoint2D(tw, th));
    }

    CPoint2D d(-bbox.getWidth()/2, -bbox.getHeight()/2);

    CPoint2D of = textStyle.offset()*CPoint2D(tw1, th);

    bbox.moveBy(d + of);

    //---

    if (! textBoxStyle.isTransparent())
      renderer->fillRect(bbox, group->window()->backgroundColor());

    if (textStyle.isBoxed())
      renderer->drawRect(bbox, CRGBA(0,0,0), 1);

    //---

    if (labelStyle.font().isValid())
      renderer->setFont(labelStyle.font());

    if (enhanced)
      text.draw(renderer, bbox, labelStyle.align(), tc1);
    else {
      double fd = renderer->pixelHeightToWindowHeight(font->getCharDescent());

      renderer->drawHAlignedText(p + d + of + CPoint2D(0, fd), labelStyle.align(), 0,
                                 CVALIGN_TYPE_CENTER, 0, str, tc1);
    }

    //---

    if (labelStyle.showPoint()) {
      CGnuPlotTypes::SymbolType pt = labelStyle.symbolType();

      renderer->drawSymbol(p, pt, labelStyle.pointSize(), tc1, labelStyle.lineWidth(), true);
    }
#endif
  }

  //------

  uint n = labelDatas.size();

  if (! renderer->isPseudo())
    plot->updateLabelCacheSize(n);

  int i = 0;

  for (const auto &labelData : labelDatas) {
    if (! renderer->isPseudo()) {
      CGnuPlotLabelObject *label = plot->labelObjects()[i];

      if (! label->testAndSetUsed()) {
        label->setData(labelData);
      }
    }
    else {
      labelData.draw(renderer, group, false);
    }

    ++i;
  }

  //------

  if (! renderer->isPseudo()) {
    for (const auto &label : plot->labelObjects())
      label->draw(renderer);
  }
}

void
CGnuPlotStyleLabels::
draw3D(CGnuPlotPlot *plot, CGnuPlotRenderer *renderer)
{
  CGnuPlotGroup *group = plot->group();

  bool enhanced = plot->isEnhanced();

  const CGnuPlotLineStyle    &lineStyle    = plot->lineStyle();
  const CGnuPlotTextStyle    &textStyle    = plot->textStyle();
  const CGnuPlotTextBoxStyle &textBoxStyle = plot->textBoxStyle();
  const CGnuPlotLabelStyle   &labelStyle   = plot->labelStyle();

  bool isCalcColor = lineStyle.isCalcColor();

  CRGBA tc(0, 0, 0);

  if (labelStyle.textColor().isValid())
    tc = labelStyle.textColor().getValue().calcColor(group);

  CFontPtr font = renderer->getFont();

  double tw1 = renderer->pixelWidthToWindowWidth(font->getStringWidth("X"));

  for (const auto &ip : plot->getPoints3D()) {
    for (const auto &point : ip.second) {
      CPoint3D p;
      int      ind;

      if (! plot->mapPoint3D(point, p, ind))
        continue;

      ++ind;

      CPoint2D p1 = renderer->transform(p);

      std::string str;

      if (! point.getValue(ind, str))
        continue;

      ++ind;

      CRGBA tc1 = tc;

      double z;

      if (isCalcColor && point.getValue(ind, z)) {
        if (renderer->isPseudo())
          renderer->setCBValue(z);
        else
          tc1 = lineStyle.calcColor(plot, z);
      }

      //----

      CBBox2D bbox;

      CGnuPlotText text(str);

      double tw = renderer->pixelWidthToWindowWidth  (font->getStringWidth(str));
      double th = renderer->pixelHeightToWindowHeight(font->getCharHeight());

      if (enhanced) {
        bbox = text.calcBBox(renderer).moveBy(p1);

        bbox.setYMax(p1.y);
      }
      else {
        bbox = CBBox2D(p1, p1 + CPoint2D(tw, th));
      }

      CPoint2D d(-bbox.getWidth()/2, -bbox.getHeight()/2);

      CPoint2D of = textStyle.offset()*CPoint2D(tw1, th);

      bbox.moveBy(d + of);

      if (! textBoxStyle.isTransparent())
        renderer->fillRect(bbox, group->window()->backgroundColor());

      if (textStyle.isBoxed())
        renderer->drawRect(bbox, CRGBA(0,0,0), 1);

      renderer->setFont(labelStyle.font());

      if (enhanced)
        text.draw(renderer, bbox, labelStyle.align(), tc1);
      else {
        double fd = renderer->pixelHeightToWindowHeight(font->getCharDescent());

        renderer->drawHAlignedText(p1 + d + of + CPoint2D(0, fd), labelStyle.align(), 0,
                                   CVALIGN_TYPE_CENTER, 0, str, tc1);
      }

      if (labelStyle.showPoint()) {
        CGnuPlotTypes::SymbolType pt = (CGnuPlotTypes::SymbolType) labelStyle.pointType();

        renderer->drawSymbol(p1, pt, labelStyle.pointSize(), tc1, labelStyle.lineWidth(), true);
      }
    }
  }
}

CBBox2D
CGnuPlotStyleLabels::
fit(CGnuPlotPlot *)
{
  return CBBox2D();
}
