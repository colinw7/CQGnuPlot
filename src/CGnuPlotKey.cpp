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

  renderer_ = renderer;

  if (group_->hasPlotStyle(CGnuPlotTypes::PlotStyle::HISTOGRAMS)) {
    CGnuPlotTypes::HistogramStyle hstyle = group_->getHistogramData().style();

    if      (hstyle == CGnuPlotTypes::HistogramStyle::CLUSTERED) {
      drawClustered();
      return;
    }
    else if (hstyle == CGnuPlotTypes::HistogramStyle::COLUMNSTACKED) {
      drawColumnStacked();
      return;
    }
    else if (hstyle == CGnuPlotTypes::HistogramStyle::ROWSTACKED) {
      drawClustered();
      return;
    }
  }

  //---

  if (getFont())
    renderer_->setFont(getFont());

  //---

  CFontPtr font = renderer_->getFont();

  double pw = renderer_->pixelWidthToWindowWidth  (1);
  double ph = renderer_->pixelHeightToWindowHeight(1);

  double bx = 8*pw;
  double by = 8*ph;
  double ll = keyData_.sampLen().getValue(4  )*renderer_->charPixelWidth ()*pw;
  double ls = keyData_.spacing().getValue(1.5)*renderer_->charPixelHeight()*ph;

  //---

  // calc key size
  CSize2D size;

  double textWidth = 0.0, textHeight = 0.0;

  std::string header;

  if (hasTitle()) {
    header = this->title();

    if (header != "") {
      textWidth = std::max(textWidth, font->getStringWidth(header)*pw);

      textHeight += ls;
    }
  }

  //---

  for (auto plot : group_->plots()) {
    if (! plot->isKeyTitleEnabled()) continue;

    CGnuPlotPlot::KeyLabels labels;

    plot->getKeyLabels(labels);

    for (const auto &l : labels) {
      textWidth = std::max(textWidth, font->getStringWidth(l.text())*pw);

      textHeight += ls;
    }
  }

  size = CSize2D(textWidth + ll + 3*bx, textHeight + 2*by);

  //---

  calcBBox(size);

  //---

  drawBox();

  //---

  double x1 = bbox_.getXMin();
  double x2 = bbox_.getXMax();
  double y2 = bbox_.getYMax();

  double y = y2 - by;

  if (header != "") {
    CPoint2D pos((x1 + x2)/2, y);

    drawHAlignedText(pos, HAlignPos(CHALIGN_TYPE_CENTER, 0),
                     VAlignPos(CVALIGN_TYPE_TOP, 0), header, CRGBA(0,0,0));

    y -= ls;
  }

  clearPlotRects();

  for (auto plot : group_->plots()) {
    if (! plot->isKeyTitleEnabled()) continue;

    CGnuPlotTypes::PlotStyle plotStyle = plot->style();

    CGnuPlotStyleBase *style = app()->getPlotStyle(plotStyle);

    const CGnuPlotLineStyle &lineStyle = plot->lineStyle();
    const CGnuPlotFillStyle &fillStyle = plot->fillStyle();

    CGnuPlotFill   fill  (plot.get());
    CGnuPlotStroke stroke(plot.get());

    //---

    double yp1 = y;

    CGnuPlotPlot::KeyLabels labels;

    plot->getKeyLabels(labels);

    for (const auto &l : labels) {
      double xx = (isReverse() ? x1 + bx : x2 - ll - bx);
      double yy = y - ls/2;

      CPoint2D p1(xx     , yy);
      CPoint2D p2(xx + ll, yy);

      // draw key line
      if      (l.color().isValid()) {
        CRGBA c = l.color().getValue();

        renderer_->drawLine(p1, p2, c, stroke.width(), stroke.lineDash());
      }
      else if (style && style->hasKeyLine()) {
        style->drawKeyLine(plot.get(), renderer_, p1, p2);
      }
      else if (group_->hasPlotStyle(CGnuPlotTypes::PlotStyle::HISTOGRAMS)) {
        CRGBA lc = stroke.color();

        double h = (renderer_->charPixelHeight() - 4)*ph;

        CBBox2D hbbox(xx, yy - h/2, xx + ll, yy + h/2);

        renderer_->fillRect(hbbox, fill);

        if (stroke.isEnabled()) {
          CRGBA lc1 = lc;

          fillStyle.calcColor(group_, lc1);

          renderer_->drawRect(hbbox, lc1, 1);
        }
      }
      else {
        CRGBA c = stroke.color();

        renderer_->drawLine(p1, p2, c, stroke.width(), stroke.lineDash());
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
        drawHAlignedText(CPoint2D(xx + ll + bx, y), HAlignPos(CHALIGN_TYPE_LEFT, 0),
                         VAlignPos(CVALIGN_TYPE_TOP, 0), l.text(), tc);
      else
        drawHAlignedText(CPoint2D(xx - bx, y), HAlignPos(CHALIGN_TYPE_RIGHT, 0),
                         VAlignPos(CVALIGN_TYPE_TOP, 0), l.text(), tc);

      y -= ls;
    }

    double yp2 = y;

    addPlotRect(plot.get(), CBBox2D(x1, yp1, x2, yp2));

    //renderer_->drawRect(bbox, CRGBA(1,0,0), 1);
  }

  group_->updateKeyBBox(bbox_);
}

