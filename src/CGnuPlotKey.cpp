#include <CGnuPlotKey.h>
#include <CGnuPlotGroup.h>
#include <CGnuPlotPlot.h>
#include <CGnuPlotWindow.h>
#include <CGnuPlotRenderer.h>
#include <CGnuPlotStyleBase.h>

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
draw(CGnuPlotRenderer *renderer)
{
  if (! isDisplayed()) return;

  if (group_->hasPlotStyle(CGnuPlotTypes::PlotStyle::HISTOGRAMS)) {
    CGnuPlotTypes::HistogramStyle hstyle = group_->getHistogramData().style();

    if      (hstyle == CGnuPlotTypes::HistogramStyle::CLUSTERED) {
      drawClustered(renderer);
      return;
    }
    else if (hstyle == CGnuPlotTypes::HistogramStyle::COLUMNSTACKED) {
      drawColumnStacked(renderer);
      return;
    }
    else if (hstyle == CGnuPlotTypes::HistogramStyle::ROWSTACKED) {
      drawClustered(renderer);
      return;
    }
  }

  //---

  if (getFont().isValid())
    renderer->setFont(getFont());

  //---

  CFontPtr font = renderer->getFont();

  double font_size = font->getCharAscent() + font->getCharDescent();

  double pw = renderer->pixelWidthToWindowWidth  (1);
  double ph = renderer->pixelHeightToWindowHeight(1);

  double bx = 8*pw;
  double by = 8*ph;
  double ll = 64*pw;

  //---

  // calc key size
  CSize2D size;

  double textWidth = 0.0, textHeight = 0.0;

  std::string header;

  if (showTitle()) {
    header = getTitle();

    if (header != "") {
      textWidth = std::max(textWidth, font->getStringWidth(header)*pw);

      textHeight += font_size*ph;
    }
  }

  //---

  for (auto plot : group_->plots()) {
    if (! plot->isKeyTitleEnabled()) continue;

    CGnuPlotPlot::KeyLabels labels;

    plot->getKeyLabels(labels);

    for (const auto &l : labels) {
      textWidth = std::max(textWidth, font->getStringWidth(l.text())*pw);

      textHeight += font_size*ph;
    }
  }

  size = CSize2D(textWidth + ll + 3*bx, textHeight + 2*by);

  //---

  calcBBox(renderer, size);

  //---

  drawBox(renderer);

  //---

  double x1 = bbox_.getXMin();
  double x2 = bbox_.getXMax();
  double y2 = bbox_.getYMax();

  double y = y2 - by;

  if (header != "") {
    renderer->drawHAlignedText(CPoint2D((x1 + x2)/2, y), CHALIGN_TYPE_CENTER, 0,
                               CVALIGN_TYPE_TOP, 0, header, CRGBA(0,0,0));

    y -= font_size*ph;
  }

  clearPlotRects();

  for (auto plot : group_->plots()) {
    if (! plot->isKeyTitleEnabled()) continue;

    CGnuPlotTypes::PlotStyle plotStyle = plot->getStyle();

    CGnuPlotStyleBase *style = app()->getPlotStyle(plotStyle);

    const CGnuPlotLineStyle       &lineStyle = plot->lineStyle();
    const CGnuPlotTypes::FillType &fillType  = plot->fillType();

    double yp1 = y;

    CGnuPlotPlot::KeyLabels labels;

    plot->getKeyLabels(labels);

    for (const auto &l : labels) {
      double xx = (isReverse() ? x1 + bx : x2 - ll - bx);
      double yy = y - font_size*ph/2;

      CPoint2D p1(xx     , yy);
      CPoint2D p2(xx + ll, yy);

      // draw key line
      if      (l.color().isValid()) {
        CRGBA c = l.color().getValue();

        renderer->drawLine(p1, p2, lineStyle.calcWidth(), c, lineStyle.calcDash());
      }
      else if (style && style->hasKeyLine()) {
        style->drawKeyLine(plot, renderer, p1, p2);
      }
      else if (group_->hasPlotStyle(CGnuPlotTypes::PlotStyle::HISTOGRAMS)) {
        CRGBA c = (fillType == CGnuPlotPlot::FillType::PATTERN ? CRGBA(0,0,0) : CRGBA(1,1,1));

        CRGBA lc = lineStyle.calcColor(group_, c);
        CRGBA fc = lineStyle.calcColor(group_, CRGBA(0,0,0));
        CRGBA bg = group_->window()->backgroundColor();

        double h = (font_size - 4)*ph;

        CBBox2D hbbox(xx, yy - h/2, xx + ll, yy + h/2);

        if      (fillType == CGnuPlotPlot::FillType::PATTERN)
          renderer->patternRect(hbbox, plot->fillStyle().pattern(), fc, bg);
        else if (fillType == CGnuPlotPlot::FillType::SOLID)
          renderer->fillRect(hbbox, fc);

        if (plot->fillStyle().hasBorder()) {
          CRGBA lc1 = lc;

          plot->fillStyle().calcColor(group_, lc1);

          renderer->drawRect(hbbox, lc1, 1);
        }
      }
      else {
        CRGBA c = lineStyle.calcColor(group_, CRGBA(1,0,0));

        renderer->drawLine(p1, p2, lineStyle.calcWidth(), c, lineStyle.calcDash());
      }

      // draw key text
      CRGBA tc = CRGBA(0,0,0);

      if      (isVariableTextColor())
        tc = lineStyle.calcColor(group_, tc);
      else if (isIndexTextColor())
        tc = CGnuPlotStyleInst->indexColor(textColorIndex());
      else if (isRGBTextColor())
        tc = textColorRGB();

      if (! plot->isDisplayed())
        tc.setAlpha(0.5);

      if (isReverse())
        renderer->drawHAlignedText(CPoint2D(xx + ll + bx, y), CHALIGN_TYPE_LEFT, 0,
                                   CVALIGN_TYPE_TOP, 0, l.text(), tc);
      else
        renderer->drawHAlignedText(CPoint2D(xx - bx, y), CHALIGN_TYPE_RIGHT, 0,
                                   CVALIGN_TYPE_TOP, 0, l.text(), tc);

      y -= font_size*ph;
    }

    double yp2 = y;

    addPlotRect(plot, CBBox2D(x1, yp1, x2, yp2));

    //renderer->drawRect(bbox, CRGBA(1,0,0), 1);
  }

  group_->updateKeyBBox(bbox_);
}

