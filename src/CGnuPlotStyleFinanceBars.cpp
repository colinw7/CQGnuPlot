#include <CGnuPlotStyleFinanceBars.h>
#include <CGnuPlotFinanceBarObject.h>
#include <CGnuPlotGroup.h>
#include <CGnuPlotPlot.h>
#include <CGnuPlotRenderer.h>

class CGnuPlotStyleFinanceData {
 public:
  CGnuPlotStyleFinanceData() { }

  double value() const { return value_; }
  void setValue(double r) { value_ = r; }

  double low() const { return low_; }
  void setLow(double r) { low_ = r; }

  double high() const { return high_; }
  void setHigh(double r) { high_ = r; }

  double open() const { return open_; }
  void setOpen(double r) { open_ = r; }

  double close() const { return close_; }
  void setClose(double r) { close_ = r; }

  double barWidth() const { return barWidth_; }
  void setBarWidth(double r) { barWidth_ = r; }

  const CRGBA &lineColor() const { return lineColor_; }
  void setLineColor(const CRGBA &v) { lineColor_ = v; }

  double lineWidth() const { return lineWidth_; }
  void setLineWidth(double r) { lineWidth_ = r; }

  void draw(CGnuPlotPlot *plot, CGnuPlotRenderer *renderer) const;

 private:
  double value_    { 0.0 };
  double low_      { 0.0 };
  double high_     { 0.0 };
  double open_     { 0.0 };
  double close_    { 0.0 };
  double barWidth_ { 0.0 };
  CRGBA  lineColor_;
  double lineWidth_ { 0.0 };
};

CGnuPlotStyleFinanceBars::
CGnuPlotStyleFinanceBars() :
 CGnuPlotStyleBase(CGnuPlot::PlotStyle::FINANCEBARS)
{
}

void
CGnuPlotStyleFinanceBars::
draw2D(CGnuPlotPlot *plot, CGnuPlotRenderer *renderer)
{
  const CGnuPlotLineStyle &lineStyle = plot->lineStyle();

  CGnuPlotStroke stroke(plot);

  bool isCalcColor = lineStyle.isCalcColor();

  //---

  // calc bar size
  double bs = plot->barsSize();

  double bw = 0;

  if (! renderer->isPseudo())
    bw = renderer->pixelWidthToWindowWidth(4*bs);

  //---

  typedef std::vector<CGnuPlotStyleFinanceData> FinanceDatas;

  FinanceDatas financeDatas;

  for (const auto &point : plot->getPoints2D()) {
    std::vector<double> reals;

    (void) point.getReals(reals);

    if ((! isCalcColor && reals.size() < 5) || (isCalcColor && reals.size() < 6))
      continue;

    double date  = reals[0];
    double open  = reals[1];
    double low   = reals[2];
    double high  = reals[3];
    double close = reals[4];

    if (renderer->isPseudo() && ! renderer->isInside(CPoint2D(date, low)))
      continue;

    CRGBA lc = stroke.color();

    if (isCalcColor) {
      double z = reals[5];

      if (renderer->isPseudo())
        renderer->setCBValue(z);
      else
        lc = lineStyle.calcColor(plot, z);
    }

    CPoint2D p1(date, low );
    CPoint2D p2(date, high);

    CPoint2D p3(date, open );
    CPoint2D p4(date, close);

    CGnuPlotStyleFinanceData financeData;

    financeData.setValue    (p1.x);
    financeData.setLow      (p1.y);
    financeData.setHigh     (p2.y);
    financeData.setOpen     (p3.y);
    financeData.setClose    (p4.y);
    financeData.setBarWidth (bw);
    financeData.setLineColor(lc);
    financeData.setLineWidth(stroke.width());

    financeDatas.push_back(financeData);
  }

  if (! renderer->isPseudo())
    plot->updateFinanceBarCacheSize(financeDatas.size());

  int i = 0;

  for (const auto &financeData : financeDatas) {
    if (! renderer->isPseudo()) {
      CGnuPlotFinanceBarObject *bar = plot->financeBarObjects()[i];

      bar->setValue(financeData.value());
      bar->setLow  (financeData.low  ());
      bar->setHigh (financeData.high ());
      bar->setOpen (financeData.open ());
      bar->setClose(financeData.close());

      if (! bar->testAndSetUsed()) {
        bar->setBarWidth(financeData.barWidth());

        CGnuPlotStrokeP stroke(plot->createStroke());

        stroke->setColor(financeData.lineColor());
        stroke->setWidth(financeData.lineWidth());

        bar->setStroke(stroke);
      }
    }
    else {
      financeData.draw(plot, renderer);
    }

    ++i;
  }

  if (! renderer->isPseudo()) {
    for (const auto &bar : plot->financeBarObjects())
      bar->draw(renderer);
  }
}

void
CGnuPlotStyleFinanceData::
draw(CGnuPlotPlot *plot, CGnuPlotRenderer *renderer) const
{
  CPoint2D p1(value(), low()  );
  CPoint2D p2(value(), high() );
  CPoint2D p3(value(), open() );
  CPoint2D p4(value(), close());

  CGnuPlotGroup *group = plot->group();

  p1 = group->mapLogPoint(plot->xind(), plot->yind(), 1, p1);
  p2 = group->mapLogPoint(plot->xind(), plot->yind(), 1, p2);
  p3 = group->mapLogPoint(plot->xind(), plot->yind(), 1, p3);
  p4 = group->mapLogPoint(plot->xind(), plot->yind(), 1, p4);

  CPoint2D db(barWidth(), 0);

  renderer->drawClipLine(p1, p2     , lineWidth(), lineColor());
  renderer->drawClipLine(p3, p3 - db, lineWidth(), lineColor());
  renderer->drawClipLine(p4, p4 + db, lineWidth(), lineColor());
}
