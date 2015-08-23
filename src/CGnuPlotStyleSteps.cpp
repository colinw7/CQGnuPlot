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

  const CRGBA &c = lineStyle.calcColor(plot->group(), CRGBA(1,0,0));

  uint np = plot->numPoints2D();

  for (uint i1 = 0, i2 = 1; i2 < np; i1 = i2++) {
    const CGnuPlotPoint &point1 = plot->getPoint2D(i1);
    const CGnuPlotPoint &point2 = plot->getPoint2D(i2);

    CPoint2D p1, p2;

    double lw = lineStyle.calcWidth();

    if (point1.getPoint(p1) && point2.getPoint(p2)) {
      if      (plot->getStyle() == CGnuPlotTypes::PlotStyle::HISTEPS) {
        double xm = CGnuPlotUtil::avg({p1.x, p2.x});

        CPoint2D p3(xm, p1.y);
        CPoint2D p4(xm, p2.y);

        if      (i1 == 0) {
          double xm1 = p1.x - (xm - p1.x);

          CPoint2D p5(xm1, 0);
          CPoint2D p6(xm1, p1.y);

          renderer->drawClipLine(p5, p6, lw, c);
          renderer->drawClipLine(p6, p1, lw, c);
        }

        renderer->drawClipLine(p1, p3, lw, c);
        renderer->drawClipLine(p3, p4, lw, c);
        renderer->drawClipLine(p4, p2, lw, c);

        if (i2 == np - 1) {
          double xm1 = p2.x + (p2.x - xm);

          CPoint2D p5(xm1, p2.y);
          CPoint2D p6(xm1, 0);

          renderer->drawClipLine(p2, p5, lw, c);
          renderer->drawClipLine(p5, p6, lw, c);
        }
      }
      else if (plot->getStyle() == CGnuPlotTypes::PlotStyle::STEPS) {
        CPoint2D p3(p2.x, p1.y);

        renderer->drawClipLine(p1, p3, lw, c);
        renderer->drawClipLine(p3, p2, lw, c);
      }
      else if (plot->getStyle() == CGnuPlotTypes::PlotStyle::FSTEPS) {
        CPoint2D p3(p1.x, p2.y);

        renderer->drawClipLine(p1, p3, lw, c);
        renderer->drawClipLine(p3, p2, lw, c);
      }
      else if (plot->getStyle() == CGnuPlotTypes::PlotStyle::FILLSTEPS) {
        CBBox2D bbox(p1.x, ymin, p2.x, p1.y);

        renderer->fillRect(bbox, c);
      }
    }
  }

#if 0
  // TODO: when is symbol needed ?
  if (plot->getStyle() != CGnuPlotTypes::PlotStyle::FILLSTEPS) {
    for (const auto &point : plot->getPoints2D()) {
      CPoint2D p;

      if (point.getPoint(p))
        renderer->drawSymbol(p, CGnuPlotTypes::SymbolType::FILLED_BOX, 1.0, c);
    }
  }
#endif
}

CBBox2D
CGnuPlotStyleStepsBase::
fit(CGnuPlotPlot *)
{
  return CBBox2D();
}
