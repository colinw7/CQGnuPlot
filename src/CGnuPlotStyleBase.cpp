#include <CGnuPlotStyleBase.h>
#include <CGnuPlotGroup.h>
#include <CGnuPlotPlot.h>

CGnuPlotStylePointsBase::
CGnuPlotStylePointsBase(CGnuPlotTypes::PlotStyle style) :
 CGnuPlotStyleBase(style)
{
}

bool
CGnuPlotStylePointsBase::
mouseProbe(CGnuPlotPlot *plot, CGnuPlotProbeEvent &probeEvent)
{
  if (! plot->is2D()) return false;

  //---

  double minX = 1E50;
  int    minI = -1;

  int i = 0;

  for (const auto &point : plot->getPoints2D()) {
    CPoint2D p;

    if (! point.getPoint(p))
      continue;

    double dx = fabs(p.x - probeEvent.x());

    if (minI < 0 || dx < minX) {
      minX = dx;
      minI = i;
    }

    ++i;
  }

  if (minI < 0)
    return false;

  CBBox2D bbox = plot->bbox2D();

  double ymin = bbox.getYMin();

  const CGnuPlotPoint &point = plot->getPoint2D(minI);

  CPoint2D p;

  if (! point.getPoint(p))
    return false;

  CGnuPlotAxis *xaxis = plot->group()->getPlotAxis(CGnuPlotTypes::AxisType::X, plot->xind(), true);
  CGnuPlotAxis *yaxis = plot->group()->getPlotAxis(CGnuPlotTypes::AxisType::Y, plot->xind(), true);

  std::string xStr = xaxis->data().formatAxisValue(p.x);
  std::string yStr = yaxis->data().formatAxisValue(p.y);

  std::string tipStr = "X=" + xStr + ", Y=" + yStr;

  double x = (probeEvent.isNearest() ? p.x : probeEvent.x());

  probeEvent.addLine(CGnuPlotProbeEvent::Line(x, ymin, p.y, p.y, tipStr));

  return true;
}
