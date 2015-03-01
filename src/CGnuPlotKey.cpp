#include <CGnuPlotKey.h>
#include <CGnuPlotGroup.h>
#include <CGnuPlotPlot.h>
#include <CGnuPlotWindow.h>
#include <CGnuPlotRenderer.h>

CGnuPlotKey::
CGnuPlotKey(CGnuPlotGroup *group) :
 group_(group)
{
}

CGnuPlotKey::
~CGnuPlotKey()
{
}

CGnuPlot *
CGnuPlotKey::
app() const
{
  return group_->app();
}

void
CGnuPlotKey::
draw()
{
  if (! isDisplayed()) return;

  CGnuPlotRenderer *renderer = app()->renderer();

  if (getFont().isValid())
    renderer->setFont(getFont());

  //---

  CBBox2D rbbox = (isOutside() ? group_->getRegionBBox() : renderer->range());

  //---

  CFontPtr font = renderer->getFont();

  double font_size = font->getCharAscent() + font->getCharDescent();

  double pw = renderer->pixelWidthToWindowWidth  (1);
  double ph = renderer->pixelHeightToWindowHeight(1);

  double bx = 8*pw;
  double by = 8*ph;
  double ll = 64*pw;

  CSize2D size;

  double textWidth = 0.0, textHeight = 0.0;

  std::string header;

  if (showTitle()) {
    header = getTitle();

    if (header != "")
      textHeight += font_size*ph;
  }

  for (auto plot : group_->plots()) {
    std::string label = plot->keyTitle();

    textWidth = std::max(textWidth, font->getStringWidth(label)*pw);

    textHeight += font_size*ph;
  }

  size = CSize2D(textWidth + ll + 3*bx, textHeight + 2*by);

  CHAlignType halign = getHAlign();
  CVAlignType valign = getVAlign();

  double x1 = 0, y1 = 0;

  if      (halign == CHALIGN_TYPE_LEFT)
    x1 = rbbox.getLeft () + bx;
  else if (halign == CHALIGN_TYPE_RIGHT)
    x1 = rbbox.getRight() - bx - size.width;
  else if (halign == CHALIGN_TYPE_CENTER)
    x1 = rbbox.getXMid() - size.width/2;

  if      (valign == CVALIGN_TYPE_TOP)
    y1 = rbbox.getTop   () - by - size.height;
  else if (valign == CVALIGN_TYPE_BOTTOM)
    y1 = rbbox.getBottom() + by;
  else if (valign == CVALIGN_TYPE_CENTER)
    y1 = rbbox.getYMid() - size.height/2;

  double x2 = x1 + size.width;
  double y2 = y1 + size.height;

  bbox_ = CBBox2D(x1, y1, x2, y2);

  if (getFillBox()) {
    CRGBA bg = group_->window()->backgroundColor();

    renderer->fillRect(bbox_, bg);
  }

  if (getDrawBox()) {
    CRGBA c(0, 0, 0);

    if (hasLineType())
      c = CGnuPlotStyleInst->indexColor(getLineType());

    renderer->drawRect(bbox_, c);
  }

  double y = y2 - by;

  if (header != "") {
    renderer->drawHAlignedText(CPoint2D((x1 + x2)/2, y), CHALIGN_TYPE_CENTER, 0,
                               CVALIGN_TYPE_TOP, 0, header, CRGBA(0,0,0));

    y -= font_size*ph;
  }

  for (auto plot : group_->plots()) {
    CGnuPlot::PlotStyle      style     = plot->getStyle();
    const CGnuPlotLineStyle &lineStyle = plot->lineStyle();

    double xx = (isReverse() ? x1 + bx : x2 - ll - bx);
    double yy = y - font_size*ph/2;

    if (group_->hasPlotStyle(CGnuPlot::PlotStyle::HISTOGRAMS) ||
        style == CGnuPlot::PlotStyle::BOXES || style == CGnuPlot::PlotStyle::CANDLESTICKS) {
      double h = (font_size - 4)*ph;

      CBBox2D hbbox(xx, yy - h/2, xx + ll, yy + h/2);

      if      (plot->fillStyle().style() == CGnuPlotPlot::FillType::PATTERN)
        renderer->patternRect(hbbox, plot->fillStyle().pattern(), lineStyle.color(CRGBA(0,0,0)));
      else if (plot->fillStyle().style() == CGnuPlotPlot::FillType::SOLID)
        renderer->fillRect(hbbox, lineStyle.color(CRGBA(1,1,1)));

      renderer->drawRect(hbbox, lineStyle.color(CRGBA(0,0,0)));
    }
    else if (style == CGnuPlot::PlotStyle::VECTORS) {
      CGnuPlotArrow arrow(group_);

      arrow.setStyle(plot->arrowStyle());

      arrow.setFrom(CPoint2D(xx, yy));
      arrow.setTo  (CPoint2D(xx + ll, yy));

      arrow.draw();
    }
    else {
      if      (style == CGnuPlot::PlotStyle::LINES ||
               style == CGnuPlot::PlotStyle::LINES_POINTS ||
               style == CGnuPlot::PlotStyle::IMPULSES ||
               style == CGnuPlot::PlotStyle::XYERRORBARS) {
        CPoint2D p1(xx, yy), p2(xx + ll, yy), pm(xx + ll/2, yy);

        renderer->drawLine(p1, p2, lineStyle.width(), lineStyle.color(CRGBA(1,0,0)));

        if      (style == CGnuPlot::PlotStyle::LINES_POINTS)
          renderer->drawSymbol(pm, plot->pointType(), plot->pointSize(),
                               lineStyle.color(CRGBA(1,0,0)));
        else if (style == CGnuPlot::PlotStyle::XYERRORBARS) {
          CPoint2D dy(0, 3*ph);

          renderer->drawLine(p1 - dy, p1 + dy, lineStyle.width(), lineStyle.color(CRGBA(1,0,0)));
          renderer->drawLine(p2 - dy, p2 + dy, lineStyle.width(), lineStyle.color(CRGBA(1,0,0)));
        }
      }
      else if (style == CGnuPlot::PlotStyle::POINTS ||
               style == CGnuPlot::PlotStyle::LINES_POINTS) {
        CPoint2D pm(xx + ll/2, yy);

        renderer->drawSymbol(pm, plot->pointType(), plot->pointSize(),
                             lineStyle.color(CRGBA(1,0,0)));
      }
      else if (style == CGnuPlot::PlotStyle::FILLEDCURVES) {
        double h = 4*ph;

        CBBox2D cbbox(xx, yy - h, xx + ll, yy + h);
        renderer->fillRect(cbbox, lineStyle.color(CRGBA(1,1,1)));
      }
      else {
        CPoint2D p1(xx, yy), p2(xx + ll, yy);

        renderer->drawLine(p1, p2, lineStyle.width(), lineStyle.color(CRGBA(1,0,0)));
      }
    }

    std::string label = plot->keyTitle();

    //double lw = font->getStringWidth(label);

    CRGBA tc = CRGBA(0,0,0);

    if      (isVariableTextColor())
      tc = lineStyle.color(tc);
    else if (isIndexTextColor())
      tc = CGnuPlotStyleInst->indexColor(textColorIndex());
    else if (isRGBTextColor())
      tc = textColorRGB();

    if (isReverse())
      renderer->drawHAlignedText(CPoint2D(xx + ll + bx, y), CHALIGN_TYPE_LEFT, 0,
                                 CVALIGN_TYPE_TOP, 0, label, tc);
    else
      renderer->drawHAlignedText(CPoint2D(xx - bx, y), CHALIGN_TYPE_RIGHT, 0,
                                 CVALIGN_TYPE_TOP, 0, label, tc);

    y -= font_size*ph;
  }
}
