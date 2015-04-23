#include <CGnuPlotStyleBoxPlot.h>
#include <CGnuPlotPlot.h>
#include <CGnuPlotRenderer.h>
#include <CBoxWhisker.h>

CGnuPlotStyleBoxPlot::
CGnuPlotStyleBoxPlot() :
 CGnuPlotStyleBase(CGnuPlot::PlotStyle::BOXPLOT)
{
}

void
CGnuPlotStyleBoxPlot::
draw2D(CGnuPlotPlot *plot, CGnuPlotRenderer *renderer)
{
  const CGnuPlotLineStyle &lineStyle = plot->lineStyle();

  CRGBA fc = lineStyle.calcColor(CRGBA(1,0,0));
  CRGBA lc = CRGBA(0,0,0);

  fc.setAlpha(0.5);

  double bw = plot->boxWidth().getSpacing(0.1);

  double ww = bw;

  if (plot->bars().size() < 0.0)
    ww = bw;
  else {
    if (! renderer->isPseudo())
      ww = renderer->pixelWidthToWindowWidth(12*plot->bars().size());
  }

  double x        = 0;
  bool   discrete = false;

  for (const auto &point : plot->getPoints2D()) {
    if (point.getNumValues() >= 2) {
      double x1;

      if (point.getValue(1, x1))
        x = x1;

      if (point.getNumValues() >= 4)
        discrete = true;

      break;
    }
  }

  //----

  typedef std::vector<double>         YVals;
  typedef std::map<int,YVals>         IYVals;
  typedef std::map<std::string,YVals> SYVals;

  IYVals iyv;
  SYVals syv;

  for (const auto &point : plot->getPoints2D()) {
    if (point.getNumValues() >= 2) {
      double y = 0, y1 = 0;

      if (point.getValue(2, y1))
        y = y1;

      if (discrete && point.getNumValues() >= 4) {
        int         i;
        std::string s;

        if      (point.getValue(4, i))
          iyv[i].push_back(y);
        else if (point.getValue(4, s))
          syv[s].push_back(y);
      }
      else
        iyv[0].push_back(y);
    }
  }

  // always use index for x (ignore x value) ??
  double ix = (syv.empty() && iyv.size() == 1 ? x : 1);

  for (const auto &yv : iyv) {
    CBoxWhisker whisker(yv.second);

    double x11 = ix - ww/2;
    double x21 = ix + ww/2;

    renderer->drawClipLine(CPoint2D(ix, whisker.min()), CPoint2D(ix, whisker.max()), 1, lc);

    renderer->drawClipLine(CPoint2D(x11, whisker.min()), CPoint2D(x21, whisker.min()), 1, lc);
    renderer->drawClipLine(CPoint2D(x11, whisker.max()), CPoint2D(x21, whisker.max()), 1, lc);

    double x12 = ix - bw/2;
    double x22 = ix + bw/2;

    CBBox2D bbox(x12, whisker.lower(), x22, whisker.upper());

    renderer->fillRect(bbox, fc);
    renderer->drawRect(bbox, lc, 1);

    renderer->drawClipLine(CPoint2D(x12, whisker.median()), CPoint2D(x22, whisker.median()), 1, lc);

    double opw = 0, oph = 0;

    if (! renderer->isPseudo()) {
      opw = renderer->pixelWidthToWindowWidth  (4);
      oph = renderer->pixelHeightToWindowHeight(4);
    }

    for (const auto &o : whisker.outliers())
      renderer->drawEllipse(CPoint2D(ix, whisker.value(o)), opw, oph, 0, lc, 1);

    ++ix;
  }

  for (const auto &yv : syv) {
    //const std::string &s = yv.first;

    CBoxWhisker whisker(yv.second);

    double x11 = ix - ww/2;
    double x21 = ix + ww/2;

    renderer->drawClipLine(CPoint2D(ix, whisker.min()), CPoint2D(ix, whisker.max()), 1, lc);

    renderer->drawClipLine(CPoint2D(x11, whisker.min()), CPoint2D(x21, whisker.min()), 1, lc);
    renderer->drawClipLine(CPoint2D(x11, whisker.max()), CPoint2D(x21, whisker.max()), 1, lc);

    double x12 = ix - bw/2;
    double x22 = ix + bw/2;

    CBBox2D bbox(x12, whisker.lower(), x22, whisker.upper());

    renderer->fillRect(bbox, fc);
    renderer->drawRect(bbox, lc, 1);

    renderer->drawClipLine(CPoint2D(x12, whisker.median()), CPoint2D(x22, whisker.median()), 1, lc);

    double opw = 0, oph = 0;

    if (! renderer->isPseudo()) {
      opw = renderer->pixelWidthToWindowWidth  (4);
      oph = renderer->pixelHeightToWindowHeight(4);
    }

    for (const auto &o : whisker.outliers())
      renderer->drawEllipse(CPoint2D(ix, whisker.value(o)), opw, oph, 0, lc, 1);

    ++ix;
  }
}

CBBox2D
CGnuPlotStyleBoxPlot::
fit(CGnuPlotPlot *)
{
  return CBBox2D();
}