void
CGnuPlotKey::
drawClustered(CGnuPlotRenderer *renderer)
{
  if (getFont().isValid())
    renderer->setFont(getFont());

  //---

  CFontPtr font = renderer->getFont();

  double font_size = font->getCharAscent() + font->getCharDescent();

  double pw = renderer->pixelWidthToWindowWidth  (1);
  double ph = renderer->pixelHeightToWindowHeight(1);

  double bx = 8*pw;
  double by = 8*ph;
  double ll = 64*pw;

  //---

  CGnuPlot::Plots plots;

  group_->getPlotsOfStyle(plots, CGnuPlot::PlotStyle::HISTOGRAMS);

  if (plots.empty())
    return;

  std::map<int,CGnuPlot::Plots> nplots;

  for (auto plot : plots)
    nplots[plot->newHistogramId()].push_back(plot);

  //---

  // calc key size
  CSize2D                  size;
  std::vector<CSize2D>     sizes;
  std::vector<std::string> titles;

  for (auto nplot : nplots) {
    int                    id     = nplot.first;
    const CGnuPlot::Plots &plots1 = nplot.second;

    std::string title;

    if (id >= 0) {
      const CGnuPlotNewHistogramData &hdata = group_->newHistogramData(id);

      title = hdata.title();
    }

    double textWidth = 0.0, textHeight = 0.0;

    if (title != "") {
      textWidth = std::max(textWidth, font->getStringWidth(title)*pw);

      textHeight += font_size*ph;
    }

    for (auto plot1 : plots1) {
      CGnuPlotPlot::KeyLabels labels;

      plot1->getKeyLabels(labels);

      for (const auto &l : labels) {
        textWidth = std::max(textWidth, font->getStringWidth(l.text())*pw);

        textHeight += font_size*ph;
      }
    }

    CSize2D size1(textWidth + ll + 3*bx, textHeight + 2*by);

    sizes.push_back(size1);

    size.width  += size1.width;
    size.height  = std::max(size.height, size1.height);
  }

  //---

  calcBBox(renderer, size);

  //---

  drawBox(renderer);

  //---

  double x1 = bbox_.getXMin();
  double x2 = bbox_.getXMax();
  double y2 = bbox_.getYMax();

  int i = 0;

  clearPlotRects();

  for (auto nplot : nplots) {
    int                    id     = nplot.first;
    const CGnuPlot::Plots &plots1 = nplot.second;

    std::string title;

    if (id >= 0) {
      const CGnuPlotNewHistogramData &hdata = group_->newHistogramData(id);

      title = hdata.title();
    }

    if (! isReverse())
      x2 = x1 + sizes[i].width;
    else
      x1 = x2 - sizes[i].width;

    double y = y2 - by;

    if (title != "") {
      renderer->drawHAlignedText(CPoint2D((x1 + x2)/2, y), CHALIGN_TYPE_CENTER, 0,
                                 CVALIGN_TYPE_TOP, 0, title, CRGBA(0,0,0));

      y -= font_size*ph;
    }

    for (auto plot1 : plots1) {
      const CGnuPlotLineStyle       &lineStyle = plot1->lineStyle();
      const CGnuPlotTypes::FillType &fillType  = plot1->fillType();

      double yp1 = y;

      CGnuPlotPlot::KeyLabels labels;

      plot1->getKeyLabels(labels);

      for (const auto &l : labels) {
        double xx = (isReverse() ? x1 + bx : x2 - ll - bx);
        double yy = y - font_size*ph/2;

        CPoint2D p1(xx     , yy);
        CPoint2D p2(xx + ll, yy);

        CRGBA c = (fillType == CGnuPlotPlot::FillType::PATTERN ? CRGBA(0,0,0) : CRGBA(1,1,1));

        CRGBA lc = lineStyle.calcColor(group_, c);
        CRGBA fc = lineStyle.calcColor(group_, CRGBA(0,0,0));
        CRGBA bg = group_->window()->backgroundColor();

        double h = (font_size - 4)*ph;

        CBBox2D hbbox(xx, yy - h/2, xx + ll, yy + h/2);

        if      (fillType == CGnuPlotPlot::FillType::PATTERN)
          renderer->patternRect(hbbox, plot1->fillStyle().pattern(), fc, bg);
        else if (fillType == CGnuPlotPlot::FillType::SOLID)
          renderer->fillRect(hbbox, fc);

        if (plot1->fillStyle().hasBorder()) {
          CRGBA lc1 = lc;

          plot1->fillStyle().calcColor(group_, lc1);

          renderer->drawRect(hbbox, lc1, 1);
        }

        // draw key text
        CRGBA tc = CRGBA(0,0,0);

        if      (isVariableTextColor())
          tc = lineStyle.calcColor(group_, tc);
        else if (isIndexTextColor())
          tc = CGnuPlotStyleInst->indexColor(textColorIndex());
        else if (isRGBTextColor())
          tc = textColorRGB();

        if (! plot1->isDisplayed())
          tc.setAlpha(0.5);

        if (isReverse())
          renderer->drawHAlignedText(CPoint2D(xx + ll + bx, y), CHALIGN_TYPE_LEFT, 0,
                                     CVALIGN_TYPE_TOP, 0, l.text(), tc);
        else
          renderer->drawHAlignedText(CPoint2D(xx - bx, y), CHALIGN_TYPE_RIGHT, 0,
                                     CVALIGN_TYPE_TOP, 0, l.text(), tc);

        y -= font_size*ph;
      }

      double yp2 = y;

      addPlotRect(plot1, CBBox2D(x1, yp1, x2, yp2));

    //renderer->drawRect(bbox, CRGBA(1,0,0), 1);
    }

    if (! isReverse())
      x1 = x2;
    else
      x2 = x1;

    ++i;
  }

  //---

  group_->updateKeyBBox(bbox_);
}

