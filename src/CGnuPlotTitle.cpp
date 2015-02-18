#include <CGnuPlotTitle.h>
#include <CGnuPlotGroup.h>
#include <CGnuPlotRenderer.h>
#include <CFontMgr.h>

CGnuPlotTitle::
CGnuPlotTitle(CGnuPlotGroup *group) :
 group_(group)
{
  font_ = CFontMgrInst->lookupFont("helvetica", CFONT_STYLE_NORMAL, 12);
}

void
CGnuPlotTitle::
draw() const
{
  if (text_.empty()) return;

  CGnuPlotRenderer *renderer = group_->app()->renderer();

  const CBBox2D &region = group_->region();

  double xmin = region.getXMin();
  double xmax = region.getXMax();
//double ymin = region.getYMin();
  double ymax = region.getYMax();

  CPoint2D p((xmin + xmax)/2, ymax);

  CRGBA c = color_.color();

  if (font_.isValid())
    renderer->setFont(font_);

  renderer->drawHAlignedText(p, CHALIGN_TYPE_CENTER, 0, CVALIGN_TYPE_BOTTOM, -8, text_, c);
}
