#include <CGnuPlotStyleRadar.h>
#include <CGnuPlotPlot.h>
#include <CGnuPlotGroup.h>
#include <CGnuPlotWindow.h>
#include <CGnuPlotRenderer.h>

namespace {
  CPoint2D radarPoint(const CPoint2D &o, double r, double a) {
    double x = o.x + r*cos(CAngle::Deg2Rad(a));
    double y = o.y + r*sin(CAngle::Deg2Rad(a));

    return CPoint2D(x, y);
  }

  std::vector<CPoint2D> radarPoints(const CPoint2D &o, double r, int np) {
    std::vector<CPoint2D> points;

    double a  = 90;
    double da = 360/np;

    for (int i = 0; i < np; ++i) {
      CPoint2D p = radarPoint(o, r, a);

      points.push_back(p);

      a -= da;
    }

    return points;
  }

  void getPointsColor(int pi, CRGBA &lc, CRGBA &fc) {
    lc = CGnuPlotStyleInst->indexColor(pi);
    fc = lc;

    fc.setAlpha(0.5);
  }
}

CGnuPlotStyleRadar::
CGnuPlotStyleRadar() :
 CGnuPlotStyleBase(CGnuPlot::PlotStyle::RADAR)
{
}

void
CGnuPlotStyleRadar::
draw2D(CGnuPlotPlot *plot, CGnuPlotRenderer *renderer)
{
  const CBBox2D &bbox = plot->bbox2D();

  CPoint2D pc = bbox.getCenter();
  double   r  = bbox.getWidth()/2;

  int np = -1;

  for (const auto &point : plot->getPoints2D()) {
    np = std::min(np < 0 ? INT_MAX : np, point.getNumValues());
  }

  if (np < 3)
    return;

  //double pw = renderer->pixelWidthToWindowWidth  (1);
  //double ph = renderer->pixelHeightToWindowHeight(1);

  CGnuPlotGroup *group = plot->group();

  const CGnuPlotKeyP     &key   = group->key();
  const CGnuPlotAxisData &xaxis = group->xaxis(1);

  const CGnuPlotKey::Columns &columns = key->columns();

  double da = 360/np;

  std::vector<CPoint2D> points = radarPoints(pc, r, np);

  double v = getRange(plot);

  //---

  // draw border
  renderer->drawPolygon(points, 0, CRGBA(0,0,0), CLineDash());

  //---

  // draw column labels (how determine indices)
  {
    double a = 90;

    for (int i = 1; i <= np && i < int(columns.size()); ++i) {
      CPoint2D p = radarPoint(pc, r, a);

      CHAlignType halign = CHALIGN_TYPE_CENTER;
      CVAlignType valign = CVALIGN_TYPE_CENTER;

      double dx = 0;
      double dy = 0;

      if      (p.x < pc.x - v/2) {
        halign = CHALIGN_TYPE_RIGHT;
        dx     = -8;
      }
      else if (p.x > pc.x + v/2) {
        halign = CHALIGN_TYPE_LEFT;
        dx     = 8;
      }

      if      (p.y < pc.y - v/2) {
        valign = CVALIGN_TYPE_TOP;
        dy     = 8;
      }
      else if (p.y > pc.y + v/2) {
        valign = CVALIGN_TYPE_BOTTOM;
        dy     = -8;
      }

      renderer->drawHAlignedText(p, HAlignPos(halign, dx), VAlignPos(valign, dy),
                                 columns[i], CRGBA(0,0,0));

      a -= da;
    }
  }

  //---

  // draw axis if needed
  if (xaxis.isDisplayed()) {
    double dr = 0.1;
    double ra = 0.1;

    while (ra < v) {
      std::vector<CPoint2D> points1 = radarPoints(pc, ra, np);

      renderer->drawPolygon(points1, 0, CRGBA(0.5,0.5,0.5,0.5), CLineDash());

      ra += dr;
    }

    for (const auto &p : points)
      renderer->drawLine(p, pc, 0, CRGBA(0.5,0.5,0.5,0.5));
  }

  //---

  int pi = 1;

  for (const auto &point : plot->getPoints2D()) {
    std::vector<CPoint2D> points1;

    double a = 90;

    for (int i = 0; i < np; ++i) {
      double v1;

      if (! point.getValue(i + 1, v1))
        continue;

      CPoint2D p = radarPoint(pc, v1, a);

      points1.push_back(p);

      a -= da;
    }

    CRGBA lc, fc;

    getPointsColor(pi, lc, fc);

    renderer->fillPolygon(points1, fc);

    renderer->drawPolygon(points1, 2, lc, CLineDash());

    ++pi;
  }
}

