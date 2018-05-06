#include <CGnuPlotColorBox.h>
#include <CGnuPlotGroup.h>
#include <CGnuPlotRenderer.h>
#include <CGnuPlotUtil.h>

CGnuPlotColorBox::
CGnuPlotColorBox(CGnuPlotGroup *group) :
 group_(group)
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

double
CGnuPlotColorBox::
titleRotate() const
{
  const CGnuPlotAxisData &cbaxis = group_->colorBox()->axis();

  return cbaxis.labelRotate().getValue(0);
}

void
CGnuPlotColorBox::
setTitleRotate(double r)
{
  CGnuPlotAxisData &cbaxis = group_->colorBox()->axis();

  cbaxis.setLabelRotate(r);
}

const std::string &
CGnuPlotColorBox::
titleString() const
{
  const CGnuPlotAxisData &cbaxis = group_->colorBox()->axis();

  return cbaxis.text();
}

void
CGnuPlotColorBox::
setTitleString(const std::string &s)
{
  CGnuPlotAxisData &cbaxis = group_->colorBox()->axis();

  cbaxis.setText(s);
}

void
CGnuPlotColorBox::
draw(CGnuPlotRenderer *renderer)
{
  if (! isEnabled())
    return;

  CBBox2D irbbox = renderer->range();
  CBBox2D orbbox = (! renderer->isPseudo() ? group_->getRegionBBox() : irbbox);
  CBBox2D axbbox = group_->axisBBox2D();

  double pw = (! renderer->isPseudo() ? renderer->pixelWidthToWindowWidth  (1) : 1);
  double ph = (! renderer->isPseudo() ? renderer->pixelHeightToWindowHeight(1) : 1);

  double bx = 4*pw;
  double by = 4*ph;

  double dx, dy;

  if (! renderer->isPseudo()) {
    dx = orbbox.getRight()  - irbbox.getRight () - 2*bx;
    dy = irbbox.getBottom() - orbbox.getBottom() - 2*by;
  }
  else {
    dx = irbbox.getWidth ()/10;
    dy = irbbox.getHeight()/10;

    bx = irbbox.getWidth ()/100;
    by = irbbox.getHeight()/100;
  }

  const CGnuPlotAxisData &cbaxis = group_->colorBox()->axis();

  double cbmin = cbaxis.min().getValue(group_->zaxis(1).min().getValue(0));
  double cbmax = cbaxis.max().getValue(group_->zaxis(1).max().getValue(1));

  // calc color box position
  double x1, y1, x2, y2, tx, ty;

  if (isUser()) {
    // screen coords
    CPoint2D origin = this->origin().getPoint2D(renderer);

    if (group()->is3D()) {
#if 0
      // TODO: colorbox position in 3D ??
      double aw = axbbox.getWidth (); double aw1 = 1.2*aw;
      double ah = axbbox.getHeight(); double ah1 = 1.2*ah;

      double ax1 = axbbox.getXMin() - aw/10; double ay1 = axbbox.getYMin() - ah/10;
      double ax2 = axbbox.getXMax() + aw/10; double ay2 = axbbox.getYMax() + ah/10;

      x1 = CGnuPlotUtil::map(origin.x, 0, 1, ax1, ax2);
      y1 = CGnuPlotUtil::map(origin.y, 0, 1, ay1, ay2);
      x2 = x1 + CGnuPlotUtil::map(size().getWidth (), 0, 1, 0, aw1);
      y2 = y1 + CGnuPlotUtil::map(size().getHeight(), 0, 1, 0, ah1);
#else
      x1 = CGnuPlotUtil::map(origin.x, 0, 1, orbbox.getXMin(), orbbox.getXMax());
      y1 = CGnuPlotUtil::map(origin.y, 0, 1, orbbox.getYMin(), orbbox.getYMax());
      x2 = x1 + CGnuPlotUtil::map(size().getWidth (), 0, 1, 0, orbbox.getWidth ());
      y2 = y1 + CGnuPlotUtil::map(size().getHeight(), 0, 1, 0, orbbox.getHeight());
#endif
    }
    else {
      x1 = CGnuPlotUtil::map(origin.x, 0, 1, orbbox.getXMin(), orbbox.getXMax());
      y1 = CGnuPlotUtil::map(origin.y, 0, 1, orbbox.getYMin(), orbbox.getYMax());
      x2 = x1 + CGnuPlotUtil::map(size().getWidth (), 0, 1, 0, orbbox.getWidth ());
      y2 = y1 + CGnuPlotUtil::map(size().getHeight(), 0, 1, 0, orbbox.getHeight());
    }

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
      dx = CGnuPlotUtil::map(size().getWidth(), 0, 1, 0, irbbox.getWidth ());

      x1 = irbbox.getRight() + bx;
      y1 = irbbox.getBottom();
      x2 = x1 + dx;
      y2 = irbbox.getTop();

      tx = x2 + bx;
      ty = y1;
    }
    else {
      dy = CGnuPlotUtil::map(size().getHeight(), 0, 1, 0, irbbox.getWidth ());

      x1 = irbbox.getLeft();
      y2 = axbbox.getBottom() - by;
      x2 = irbbox.getRight();
      y1 = y2 - dy;

      tx = x1;
      ty = y1 - by;
    }
  }

  //---

  // draw colors
  if (! renderer->isPseudo()) {
    CPoint2D p1, p2;

    if (! renderer->isPseudo()) {
      renderer->windowToPixel(CPoint2D(x1, y1), p1);
      renderer->windowToPixel(CPoint2D(x2, y2), p2);
    }

    //---

    if (isVertical()) {
      double h  = fabs(p2.y - p1.y);
      int    ih = int(h);

      for (int i = 0; i < ih; ++i) {
        CPoint2D pl1(p1.x, p1.y - i);
        CPoint2D pl2(p2.x, p1.y - i);

        double z = CGnuPlotUtil::map(i, 0, ih - 1, 0, 1);

        CColor c = group_->palette()->getColor(z);

        renderer->drawPixelLine(pl1, pl2, c.rgba(), 1);
      }
    }
    else {
      double w  = fabs(p2.x - p1.x);
      int    iw = int(w);

      for (int i = 0; i < iw; ++i) {
        CPoint2D pl1(p1.x + i, p1.y);
        CPoint2D pl2(p1.x + i, p2.y);

        double z = CGnuPlotUtil::map(i, 0, iw - 1, 0, 1);

        CColor c = group_->palette()->getColor(z);

        renderer->drawPixelLine(pl1, pl2, c.rgba(), 1);
      }
    }
  }

  //---

  // draw tics
  if (cbaxis.isShowTics()) {
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

        double w1 = cbaxis.getTicMajorScale()*6*pw;

        for (int i = 0; i <= numTicks1; ++i) {
          double yy = CGnuPlotUtil::map(c, cbmin, cbmax, y1, y2);

          if (i != 0 && i != numTicks1) {
            CPoint2D pl1(x1     , yy);
            CPoint2D pl2(x1 + w1, yy);

            renderer->drawLine(pl1, pl2, CRGBA(0,0,0), 1);

            CPoint2D pl3(x2 - w1, yy);
            CPoint2D pl4(x2     , yy);

            renderer->drawLine(pl3, pl4, CRGBA(0,0,0), 1);
          }

          std::stringstream ss;

          ss << c;

          CPoint2D p(tx, yy);

          renderer->drawHAlignedText(p, HAlignPos(CHALIGN_TYPE_LEFT, 0),
                                     VAlignPos(CVALIGN_TYPE_CENTER, 0), ss.str(), CRGBA(0,0,0));

          group_->updateMarginBBox(renderer->getHAlignedTextBBox(ss.str()).moveBy(p));

          //y += dy;
          c += dc;
        }
      }
      else {
        //double dx = (x2 - x1)/numTicks1;

        //double x = x1;

        double h1 = cbaxis.getTicMajorScale()*6*ph;

        for (int i = 0; i <= numTicks1; ++i) {
          double xx = CGnuPlotUtil::map(c, cbmin, cbmax, x1, x2);

          if (i != 0 && i != numTicks1) {
            CPoint2D pl1(xx, y1     );
            CPoint2D pl2(xx, y1 + h1);

            renderer->drawLine(pl1, pl2, CRGBA(0,0,0), 1);

            CPoint2D pl3(xx, y2 - h1);
            CPoint2D pl4(xx, y2     );

            renderer->drawLine(pl3, pl4, CRGBA(0,0,0), 1);
          }

          std::stringstream ss;

          ss << c;

          CPoint2D p(xx, ty);

          renderer->drawHAlignedText(p, HAlignPos(CHALIGN_TYPE_CENTER, 0),
                                     VAlignPos(CVALIGN_TYPE_TOP, 0), ss.str(), CRGBA(0,0,0));

          group_->updateMarginBBox(renderer->getHAlignedTextBBox(ss.str()).moveBy(p));

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
  }

  //---

  bbox_ = CBBox2D(CPoint2D(x1, y1), CPoint2D(x2, y2));

  group_->updateMarginBBox(bbox_);

  //---

  // draw border

  if (hasBorder()) {
    CRGBA  c = CRGBA(0, 0, 0);
    double w = 1.0;

    if (borderStyle() >= 0) {
      CGnuPlotLineStyleP lineStyle = group_->app()->getLineStyleInd(borderStyle());

      c = lineStyle->calcColor(group_, c);
      w = lineStyle->calcWidth();
    }

    renderer->drawRect(bbox_, c, w);
  }

  //---

  // draw label

  if (titleString() != "") {
    CRGBA c(0,0,0);

    const std::string &str = titleString();

    double   a;
    CPoint2D p;

    if (isVertical()) {
      a = cbaxis.labelRotate().getValue(90);

      double ym = (y1 + y2)/2;

      p = CPoint2D(x2 + bx, ym);

      renderer->drawRotatedText(p, str, a, HAlignPos(CHALIGN_TYPE_LEFT, 0),
                                VAlignPos(CVALIGN_TYPE_CENTER, 0), c);
    }
    else {
      a = cbaxis.labelRotate().getValue(0);

      double xm = (x1 + x2)/2;

       p = CPoint2D(xm, y1 - by);

      renderer->drawRotatedText(p, str, a, HAlignPos(CHALIGN_TYPE_CENTER, 0),
                                VAlignPos(CVALIGN_TYPE_TOP, 0), c);
    }

    if (fabs(a) < 1E-6)
      group_->updateMarginBBox(renderer->getHAlignedTextBBox(str).moveBy(p));
  }
}

