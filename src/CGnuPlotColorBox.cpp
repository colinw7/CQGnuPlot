#include <CGnuPlotColorBox.h>
#include <CGnuPlotGroup.h>
#include <CGnuPlotRenderer.h>
#include <CGnuPlotUtil.h>

CGnuPlotColorBox::
CGnuPlotColorBox(CGnuPlotGroup *group) :
 group_(group), axis_(CGnuPlotTypes::AxisType::CB, 1)
{
}

CGnuPlotColorBox::
~CGnuPlotColorBox()
{
}

CGnuPlot *
CGnuPlotColorBox::
app() const
{
  return group_->app();
}

void
CGnuPlotColorBox::
draw(CGnuPlotRenderer *renderer)
{
  if (! isEnabled())
    return;

  CBBox2D irbbox = renderer->range();
  CBBox2D orbbox = group_->getRegionBBox();
  CBBox2D axbbox = group_->getAxisBBox();

  double pw = renderer->pixelWidthToWindowWidth  (1);
  double ph = renderer->pixelHeightToWindowHeight(1);

  double bx = 4*pw;
  double by = 4*ph;

  double dx = orbbox.getRight()  - irbbox.getRight () - 2*bx;
  double dy = irbbox.getBottom() - orbbox.getBottom() - 2*by;

  const CGnuPlotAxisData &cbaxis = group_->colorBox()->axis();

  double cbmin = cbaxis.min().getValue(0);
  double cbmax = cbaxis.max().getValue(1);

  // calc color box position
  double x1, y1, x2, y2, tx, ty;

  if (user_) {
    // screen coords
    CPoint2D origin = origin_.getPoint2D(renderer);

    x1 = CGnuPlotUtil::map(origin.x, 0, 1, orbbox.getXMin(), orbbox.getXMax());
    y1 = CGnuPlotUtil::map(origin.y, 0, 1, orbbox.getYMin(), orbbox.getYMax());
    x2 = x1 + CGnuPlotUtil::map(size_.width , 0, 1, 0, orbbox.getWidth ());
    y2 = y1 + CGnuPlotUtil::map(size_.height, 0, 1, 0, orbbox.getHeight());

    if (isVertical()) {
      tx = x2 + bx;
      ty = y1;
    }
    else {
      tx = x1;
      ty = y1 - by;
    }
  }
  else {
    if (isVertical()) {
      x1 = irbbox.getRight() + bx;
      y1 = irbbox.getBottom();
      x2 = x1 + dx/2;
      y2 = irbbox.getTop();

      tx = x2 + bx;
      ty = y1;
    }
    else {
      x1 = irbbox.getLeft();
      y2 = axbbox.getBottom() - by;
      x2 = irbbox.getRight();
      y1 = y2 - dy/2;

      tx = x1;
      ty = y1 - by;
    }
  }

  CPoint2D p1, p2;

  renderer->windowToPixel(CPoint2D(x1, y1), p1);
  renderer->windowToPixel(CPoint2D(x2, y2), p2);

  //---

  // draw colors
  if (isVertical()) {
    double h  = fabs(p2.y - p1.y);
    int    ih = int(h);

    for (int i = 0; i < ih; ++i) {
      CPoint2D pl1, pl2;

      renderer->pixelToWindow(CPoint2D(p1.x, p1.y - i), pl1);
      renderer->pixelToWindow(CPoint2D(p2.x, p1.y - i), pl2);

      double z = CGnuPlotUtil::map(i, 0, ih - 1, 0, 1);

      CColor c = group_->palette()->getColor(z);

      renderer->drawLine(pl1, pl2, 1, c.rgba());
    }
  }
  else {
    double w  = fabs(p2.x - p1.x);
    int    iw = int(w);

    for (int i = 0; i < iw; ++i) {
      CPoint2D pl1, pl2;

      renderer->pixelToWindow(CPoint2D(p1.x + i, p1.y), pl1);
      renderer->pixelToWindow(CPoint2D(p1.x + i, p2.y), pl2);

      double z = CGnuPlotUtil::map(i, 0, iw - 1, 0, 1);

      CColor c = group_->palette()->getColor(z);

      renderer->drawLine(pl1, pl2, 1, c.rgba());
    }
  }

  //---

  // draw tics
  if (! cbaxis.hasRTicLabels(0)) {
    double cbmin1, cbmax1;
    int    numTicks1, numTicks2;

    if (! CGnuPlotAxis::calcTics(cbmin, cbmax, cbmin1, cbmax1, numTicks1, numTicks2)) {
      cbmin1 = cbmin;
      cbmax1 = cbmax;

      numTicks1 = 10;
      numTicks2 = 0;
    }

    double dc = (cbmax1 - cbmin1)/numTicks1;
    double c  = cbmin1;

    if (isVertical()) {
      //double dy = (y2 - y1)/numTicks1;

      //double y = y1;

      for (int i = 0; i <= numTicks1; ++i) {
        double yy = CGnuPlotUtil::map(c, cbmin, cbmax, y1, y2);

        if (i != 0 && i != numTicks1) {
          CPoint2D pl1(x1     , yy);
          CPoint2D pl2(x1 + bx, yy);

          renderer->drawLine(pl1, pl2, 1, CRGBA(0,0,0));

          CPoint2D pl3(x2 - bx, yy);
          CPoint2D pl4(x2     , yy);

          renderer->drawLine(pl3, pl4, 1, CRGBA(0,0,0));
        }

        std::stringstream ss;

        ss << c;

        renderer->drawHAlignedText(CPoint2D(tx, yy), CHALIGN_TYPE_LEFT, 0, CVALIGN_TYPE_CENTER, 0,
                                   ss.str(), CRGBA(0,0,0));

        //y += dy;
        c += dc;
      }
    }
    else {
      //double dx = (x2 - x1)/numTicks1;

      //double x = x1;

      for (int i = 0; i <= numTicks1; ++i) {
        double xx = CGnuPlotUtil::map(c, cbmin, cbmax, x1, x2);

        if (i != 0 && i != numTicks1) {
          CPoint2D pl1(xx, y1     );
          CPoint2D pl2(xx, y1 + by);

          renderer->drawLine(pl1, pl2, 1, CRGBA(0,0,0));

          CPoint2D pl3(xx, y2 - by);
          CPoint2D pl4(xx, y2     );

          renderer->drawLine(pl3, pl4, 1, CRGBA(0,0,0));
        }

        std::stringstream ss;

        ss << c;

        renderer->drawHAlignedText(CPoint2D(xx, ty), CHALIGN_TYPE_CENTER, 0, CVALIGN_TYPE_TOP, 0,
                                   ss.str(), CRGBA(0,0,0));

        //x += dx;
        c += dc;
      }
    }
  }
  else {
    const CGnuPlotAxisData::RTicLabels &ticLabels = cbaxis.rticLabels(0);

    if (isVertical()) {
      for (const auto &label : ticLabels) {
        double      r = label.first;
        std::string s = label.second;

        double y = CGnuPlotUtil::map(r, cbmin, cbmax, y1, y2);

        renderer->drawText(CPoint2D(tx, y), s, CRGBA(0,0,0));
      }
    }
    else {
      for (const auto &label : ticLabels) {
        double      r = label.first;
        std::string s = label.second;

        double x = CGnuPlotUtil::map(r, cbmin, cbmax, x1, x2);

        renderer->drawText(CPoint2D(x, ty), s, CRGBA(0,0,0));
      }
    }
  }

  //---

  bbox_ = CBBox2D(CPoint2D(x1, y1), CPoint2D(x2, y2));

  //---

  // draw border

  if (border_) {
    CRGBA  c = CRGBA(0, 0, 0);
    double w = 1.0;

    if (borderStyle_ >= 0) {
      CGnuPlotLineStyleP lineStyle = group_->app()->getLineStyleInd(borderStyle_);

      c = lineStyle->calcColor(group_, c);
      w = lineStyle->calcWidth();
    }

    renderer->drawRect(bbox_, c, w);
  }

  //---

  // draw label

  if (cbaxis.text() != "") {
    if (isVertical()) {
      double ym = (y1 + y2)/2;

      renderer->drawRotatedText(CPoint2D(x2, ym), cbaxis.text(), cbaxis.labelRotate(),
                                CHALIGN_TYPE_LEFT, CVALIGN_TYPE_CENTER, CRGBA(0,0,0));
    }
    else {
      double xm = (x1 + x2)/2;

      renderer->drawRotatedText(CPoint2D(xm, y1), cbaxis.text(), cbaxis.labelRotate(),
                                CHALIGN_TYPE_LEFT, CVALIGN_TYPE_CENTER, CRGBA(0,0,0));
    }
  }
}

