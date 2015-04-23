#include <CGnuPlotStylePieChart.h>
#include <CGnuPlotPlot.h>
#include <CGnuPlotGroup.h>
#include <CGnuPlotWindow.h>
#include <CGnuPlotRenderer.h>
#include <CGnuPlotKey.h>
#include <CGnuPlotPieObject.h>

namespace {
  typedef std::pair<std::string,double> NameValue;
  typedef std::vector<NameValue>        NameValues;

  NameValues nameValues(CGnuPlotPlot *plot) {
    NameValues values;

    for (const auto &point : plot->getPoints2D()) {
      std::string name;
      double      value;

      if (point.getValue(1, name) && point.getValue(2, value))
        values.push_back(NameValue(name, value));
    }

    return values;
  }

  void getPointsColor(int pi, CRGBA &lc, CRGBA &fc) {
    lc = CGnuPlotStyleInst->indexColor("subtle", pi + 1);
    fc = lc;

    fc.setAlpha(0.5);
  }
}

//-------

CGnuPlotStylePieChart::
CGnuPlotStylePieChart() :
 CGnuPlotStyleBase(CGnuPlot::PlotStyle::PIECHART)
{
}

void
CGnuPlotStylePieChart::
draw2D(CGnuPlotPlot *plot, CGnuPlotRenderer *renderer)
{
  const CBBox2D &bbox = plot->getBBox();

  CPoint2D pc = bbox.getCenter();
  double   r  = bbox.getWidth()/2;

  NameValues values = nameValues(plot);

  //---

  double sum = 0;

  for (const auto &v : values)
    sum += v.second;

  //---

  if (! renderer->isPseudo())
    plot->updatePieCacheSize(values.size());

  int    i      = 0;
  double angle1 = 90.0;

  for (const auto &v : values) {
    const std::string &name  = v.first;
    double             value = v.second;

    double dangle = 360.0*value/sum;
    double angle2 = angle1 - dangle;

    if (! renderer->isPseudo()) {
      CGnuPlotPieObject *pie = plot->pieObjects()[i];

      pie->setCenter(pc);
      pie->setRadius(r);
      pie->setAngle1(angle1);
      pie->setAngle2(angle2);
      pie->setName(name);

      CRGBA lc, fc;

      getPointsColor(i, lc, fc);

      fc.setAlpha(0.5);

      if (! pie->hasColor()) {
        pie->setLineColor(lc);
        pie->setFillColor(fc);
      }
    }
    else
      renderer->drawEllipse(pc, r, r, 0, CRGBA(0,0,0), 1);

    angle1 = angle2;

    ++i;
  }

  if (! renderer->isPseudo()) {
    for (const auto &pie : plot->pieObjects())
      pie->draw(renderer);
  }
}

void
CGnuPlotStylePieChart::
drawKey(CGnuPlotPlot *plot, CGnuPlotRenderer *renderer)
{
  CGnuPlotGroup *group = plot->group();

  const CGnuPlotKeyP     &key   = group->key();
//const CGnuPlotAxisData &xaxis = group->xaxis(1);

  if (! key->isDisplayed()) return;

  if (key->getFont().isValid())
    renderer->setFont(key->getFont());

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

  if (key->showTitle()) {
    header = key->getTitle();

    if (header != "")
      textHeight += font_size*ph;
  }

  NameValues values = nameValues(plot);

  for (const auto &v : values) {
    std::string label = v.first;

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
    CRGBA bg = group->window()->backgroundColor();

    renderer->fillRect(bbox, bg);
  }

  if (key->getDrawBox()) {
    CRGBA c(0, 0, 0);

    if (key->hasLineType())
      c = CGnuPlotStyleInst->indexColor(key->getLineType());

    renderer->drawRect(bbox, c, 1);
  }

  double y = y2 - by;

  if (header != "") {
    renderer->drawHAlignedText(CPoint2D((x1 + x2)/2, y), CHALIGN_TYPE_CENTER, 0,
                               CVALIGN_TYPE_TOP, 0, header, CRGBA(0,0,0));

    y -= font_size*ph;
  }

  int pi = 0;

  for (const auto &v : values) {
    double xx = (key->isReverse() ? x1 + bx : x2 - bw*pw - bx);
    double yy = y - font_size*ph/2;

    CPoint2D p1(xx, yy - bw*ph/2), p2(xx + bw*pw, yy + bw*ph/2);

    CRGBA lc, fc;

    getPointsColor(pi, lc, fc);

    CBBox2D bbox(p1, p2);

    renderer->fillRect(bbox, fc);
    renderer->drawRect(bbox, lc, 1);

    std::string label = v.first;

    //double lw = font->getStringWidth(label);

    CRGBA tc = CRGBA(0,0,0);

    if (key->isReverse())
      renderer->drawHAlignedText(CPoint2D(xx + bw*pw + bx, y), CHALIGN_TYPE_LEFT, 0,
                                 CVALIGN_TYPE_TOP, 0, label, tc);
    else
      renderer->drawHAlignedText(CPoint2D(xx - bx, y), CHALIGN_TYPE_RIGHT, 0,
                                 CVALIGN_TYPE_TOP, 0, label, tc);

    y -= font_size*ph;

    ++pi;
  }
}

CBBox2D
CGnuPlotStylePieChart::
fit(CGnuPlotPlot *)
{
  return CBBox2D(-1, -1, 1, 1);
}