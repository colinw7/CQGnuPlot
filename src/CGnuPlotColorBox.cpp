#include <CGnuPlotColorBox.h>
#include <CGnuPlotGroup.h>
#include <CGnuPlotRenderer.h>

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

void
CGnuPlotColorBox::
draw(CGnuPlotRenderer *renderer)
{
  if (! isEnabled())
    return;

  CBBox2D irbbox = renderer->range();
  CBBox2D orbbox = group_->getRegionBBox();

  double pw = renderer->pixelWidthToWindowWidth  (1);
  double ph = renderer->pixelHeightToWindowHeight(1);

  double bx = 4*pw;
  double by = 4*ph;

  double dx = orbbox.getRight()  - irbbox.getRight () - 2*bx;
  double dy = irbbox.getBottom() - orbbox.getBottom() - 2*by;

  // calc color box position
  double x1, y1, x2, y2, tx, ty;

  if (user_) {
    CPoint2D origin = origin_.getPoint(renderer);

    x1 = origin.x;
    y1 = origin.y;
    x2 = x1 + size_.width ;
    y2 = y1 + size_.height;

    if (vertical_) {
      tx = x2 + bx;
      ty = y1;
    }
    else {
      tx = x1;
      ty = y1 - by;
    }
  }
  else {
    if (vertical_) {
      x1 = irbbox.getRight() + bx;
      y1 = irbbox.getBottom();
      x2 = x1 + dx/2;
      y2 = irbbox.getTop();

      tx = x2 + bx;
      ty = y1;
    }
    else {
      x1 = irbbox.getLeft();
      y1 = orbbox.getBottom() + by;
      x2 = irbbox.getRight();
      y2 = y1 + dy/2;

      tx = x1;
      ty = y1 - by;
    }
  }

  //int n = CGnuPlotStyleInst->getNumColors("basic");
  int n = 10; // TODO: from palette

  CPoint2D p1, p2;

  renderer->windowToPixel(CPoint2D(x1, y1), p1);
  renderer->windowToPixel(CPoint2D(x2, y2), p2);

  // draw colors
  if (vertical_) {
    double h  = fabs(p2.y - p1.y);
    int    ih = int(h);

    for (int i = 0; i < ih; ++i) {
      CPoint2D pl1, pl2;

      renderer->pixelToWindow(CPoint2D(p1.x, p1.y - i), pl1);
      renderer->pixelToWindow(CPoint2D(p2.x, p1.y - i), pl2);

      double z = double(i)/h;

      CRGBA c = group_->palette()->getColor(z);

      renderer->drawLine(pl1, pl2, 1, c);
    }
  }
  else {
    double w  = fabs(p2.x - p1.x);
    int    iw = int(w);

    for (int i = 0; i < iw; ++i) {
      CPoint2D pl1, pl2;

      renderer->pixelToWindow(CPoint2D(p1.x + i, p1.y), pl1);
      renderer->pixelToWindow(CPoint2D(p1.x + i, p2.y), pl2);

      double z = double(i)/w;

      CRGBA c = group_->palette()->getColor(z);

      renderer->drawLine(pl1, pl2, 1, c);
    }
  }

  // draw tics
  if (vertical_) {
    double dy = (y2 - y1)/(n - 1);

    double y = y1;

    for (int i = 1; i <= n; ++i) {
      if (i != 1 || i != n) {
        CPoint2D pl1(x1     , y);
        CPoint2D pl2(x1 + bx, y);

        renderer->drawLine(pl1, pl2, 1, CRGBA(0,0,0));

        CPoint2D pl3(x2 - bx, y);
        CPoint2D pl4(x2     , y);

        renderer->drawLine(pl3, pl4, 1, CRGBA(0,0,0));
      }

      std::stringstream ss;

      ss << i;

      renderer->drawText(CPoint2D(tx, y), ss.str());

      y += dy;
    }
  }
  else {
    double dx = (x2 - x1)/(n - 1);

    double x = x1;

    for (int i = 1; i <= n; ++i) {
      if (i != 1 || i != n) {
        CPoint2D pl1(x, y1     );
        CPoint2D pl2(x, y1 + by);

        renderer->drawLine(pl1, pl2, 1, CRGBA(0,0,0));

        CPoint2D pl3(x, y2 - by);
        CPoint2D pl4(x, y2     );

        renderer->drawLine(pl3, pl4, 1, CRGBA(0,0,0));
      }

      std::stringstream ss;

      ss << i;

      renderer->drawText(CPoint2D(x, ty), ss.str());

      x += dx;
    }
  }

  bbox_ = CBBox2D(CPoint2D(x1, y1), CPoint2D(x2, y2));

  if (border_) {
    CRGBA  c = CRGBA(0, 0, 0);
    double w = 1.0;

    if (borderStyle_ >= 0) {
      CGnuPlotLineStyleP lineStyle = group_->app()->getLineStyleInd(borderStyle_);

      c = lineStyle->color(c);
      w = lineStyle->width();
    }

    renderer->drawRect(bbox_, c, w);
  }
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

  if (vertical_)
    os << "color gradient is VERTICAL in the color box" << std::endl;
  else
    os << "color gradient is HORIZONTAL in the color box" << std::endl;
}

void
CGnuPlotColorBox::
save(std::ostream &os) const
{
  os << "set colorbox ";

  if (vertical_)
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