CColor
CGnuPlotColorBox::
valueToColor(double x) const
{
  const CGnuPlotAxisData &cbaxis = group_->colorBox()->axis();

  double cbmin = cbaxis.min().getValue(0);
  double cbmax = cbaxis.max().getValue(1);

  double z = CGnuPlotUtil::map(x, cbmin, cbmax, 0, 1);

  return group_->palette()->getColor(z);
}

void
CGnuPlotColorBox::
show(std::ostream &os) const
{
  os << "color box " <<
        (border_ ? "with border" : "without border") << ", ";

  if (enabled_) {
    os << (borderStyle_ < 0 ? std::string("DEFAULT line type") :
                              "line type " + CStrUtil::toString(borderStyle_)) << " " <<
          (front_ ? "is drawn front" : "is drawn back") << std::endl;

    if (! user_)
      os << "at DEFAULT position" << std::endl;
    else
      os << "at USER origin: " << origin_ << " size: " << size_ << std::endl;
  }
  else {
    os << "DEFAULT line type is NOT drawn" << std::endl;
  }

  if (isVertical())
    os << "color gradient is VERTICAL in the color box" << std::endl;
  else
    os << "color gradient is HORIZONTAL in the color box" << std::endl;
}

void
CGnuPlotColorBox::
save(std::ostream &os) const
{
  os << "set colorbox ";

  if (isVertical())
    os << "vertical ";
  else
    os << "horizontal ";

  if (user_) {
    os << "origin " << origin_ << " ";
    os << "size "   << size_   << " ";
  }
  else
    os << "default ";

  if (front_)
    os << "front ";
  else
    os << "back ";

  if (border_) {
    if (borderStyle_ < 0)
      os << "bdefault";
    else
      os << "border " << borderStyle_;
  }
  else
    os << "noborder";

  os << std::endl;
}
