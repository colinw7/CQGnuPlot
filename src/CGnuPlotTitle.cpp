#include <CGnuPlotTitle.h>
#include <CGnuPlotGroup.h>
#include <CGnuPlotRenderer.h>
#include <CGnuPlotDevice.h>
#include <CFontMgr.h>

CGnuPlotTitle::
CGnuPlotTitle(CGnuPlotGroup *group) :
 group_(group)
{
  font_ = CFontMgrInst->lookupFont("helvetica", CFONT_STYLE_NORMAL, 12);
}

void
CGnuPlotTitle::
draw(CGnuPlotRenderer *renderer) const
{
  if (text_.empty()) return;

  const CBBox2D &region = group_->region();

  double xmin = region.getXMin();
  double xmax = region.getXMax();
//double ymin = region.getYMin();
  double ymax = region.getYMax();

  CPoint2D p((xmin + xmax)/2, ymax);

  p += offset_;

  CRGBA c = color_.color();

  if (font_.isValid())
    renderer->setFont(font_);

  if (isEnhanced()) {
    CGnuPlotText text(text_);

    CBBox2D bbox = text.calcBBox(renderer);

    CBBox2D bbox1 = bbox.moveBy(p + CPoint2D(-bbox.getWidth()/2, bbox.getHeight()));

    bbox1.setYMax(p.y + bbox.getHeight());

    text.draw(renderer, bbox1, CHALIGN_TYPE_CENTER, c);
  }
  else
    renderer->drawHAlignedText(p, CHALIGN_TYPE_CENTER, 0, CVALIGN_TYPE_BOTTOM, -8, text_, c);
}