void
CGnuPlotStyleRadar::
drawKey(CGnuPlotPlot *plot, CGnuPlotRenderer *renderer)
{
  CGnuPlotGroup *group = plot->group();

  const CGnuPlotKeyP     &key   = group->key();
//const CGnuPlotAxisData &xaxis = group->xaxis(1);

  if (! key->isDisplayed()) return;

  if (key->getFont().isValid())
    renderer->setFont(key->getFont());

  CGnuPlotFill fill(plot);

  //---

  CBBox2D rbbox = (key->isOutside() ? group->getRegionBBox() : renderer->range());

  //---

  CFontPtr font = renderer->getFont();

  double font_size = font->getCharAscent() + font->getCharDescent();

  double pw = renderer->pixelWidthToWindowWidth  (1);
  double ph = renderer->pixelHeightToWindowHeight(1);

  double bx = 8*pw;
  double by = 8*ph;
  double bw = font_size - 2;

  CSize2D size;

  double textWidth = 0.0, textHeight = 0.0;

  std::string header;

  if (key->hasTitle()) {
    header = key->title();

    if (header != "")
      textHeight += font_size*ph;
  }

  for (const auto &point : plot->getPoints2D()) {
    std::string label = point.label();

    textWidth = std::max(textWidth, font->getStringWidth(label)*pw);

    textHeight += font_size*ph;
  }

  size = CSize2D(textWidth + bw*pw + 3*bx, textHeight + 2*by);

  CHAlignType halign = key->getHAlign();
  CVAlignType valign = key->getVAlign();

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

  CBBox2D bbox(x1, y1, x2, y2);

  if (key->getFillBox()) {
    renderer->fillRect(bbox, fill.background());
  }

  if (key->getDrawBox()) {
    CRGBA c(0, 0, 0);

    if (key->hasLineType())
      c = CGnuPlotStyleInst->indexColor(key->getLineType());

    renderer->drawRect(bbox, c, 1);
  }

  double y = y2 - by;

  if (header != "") {
    renderer->drawHAlignedText(CPoint2D((x1 + x2)/2, y), HAlignPos(CHALIGN_TYPE_CENTER, 0),
                               VAlignPos(CVALIGN_TYPE_TOP, 0), header, CRGBA(0,0,0));

    y -= font_size*ph;
  }

  int pi = 1;

  for (const auto &point : plot->getPoints2D()) {
    double xx = (key->isReverse() ? x1 + bx : x2 - bw*pw - bx);
    double yy = y - font_size*ph/2;

    CPoint2D p1(xx, yy - bw*ph/2), p2(xx + bw*pw, yy + bw*ph/2);

    CRGBA lc, fc;

    getPointsColor(pi, lc, fc);

    CBBox2D bbox(p1, p2);

    renderer->fillRect(bbox, fc);
    renderer->drawRect(bbox, lc, 1);

    std::string label = point.label();

    //double lw = font->getStringWidth(label);

    CRGBA tc = CRGBA(0,0,0);

    if (key->isReverse())
      renderer->drawHAlignedText(CPoint2D(xx + bw*pw + bx, y), HAlignPos(CHALIGN_TYPE_LEFT, 0),
                                 VAlignPos(CVALIGN_TYPE_TOP, 0), label, tc);
    else
      renderer->drawHAlignedText(CPoint2D(xx - bx, y), HAlignPos(CHALIGN_TYPE_RIGHT, 0),
                                 VAlignPos(CVALIGN_TYPE_TOP, 0), label, tc);

    y -= font_size*ph;

    ++pi;
  }
}

CBBox2D
CGnuPlotStyleRadar::
fit(CGnuPlotPlot *plot)
{
  double v = getRange(plot);

  return CBBox2D(0, 0, v, v);
}

double
CGnuPlotStyleRadar::
getRange(CGnuPlotPlot *plot) const
{
  COptReal vmin, vmax;

  for (const auto &point : plot->getPoints2D()) {
    int np = point.getNumValues();

    for (int i = 0; i < np; ++i) {
      double v;

      if (! point.getValue(i + 1, v))
        continue;

      vmin.updateMin(v);
      vmax.updateMax(v);
    }
  }

  return vmax.getValue(1);
}
