#include <CGnuPlotAnnotation.h>

CGnuPlotGroupAnnotation::
CGnuPlotGroupAnnotation(CGnuPlotGroup *group) :
 CGnuPlotGroupObject(group)
{
}

CGnuPlotGroupAnnotation *
CGnuPlotGroupAnnotation::
setData(const CGnuPlotGroupAnnotation *ann)
{
  ind_         = ann->ind_;
  strokeColor_ = ann->strokeColor_;
  fillColor_   = ann->fillColor_;
  layer_       = ann->layer_;
  clip_        = ann->clip_;

  return this;
}