CColor
CGnuPlotColorBox::
valueToColor(double x) const
{
  const CGnuPlotAxisData &cbaxis = group_->colorBox()->axis();

  double cbmin = cbaxis.min().getValue(group_->zaxis(1).min().getValue(0));
  double cbmax = cbaxis.max().getValue(group_->zaxis(1).max().getValue(1));

  double z = CGnuPlotUtil::map(x, cbmin, cbmax, 0, 1);

  return group_->palette()->getColor(z);
}

//------

CGnuPlotColorBoxData::
CGnuPlotColorBoxData() :
 axis_(CGnuPlotTypes::AxisType::CB, 1)
{
}

void
CGnuPlotColorBoxData::
show(std::ostream &os) const
{
  os << "color box " <<
        (hasBorder() ? "with border" : "without border") << ", ";

  if (isEnabled()) {
    os << (borderStyle() < 0 ? std::string("DEFAULT line type") :
                               "line type " + CGnuPlotUtil::toString(borderStyle())) << " " <<
          (isFront() ? "is drawn front" : "is drawn back") << std::endl;

    if (! isUser())
      os << "at DEFAULT position" << std::endl;
    else
      os << "at USER origin: " << origin() << " size: " << size() << std::endl;
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
CGnuPlotColorBoxData::
save(std::ostream &os) const
{
  os << "set colorbox ";

  if (isVertical())
    os << "vertical ";
  else
    os << "horizontal ";

  if (isUser()) {
    os << "origin " << origin() << " ";
    os << "size "   << size()   << " ";
  }
  else
    os << "default ";

  if (isFront())
    os << "front ";
  else
    os << "back ";

  if (hasBorder()) {
    if (borderStyle() < 0)
      os << "bdefault";
    else
      os << "border " << borderStyle();
  }
  else
    os << "noborder";

  os << std::endl;
}