void
CGnuPlotKey::
drawClustered()
{
  if (getFont())
    renderer_->setFont(getFont());

  //---

  CFontPtr font = renderer_->getFont();

  double pw = renderer_->pixelWidthToWindowWidth  (1);
  double ph = renderer_->pixelHeightToWindowHeight(1);

  double bx = 8*pw;
  double by = 8*ph;
  double ll = keyData_.sampLen().getValue(4  )*renderer_->charPixelWidth ()*pw;
  double ls = keyData_.spacing().getValue(1.5)*renderer_->charPixelHeight()*ph;

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

      textHeight += ls;
    }

    for (auto plot1 : plots1) {
      CGnuPlotPlot::KeyLabels labels;

      plot1->getKeyLabels(labels);

      for (const auto &l : labels) {
        textWidth = std::max(textWidth, font->getStringWidth(l.text())*pw);

        textHeight += ls;
      }
    }

    CSize2D size1(textWidth + ll + 3*bx, textHeight + 2*by);

    sizes.push_back(size1);

    size.setWidth (size.getWidth() + size1.getWidth());
    size.setHeight(std::max(size.getHeight(), size1.getHeight()));
  }

  //---

  calcBBox(size);

  //---

  drawBox();

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
      x2 = x1 + sizes[i].getWidth();
    else
      x1 = x2 - sizes[i].getWidth();

    double y = y2 - by;

    if (title != "") {
      drawHAlignedText(CPoint2D((x1 + x2)/2, y), HAlignPos(CHALIGN_TYPE_CENTER, 0),
                       VAlignPos(CVALIGN_TYPE_TOP, 0), title, CRGBA(0,0,0));

      y -= ls;
    }

    for (auto plot1 : plots1) {
      const CGnuPlotLineStyle &lineStyle1 = plot1->lineStyle();
      const CGnuPlotFillStyle &fillStyle1 = plot1->fillStyle();

      CGnuPlotFill   fill1  (plot1.get());
      CGnuPlotStroke stroke1(plot1.get());

      double yp1 = y;

      CGnuPlotPlot::KeyLabels labels;

      plot1->getKeyLabels(labels);

      for (const auto &l : labels) {
        double xx = (isReverse() ? x1 + bx : x2 - ll - bx);
        double yy = y - ls/2;

        CPoint2D p1(xx     , yy);
        CPoint2D p2(xx + ll, yy);

        CRGBA lc = stroke1.color();

        double h = (renderer_->charPixelHeight() - 4)*ph;

        CBBox2D hbbox(xx, yy - h/2, xx + ll, yy + h/2);

        renderer_->fillRect(hbbox, fill1);

        if (stroke1.isEnabled()) {
          CRGBA lc1 = lc;

          fillStyle1.calcColor(group_, lc1);

          renderer_->drawRect(hbbox, lc1, 1);
        }

        // draw key text
        CRGBA tc = CRGBA(0,0,0);

        if      (isVariableTextColor())
          tc = lineStyle1.calcColor(group_, tc);
        else if (isIndexTextColor())
          tc = CGnuPlotStyleInst->indexColor(textColorIndex());
        else if (isRGBTextColor())
          tc = textColorRGB();

        if (! plot1->isDisplayed())
          tc.setAlpha(0.5);

        if (isReverse())
          drawHAlignedText(CPoint2D(xx + ll + bx, y), HAlignPos(CHALIGN_TYPE_LEFT, 0),
                           VAlignPos(CVALIGN_TYPE_TOP, 0), l.text(), tc);
        else
          drawHAlignedText(CPoint2D(xx - bx, y), HAlignPos(CHALIGN_TYPE_RIGHT, 0),
                           VAlignPos(CVALIGN_TYPE_TOP, 0), l.text(), tc);

        y -= ls;
      }

      double yp2 = y;

      addPlotRect(plot1.get(), CBBox2D(x1, yp1, x2, yp2));

    //renderer_->drawRect(bbox, CRGBA(1,0,0), 1);
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
drawColumnStacked()
{
  if (getFont())
    renderer_->setFont(getFont());

  //---

  CFontPtr font = renderer_->getFont();

  double pw = renderer_->pixelWidthToWindowWidth  (1);
  double ph = renderer_->pixelHeightToWindowHeight(1);

  double bx = 8*pw;
  double by = 8*ph;
  double ll = keyData_.sampLen().getValue(4  )*renderer_->charPixelWidth ()*pw;
  double ls = keyData_.spacing().getValue(1.5)*renderer_->charPixelHeight()*ph;

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

  if (hasTitle()) {
    header = this->title();

    if (header != "") {
      textWidth = std::max(textWidth, font->getStringWidth(header)*pw);

      textHeight += ls;
    }
  }

  for (auto nplot : nplots) {
    const CGnuPlot::Plots &plots1 = nplot.second;

    CGnuPlotPlotP plot = *plots1.begin();

    for (uint i = 0; i < plot->numPoints2D(); ++i) {
      //const CGnuPlotPoint &point = plot->getPoint2D(i);

      std::string pointStr = keyData_.pointLabel(i);

      if (pointStr == "")
        pointStr = CStrUtil::strprintf("%d", i);

      textWidth = std::max(textWidth, font->getStringWidth(pointStr)*pw);

      textHeight += ls;
    }
  }

  CSize2D size(textWidth + ll + 3*bx, textHeight + 2*by);

  //---

  calcBBox(size);

  //---

  drawBox();

  //---

  double x1 = bbox_.getXMin();
  double x2 = bbox_.getXMax();
  double y2 = bbox_.getYMax();

  double y = y2 - by;

  if (header != "") {
    drawHAlignedText(CPoint2D((x1 + x2)/2, y), HAlignPos(CHALIGN_TYPE_CENTER, 0),
                     VAlignPos(CVALIGN_TYPE_TOP, 0), header, CRGBA(0,0,0));

    y2 -= ls;
  }

  int pi = 0;

  for (auto nplot : nplots) {
    const CGnuPlot::Plots &plots1 = nplot.second;

    CGnuPlotPlotP plot = *plots1.begin();

    const CGnuPlotFillStyle &fillStyle = plot->fillStyle();

    CGnuPlotStroke stroke(plot.get());

    //---

    double y2b = y2 - by;

    clearPlotRects();

    for (uint i = 0; i < plot->numPoints2D(); ++i) {
      double xx = (isReverse() ? x1 + bx : x2 - ll - bx);
      double yy = y2b - ls/2;

      //const CGnuPlotPoint &point = plot->getPoint2D(i);

      std::string pointStr = keyData_.pointLabel(i);

      if (pointStr == "")
        pointStr = CStrUtil::strprintf("%d", i);

      CRGBA lc = CRGBA(0,0,0);
      CRGBA fc = CGnuPlotStyleInst->indexColor(i + 1);

      double h = (renderer_->charPixelHeight() - 4)*ph;

      CBBox2D hbbox(xx, yy - h/2, xx + ll, yy + h/2);

      renderer_->fillRect(hbbox, fc);

      if (stroke.isEnabled()) {
        CRGBA lc1 = lc;

        fillStyle.calcColor(group_, lc1);

        renderer_->drawRect(hbbox, lc1, 1);
      }

      // draw key text
      CRGBA tc = CRGBA(0,0,0);

      if (isReverse())
        drawHAlignedText(CPoint2D(xx + ll + bx, y2b), HAlignPos(CHALIGN_TYPE_LEFT, 0),
                         VAlignPos(CVALIGN_TYPE_TOP, 0), pointStr, tc);
      else
        drawHAlignedText(CPoint2D(xx - bx, y2b), HAlignPos(CHALIGN_TYPE_RIGHT, 0),
                         VAlignPos(CVALIGN_TYPE_TOP, 0), pointStr, tc);

      y2b -= ls;
    }

    ++pi;
  }

  //---

  group_->updateKeyBBox(bbox_);
}

void
CGnuPlotKey::
drawBox()
{
  if (getFillBox()) {
    CRGBA fc = group_->window()->backgroundColor();

    if (boxLineStyle().isValid()) {
      CGnuPlotLineStyleP lineStyle = app()->getLineStyleInd(boxLineStyle().getValue());

      fc = lineStyle->calcColor(group_, fc);
    }

    renderer_->fillRect(bbox_, fc);
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

    renderer_->drawRect(bbox_, lc, lw);
  }
}

void
CGnuPlotKey::
calcBBox(const CSize2D &size)
{
  double pw = renderer_->pixelWidthToWindowWidth  (1);
  double ph = renderer_->pixelHeightToWindowHeight(1);

  double bx = 8*pw;
  double by = 8*ph;

  CHAlignType halign = getHAlign();
  CVAlignType valign = getVAlign();

  double x1 = 0, y1 = 0;

  if (keyData_.position().isValid()) {
    CPoint2D p = keyData_.position().getValue().getPoint2D(renderer_);

    if      (halign == CHALIGN_TYPE_LEFT)
      x1 = p.x;
    else if (halign == CHALIGN_TYPE_RIGHT)
      x1 = p.x - size.getWidth();
    else if (halign == CHALIGN_TYPE_CENTER)
      x1 = p.x - size.getWidth()/2;

    if      (valign == CVALIGN_TYPE_TOP)
      y1 = p.y - size.getHeight();
    else if (valign == CVALIGN_TYPE_BOTTOM)
      y1 = p.y;
    else if (valign == CVALIGN_TYPE_CENTER)
      y1 = p.y - size.getHeight()/2;
  }
  else if (inMargin()) {
#if 0
    double px1, py1, px2, py2;

    renderer_->pixelToWindow(                     0,                       0, &px1, &py2);
    renderer_->pixelToWindow(renderer_->width() - 1, renderer_->height() - 1, &px2, &py1);

    CBBox2D rbbox(px1, py1, px2, py2);

    if (isLMargin() || isRMargin()) {
      x1 = (isLMargin() ? rbbox.getXMin() + bx : rbbox.getXMax() - size.getWidth() - bx);

      if      (valign == CVALIGN_TYPE_TOP)
        y1 = rbbox.getYMax() - by - size.getHeight();
      else if (valign == CVALIGN_TYPE_BOTTOM)
        y1 = rbbox.getYMin() + by;
      else if (valign == CVALIGN_TYPE_CENTER)
        y1 = (rbbox.getYMin() + rbbox.getYMax())/2 - size.getHeight()/2;
    }
    else {
      y1 = (isBMargin() ? rbbox.getYMin() + by : rbbox.getYMax() - size.getHeight() - by);

      if      (halign == CHALIGN_TYPE_LEFT)
        x1 = rbbox.getXMin() + bx;
      else if (halign == CHALIGN_TYPE_RIGHT)
        x1 = rbbox.getXMax() - bx - size.getWidth();
      else if (halign == CHALIGN_TYPE_CENTER)
        x1 = (rbbox.getXMin() + rbbox.getXMax())/2 - size.getWidth()/2;
    }
#else
    CBBox2D rbbox = group_->axisBBox2D();

    if (isLMargin() || isRMargin()) {
      x1 = (isLMargin() ? rbbox.getXMin() - size.getWidth() - bx : rbbox.getXMax() + bx);

      if      (valign == CVALIGN_TYPE_TOP)
        y1 = rbbox.getYMax() + by + size.getHeight();
      else if (valign == CVALIGN_TYPE_BOTTOM)
        y1 = rbbox.getYMin() - by;
      else if (valign == CVALIGN_TYPE_CENTER)
        y1 = (rbbox.getYMin() + rbbox.getYMax())/2 - size.getHeight()/2;
    }
    else {
      y1 = (isBMargin() ? rbbox.getYMin() - size.getHeight() - by : rbbox.getYMax() + by);

      if      (halign == CHALIGN_TYPE_LEFT)
        x1 = rbbox.getXMin() - size.getWidth() - bx;
      else if (halign == CHALIGN_TYPE_RIGHT)
        x1 = rbbox.getXMax() + bx;
      else if (halign == CHALIGN_TYPE_CENTER)
        x1 = (rbbox.getXMin() + rbbox.getXMax())/2 - size.getWidth()/2;
    }
#endif
  }
  else {
    if (! isOutside()) {
      CBBox2D rbbox = renderer_->range();

      if      (halign == CHALIGN_TYPE_LEFT)
        x1 = rbbox.getLeft () + bx;
      else if (halign == CHALIGN_TYPE_RIGHT)
        x1 = rbbox.getRight() - bx - size.getWidth();
      else if (halign == CHALIGN_TYPE_CENTER)
        x1 = rbbox.getXMid() - size.getWidth()/2;

      if      (valign == CVALIGN_TYPE_TOP)
        y1 = rbbox.getTop   () - by - size.getHeight();
      else if (valign == CVALIGN_TYPE_BOTTOM)
        y1 = rbbox.getBottom() + by;
      else if (valign == CVALIGN_TYPE_CENTER)
        y1 = rbbox.getYMid() - size.getHeight()/2;
    }
    else {
      CBBox2D rbbox = group_->axisBBox2D();

      // vertical: place outside left/right
      if (vertical()) {
        if      (halign == CHALIGN_TYPE_LEFT)
          x1 = rbbox.getLeft () - bx - size.getWidth();
        else if (halign == CHALIGN_TYPE_RIGHT)
          x1 = rbbox.getRight() + bx;
        else if (halign == CHALIGN_TYPE_CENTER)
          x1 = rbbox.getXMid() - size.getWidth()/2;

        if      (valign == CVALIGN_TYPE_TOP)
          y1 = rbbox.getTop   () - by - size.getHeight();
        else if (valign == CVALIGN_TYPE_BOTTOM)
          y1 = rbbox.getBottom() + by;
        else if (valign == CVALIGN_TYPE_CENTER)
          y1 = rbbox.getYMid() - size.getHeight()/2;
      }
      // horizontal: place outside top/bottom
      else {
        if      (halign == CHALIGN_TYPE_LEFT)
          x1 = rbbox.getLeft () + bx;
        else if (halign == CHALIGN_TYPE_RIGHT)
          x1 = rbbox.getRight() - bx - size.getWidth();
        else if (halign == CHALIGN_TYPE_CENTER)
          x1 = rbbox.getXMid() - size.getWidth()/2;

        if      (valign == CVALIGN_TYPE_TOP)
          y1 = rbbox.getTop   () + by;
        else if (valign == CVALIGN_TYPE_BOTTOM)
          y1 = rbbox.getBottom() - by - size.getHeight();
        else if (valign == CVALIGN_TYPE_CENTER)
          y1 = rbbox.getYMid() - size.getHeight()/2;
      }
    }
  }

  double x2 = x1 + size.getWidth ();
  double y2 = y1 + size.getHeight();

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
inside(const CGnuPlotMouseEvent &mouseEvent) const
{
  return bbox_.inside(mouseEvent.window());
}

void
CGnuPlotKey::
drawHAlignedText(const CPoint2D &pos, const HAlignPos &halignPos, const VAlignPos &valignPos,
                 const std::string &str, const CRGBA &c)
{
  if (isEnhanced()) {
    CBBox2D bbox, rbbox;

    renderer_->calcTextRectAtPoint(pos, str, true, halignPos, valignPos, 0.0, bbox, rbbox);

    renderer_->drawTextAtPoint(pos, str, true, halignPos, valignPos, c, 0.0);
  }
  else {
    renderer_->drawHAlignedText(pos, halignPos, valignPos, str, c);
  }
}