void
CGnuPlotKey::
drawColumnStacked(CGnuPlotRenderer *renderer)
{
  if (getFont().isValid())
    renderer->setFont(getFont());

  //---

  CFontPtr font = renderer->getFont();

  double font_size = font->getCharAscent() + font->getCharDescent();

  double pw = renderer->pixelWidthToWindowWidth  (1);
  double ph = renderer->pixelHeightToWindowHeight(1);

  double bx = 8*pw;
  double by = 8*ph;
  double ll = 64*pw;

  //---

  CGnuPlot::Plots plots;

  group_->getPlotsOfStyle(plots, CGnuPlot::PlotStyle::HISTOGRAMS);

  if (plots.empty())
    return;

  std::map<int,CGnuPlot::Plots> nplots;

  for (auto plot : plots)
    nplots[plot->newHistogramId()].push_back(plot);

  //---

  // calc key size
  double textWidth = 0.0, textHeight = 0.0;

  std::string header;

  if (showTitle()) {
    header = getTitle();

    if (header != "") {
      textWidth = std::max(textWidth, font->getStringWidth(header)*pw);

      textHeight += font_size*ph;
    }
  }

  for (auto nplot : nplots) {
    const CGnuPlot::Plots &plots1 = nplot.second;

    CGnuPlotPlot *plot = *plots1.begin();

    for (uint i = 0; i < plot->numPoints2D(); ++i) {
      //const CGnuPlotPoint &point = plot->getPoint2D(i);

      std::string pointStr = keyData_.pointLabel(i);

      if (pointStr == "")
        pointStr = CStrUtil::strprintf("%d", i);

      textWidth = std::max(textWidth, font->getStringWidth(pointStr)*pw);

      textHeight += font_size*ph;
    }
  }

  CSize2D size(textWidth + ll + 3*bx, textHeight + 2*by);

  //---

  calcBBox(renderer, size);

  //---

  drawBox(renderer);

  //---

  double x1 = bbox_.getXMin();
  double x2 = bbox_.getXMax();
  double y2 = bbox_.getYMax();

  double y = y2 - by;

  if (header != "") {
    renderer->drawHAlignedText(CPoint2D((x1 + x2)/2, y), CHALIGN_TYPE_CENTER, 0,
                               CVALIGN_TYPE_TOP, 0, header, CRGBA(0,0,0));

    y2 -= font_size*ph;
  }

  int pi = 0;

  for (auto nplot : nplots) {
    const CGnuPlot::Plots &plots1 = nplot.second;

    CGnuPlotPlot *plot = *plots1.begin();

    //---

    double y = y2 - by;

    clearPlotRects();

    for (uint i = 0; i < plot->numPoints2D(); ++i) {
      double xx = (isReverse() ? x1 + bx : x2 - ll - bx);
      double yy = y - font_size*ph/2;

      //const CGnuPlotPoint &point = plot->getPoint2D(i);

      std::string pointStr = keyData_.pointLabel(i);

      if (pointStr == "")
        pointStr = CStrUtil::strprintf("%d", i);

      CRGBA lc = CRGBA(0,0,0);
      CRGBA fc = CGnuPlotStyleInst->indexColor(i + 1);
      CRGBA bg = group_->window()->backgroundColor();

      double h = (font_size - 4)*ph;

      CBBox2D hbbox(xx, yy - h/2, xx + ll, yy + h/2);

      renderer->fillRect(hbbox, fc);

      if (plot->fillStyle().hasBorder()) {
        CRGBA lc1 = lc;

        plot->fillStyle().calcColor(group_, lc1);

        renderer->drawRect(hbbox, lc1, 1);
      }

      // draw key text
      CRGBA tc = CRGBA(0,0,0);

      if (isReverse())
        renderer->drawHAlignedText(CPoint2D(xx + ll + bx, y), CHALIGN_TYPE_LEFT, 0,
                                   CVALIGN_TYPE_TOP, 0, pointStr, tc);
      else
        renderer->drawHAlignedText(CPoint2D(xx - bx, y), CHALIGN_TYPE_RIGHT, 0,
                                   CVALIGN_TYPE_TOP, 0, pointStr, tc);

      y -= font_size*ph;
    }

    ++pi;
  }

  //---

  group_->updateKeyBBox(bbox_);
}

