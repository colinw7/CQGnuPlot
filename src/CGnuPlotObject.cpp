#include <CGnuPlotObject.h>
#include <CGnuPlotGroup.h>
#include <CGnuPlotPlot.h>

CGnuPlotGroupObject::
CGnuPlotGroupObject(CGnuPlotGroup *group) :
 CGnuPlotObject(group ? group->app() : 0), group_(group)
{
}

//---

CGnuPlotPlotObject::
CGnuPlotPlotObject(CGnuPlotPlot *plot) :
 CGnuPlotObject(plot ? plot->app() : 0), plot_(plot)
{
}

//---

CGnuPlotObject::
CGnuPlotObject(CGnuPlot *app) :
 app_(app)
{
}

bool
CGnuPlotObject::
inside(const CGnuPlotTypes::InsideData &data) const
{
  return bbox().inside(data.window);
}

void
CGnuPlotObject::
setBBox(const CBBox2D &)
{
  std::cerr << "Not implemented" << std::endl;
}
