#include <CGnuPlotPm3D.h>

CGnuPlotPm3D::
CGnuPlotPm3D(CGnuPlotGroup *group) :
 group_(group)
{
}

void
CGnuPlotPm3D::
setData(const CGnuPlotPm3DData &data)
{
  data_= data;
}
