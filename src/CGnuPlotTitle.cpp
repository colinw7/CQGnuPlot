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

  if (font_.isValid())
    renderer->setFont(font_);

  CBBox2D bbox = group_->getAxisBBox();

  double xmin = bbox.getXMin();
  double xmax = bbox.getXMax();
//double ymin = bbox.getYMin();
  double ymax = bbox.getYMax();

//double pw = renderer->pixelWidthToWindowWidth  (1);
  double ph = renderer->pixelHeightToWindowHeight(1);

  CPoint2D p((xmin + xmax)/2, ymax);

  p += offset_; // offset units ?

  CRGBA c = color_.color();

  if (isEnhanced()) {
    CGnuPlotText text(text_);

    CBBox2D bbox = text.calcBBox(renderer);

    CBBox2D bbox1 = bbox.moveBy(p + CPoint2D(-bbox.getWidth()/2, bbox.getHeight()));

    bbox1.setYMax(p.y + bbox.getHeight());

    text.draw(renderer, bbox1, CHALIGN_TYPE_CENTER, c);
  }
  else {
    CPoint2D p1(p.x, p.y - 8*ph);

    renderer->drawHAlignedText(p1, CHALIGN_TYPE_CENTER, 0, CVALIGN_TYPE_BOTTOM, 0, text_, c);
  }
}
