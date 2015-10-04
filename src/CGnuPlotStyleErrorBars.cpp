#include <CGnuPlotStyleErrorBars.h>
#include <CGnuPlotPlot.h>
#include <CGnuPlotRenderer.h>

CGnuPlotStyleErrorBars::
CGnuPlotStyleErrorBars() :
 CGnuPlotStyleBase(CGnuPlot::PlotStyle::ERRORBARS)
{
}

void
CGnuPlotStyleErrorBars::
draw2D(CGnuPlotPlot *plot, CGnuPlotRenderer *renderer)
{
  CGnuPlotGroup *group = plot->group();

  const CGnuPlotLineStyle &lineStyle = plot->lineStyle();

  CRGBA c = lineStyle.calcColor(group, CRGBA(1,0,0));

  typedef std::map<std::string,int> IndexMap;

  IndexMap indexMap;

  double pw = 0, ph = 0;

  if (! renderer->isPseudo()) {
    pw = renderer->pixelWidthToWindowWidth  (4);
    ph = renderer->pixelHeightToWindowHeight(4);
  }

  for (const auto &point : plot->getPoints2D()) {
    double x = 0;

    if (! point.getValue(1, x)) {
      std::string str;

      if (point.getValue(1, str)) {
        auto p = indexMap.find(str);

        if (p == indexMap.end()) {
          int ind = 0;

          auto p1 = indexMap.rbegin();

          if (p1 != indexMap.rend())
            ind = (*p1).second;

          p = indexMap.insert(p, IndexMap::value_type(str, ind));
        }

        x = (*p).second;
      }
    }

    double y = 0;

    (void) point.getValue(2, y);

    double xl = x;
    double xh = x;
    double yl = y;
    double yh = y;

    // x y xlow xhigh ylow yhigh
    if      (point.getNumValues() >= 6) {
      (void) point.getValue(3, xl);
      (void) point.getValue(4, xh);
      (void) point.getValue(5, yl);
      (void) point.getValue(6, yh);

      if (point.getNumValues() >= 6) {
        double z;

        if (point.getValue(7, z)) {
          if (renderer->isPseudo())
            renderer->setCBValue(z);
          else
            c = lineStyle.calcColor(plot, z);
        }
      }
    }
    // x y xdelta ydelta
    else if (point.getNumValues() >= 4) {
      double dx = 0;
      double dy = 0;

      (void) point.getValue(3, dx);
      (void) point.getValue(4, dy);

      xl = x - dx;
      xh = x + dx;
      yl = y - dy;
      yh = y + dy;

      if (point.getNumValues() >= 4) {
        double z;

        if (point.getValue(5, z)) {
          if (renderer->isPseudo())
            renderer->setCBValue(z);
          else
            c = lineStyle.calcColor(plot, z);
        }
      }
    }
    // x y delta
    else if (point.getNumValues() >= 3) {
      double dx = 0;

      (void) point.getValue(3, dx);

      double dy = dx;

      xl = x - dx;
      xh = x + dx;
      yl = y - dy;
      yh = y + dy;
    }

    renderer->drawClipLine(CPoint2D(x, yl), CPoint2D(x, yh), 1.0, c);
    renderer->drawClipLine(CPoint2D(xl, y), CPoint2D(xh, y), 1.0, c);

    renderer->drawClipLine(CPoint2D(xl, y - ph), CPoint2D(xl, y + ph), 1.0, c);
    renderer->drawClipLine(CPoint2D(xl, y - ph), CPoint2D(xl, y + ph), 1.0, c);

    renderer->drawClipLine(CPoint2D(x - pw, yl), CPoint2D(x + pw, yl), 1.0, c);
    renderer->drawClipLine(CPoint2D(x - pw, yh), CPoint2D(x + pw, yh), 1.0, c);
  }
}

void
CGnuPlotStyleErrorBars::
drawKeyLine(CGnuPlotPlot *plot, CGnuPlotRenderer *renderer, const CPoint2D &p1, const CPoint2D &p2)
{
  CGnuPlotGroup *group = plot->group();

  const CGnuPlotLineStyle &lineStyle = plot->lineStyle();

  CRGBA c = lineStyle.calcColor(group, CRGBA(1,0,0));

  renderer->drawLine(p1, p2, lineStyle.calcWidth(), c, lineStyle.calcDash());

  double ph = renderer->pixelHeightToWindowHeight(4);

  CPoint2D d1(0, -ph), d2(0, ph);

  renderer->drawLine(p1 + d1, p1 + d2, lineStyle.calcWidth(), c, lineStyle.calcDash());
  renderer->drawLine(p2 + d1, p2 + d2, lineStyle.calcWidth(), c, lineStyle.calcDash());
}

CBBox2D
CGnuPlotStyleErrorBars::
fit(CGnuPlotPlot *)
{
  return CBBox2D();
}
