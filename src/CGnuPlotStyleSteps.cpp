#include <CGnuPlotStyleSteps.h>
#include <CGnuPlotPlot.h>
#include <CGnuPlotRenderer.h>
#include <CGnuPlotUtil.h>

CGnuPlotStyleStepsBase::
CGnuPlotStyleStepsBase(CGnuPlotTypes::PlotStyle style) :
 CGnuPlotStyleBase(style)
{
}

//------

CGnuPlotStyleSteps::
CGnuPlotStyleSteps() :
 CGnuPlotStyleStepsBase(CGnuPlot::PlotStyle::STEPS)
{
}

//------

CGnuPlotStyleFSteps::
CGnuPlotStyleFSteps() :
 CGnuPlotStyleStepsBase(CGnuPlot::PlotStyle::FSTEPS)
{
}

//------

CGnuPlotStyleHiSteps::
CGnuPlotStyleHiSteps() :
 CGnuPlotStyleStepsBase(CGnuPlot::PlotStyle::HISTEPS)
{
}

//------

CGnuPlotStyleFillSteps::
CGnuPlotStyleFillSteps() :
 CGnuPlotStyleStepsBase(CGnuPlot::PlotStyle::FILLSTEPS)
{
}

//------

void
CGnuPlotStyleStepsBase::
draw2D(CGnuPlotPlot *plot, CGnuPlotRenderer *renderer)
{
  const CBBox2D &bbox = plot->getBBox();

  double ymin = bbox.getYMin();

  const CGnuPlotLineStyle &lineStyle = plot->lineStyle();

  const CRGBA &c = lineStyle.calcColor(CRGBA(1,0,0));

  uint np = plot->numPoints2D();

  for (uint i1 = 0, i2 = 1; i2 < np; i1 = i2++) {
    const CGnuPlotPoint &point1 = plot->getPoint2D(i1);
    const CGnuPlotPoint &point2 = plot->getPoint2D(i2);

    CPoint2D p1, p2;

    if (point1.getPoint(p1) && point2.getPoint(p2)) {
      if      (plot->getStyle() == CGnuPlotTypes::PlotStyle::HISTEPS) {
        double xm = CGnuPlotUtil::avg({p1.x, p2.x});

        CPoint2D p3(xm, p1.y);
        CPoint2D p4(xm, p2.y);

        renderer->drawClipLine(p1, p3, lineStyle.width(), c);
        renderer->drawClipLine(p3, p4, lineStyle.width(), c);
        renderer->drawClipLine(p4, p2, lineStyle.width(), c);
      }
      else if (plot->getStyle() == CGnuPlotTypes::PlotStyle::STEPS) {
        CPoint2D p3(p2.x, p1.y);

        renderer->drawClipLine(p1, p3, lineStyle.width(), c);
        renderer->drawClipLine(p3, p2, lineStyle.width(), c);
      }
      else if (plot->getStyle() == CGnuPlotTypes::PlotStyle::FSTEPS) {
        CPoint2D p3(p1.x, p2.y);

        renderer->drawClipLine(p1, p3, lineStyle.width(), c);
        renderer->drawClipLine(p3, p2, lineStyle.width(), c);
      }
      else if (plot->getStyle() == CGnuPlotTypes::PlotStyle::FILLSTEPS) {
        CBBox2D bbox(p1.x, ymin, p2.x, p1.y);

        renderer->fillRect(bbox, c);
      }
    }
  }

  if (plot->getStyle() != CGnuPlotTypes::PlotStyle::FILLSTEPS) {
    for (const auto &point : plot->getPoints2D()) {
      CPoint2D p;

      if (point.getPoint(p))
        renderer->drawSymbol(p, CGnuPlotTypes::SymbolType::FILLED_BOX, 1.0, c);
    }
  }
}

CBBox2D
CGnuPlotStyleStepsBase::
fit(CGnuPlotPlot *)
{
  return CBBox2D();
}