void
CGnuPlotKey::
drawBox(CGnuPlotRenderer *renderer)
{
  if (getFillBox()) {
    CRGBA fc = group_->window()->backgroundColor();

    if (boxLineStyle().isValid()) {
      CGnuPlotLineStyleP lineStyle = app()->getLineStyleInd(boxLineStyle().getValue());

      fc = lineStyle->calcColor(group_, fc);
    }

    renderer->fillRect(bbox_, fc);
  }

  if (getDrawBox()) {
    CRGBA lc(0, 0, 0);

    if      (boxLineType().isValid())
      lc = CGnuPlotStyleInst->indexColor(boxLineType().getValue());
    else if (boxLineStyle().isValid()) {
      CGnuPlotLineStyleP lineStyle = app()->getLineStyleInd(boxLineStyle().getValue());

      lc = lineStyle->calcColor(group_, lc);
    }

    double lw = boxLineWidth();

    renderer->drawRect(bbox_, lc, lw);
  }
}

void
CGnuPlotKey::
calcBBox(CGnuPlotRenderer *renderer, const CSize2D &size)
{
  double pw = renderer->pixelWidthToWindowWidth  (1);
  double ph = renderer->pixelHeightToWindowHeight(1);

  double bx = 8*pw;
  double by = 8*ph;

  CHAlignType halign = getHAlign();
  CVAlignType valign = getVAlign();

  double x1 = 0, y1 = 0;

  if (keyData_.position().isValid()) {
    CPoint2D p = keyData_.position().getValue().getPoint2D(renderer);

    if      (halign == CHALIGN_TYPE_LEFT)
      x1 = p.x;
    else if (halign == CHALIGN_TYPE_RIGHT)
      x1 = p.x - size.width;
    else if (halign == CHALIGN_TYPE_CENTER)
      x1 = p.x - size.width/2;

    if      (valign == CVALIGN_TYPE_TOP)
      y1 = p.y - size.height;
    else if (valign == CVALIGN_TYPE_BOTTOM)
      y1 = p.y;
    else if (valign == CVALIGN_TYPE_CENTER)
      y1 = p.y - size.height/2;
  }
  else if (inMargin()) {
#if 0
    double px1, py1, px2, py2;

    renderer->pixelToWindow(                    0,                      0, &px1, &py2);
    renderer->pixelToWindow(renderer->width() - 1, renderer->height() - 1, &px2, &py1);

    CBBox2D rbbox(px1, py1, px2, py2);

    if (isLMargin() || isRMargin()) {
      x1 = (isLMargin() ? rbbox.getXMin() + bx : rbbox.getXMax() - size.width - bx);

      if      (valign == CVALIGN_TYPE_TOP)
        y1 = rbbox.getYMax() - by - size.height;
      else if (valign == CVALIGN_TYPE_BOTTOM)
        y1 = rbbox.getYMin() + by;
      else if (valign == CVALIGN_TYPE_CENTER)
        y1 = (rbbox.getYMin() + rbbox.getYMax())/2 - size.height/2;
    }
    else {
      y1 = (isBMargin() ? rbbox.getYMin() + by : rbbox.getYMax() - size.height - by);

      if      (halign == CHALIGN_TYPE_LEFT)
        x1 = rbbox.getXMin() + bx;
      else if (halign == CHALIGN_TYPE_RIGHT)
        x1 = rbbox.getXMax() - bx - size.width;
      else if (halign == CHALIGN_TYPE_CENTER)
        x1 = (rbbox.getXMin() + rbbox.getXMax())/2 - size.width/2;
    }
#else
    CBBox2D rbbox = group_->getAxisBBox();

    if (isLMargin() || isRMargin()) {
      x1 = (isLMargin() ? rbbox.getXMin() - size.width - bx : rbbox.getXMax() + bx);

      if      (valign == CVALIGN_TYPE_TOP)
        y1 = rbbox.getYMax() + by + size.height;
      else if (valign == CVALIGN_TYPE_BOTTOM)
        y1 = rbbox.getYMin() - by;
      else if (valign == CVALIGN_TYPE_CENTER)
        y1 = (rbbox.getYMin() + rbbox.getYMax())/2 - size.height/2;
    }
    else {
      y1 = (isBMargin() ? rbbox.getYMin() - size.height - by : rbbox.getYMax() + by);

      if      (halign == CHALIGN_TYPE_LEFT)
        x1 = rbbox.getXMin() - size.width - bx;
      else if (halign == CHALIGN_TYPE_RIGHT)
        x1 = rbbox.getXMax() + bx;
      else if (halign == CHALIGN_TYPE_CENTER)
        x1 = (rbbox.getXMin() + rbbox.getXMax())/2 - size.width/2;
    }
#endif
  }
  else {
    if (! isOutside()) {
      CBBox2D rbbox = renderer->range();

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
    }
    else {
      CBBox2D rbbox = group_->getAxisBBox();

      // vertical: place outside left/right
      if (vertical()) {
        if      (halign == CHALIGN_TYPE_LEFT)
          x1 = rbbox.getLeft () - bx - size.width;
        else if (halign == CHALIGN_TYPE_RIGHT)
          x1 = rbbox.getRight() + bx;
        else if (halign == CHALIGN_TYPE_CENTER)
          x1 = rbbox.getXMid() - size.width/2;

        if      (valign == CVALIGN_TYPE_TOP)
          y1 = rbbox.getTop   () - by - size.height;
        else if (valign == CVALIGN_TYPE_BOTTOM)
          y1 = rbbox.getBottom() + by;
        else if (valign == CVALIGN_TYPE_CENTER)
          y1 = rbbox.getYMid() - size.height/2;
      }
      // horizontal: place outside top/bottom
      else {
        if      (halign == CHALIGN_TYPE_LEFT)
          x1 = rbbox.getLeft () + bx;
        else if (halign == CHALIGN_TYPE_RIGHT)
          x1 = rbbox.getRight() - bx - size.width;
        else if (halign == CHALIGN_TYPE_CENTER)
          x1 = rbbox.getXMid() - size.width/2;

        if      (valign == CVALIGN_TYPE_TOP)
          y1 = rbbox.getTop   () + by;
        else if (valign == CVALIGN_TYPE_BOTTOM)
          y1 = rbbox.getBottom() - by - size.height;
        else if (valign == CVALIGN_TYPE_CENTER)
          y1 = rbbox.getYMid() - size.height/2;
      }
    }
  }

  double x2 = x1 + size.width;
  double y2 = y1 + size.height;

  bbox_ = CBBox2D(x1, y1, x2, y2);
}

int
CGnuPlotKey::
plotAtPos(const CPoint2D &pos) const
{
  for (const auto &p : prects_) {
    if (p.second.inside(pos))
      return p.first;
  }

  return -1;
}

void
CGnuPlotKey::
addPlotRect(CGnuPlotPlot *plot, const CBBox2D &rect)
{
  prects_[plot->id()] = rect;
}

bool
CGnuPlotKey::
inside(const CGnuPlotTypes::InsideData &data) const
{
  return bbox_.inside(data.window);
}

