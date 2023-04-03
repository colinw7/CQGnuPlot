#include <CGnuPlotObject.h>
#include <CGnuPlotGroup.h>
#include <CGnuPlotPlot.h>

CGnuPlotGroupObject::
CGnuPlotGroupObject(CGnuPlotGroup *group) :
 CGnuPlotObject(group ? group->app() : nullptr), group_(group)
{
}

//---

CGnuPlotPlotObject::
CGnuPlotPlotObject(CGnuPlotPlot *plot) :
 CGnuPlotObject(plot ? plot->app() : nullptr), plot_(plot)
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
inside(const CGnuPlotMouseEvent &mouseEvent) const
{
  return bbox().inside(mouseEvent.window());
}

void
CGnuPlotObject::
setBBox(const CBBox2D &)
{
  std::cerr << "Not implemented" << std::endl;
}
