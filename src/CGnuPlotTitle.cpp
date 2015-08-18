#include <CGnuPlotTitle.h>
#include <CGnuPlotGroup.h>
#include <CGnuPlotRenderer.h>
#include <CGnuPlotDevice.h>
#include <CFontMgr.h>

CGnuPlotTitle::
CGnuPlotTitle(CGnuPlotGroup *group) :
 group_(group)
{
  setFont(CFontMgrInst->lookupFont("helvetica", CFONT_STYLE_NORMAL, 12));
}

void
CGnuPlotTitle::
draw(CGnuPlotRenderer *renderer) const
{
  if (text().empty()) return;

  if (font().isValid())
    renderer->setFont(font());

  double xmin, xmax, ymax;

  if (! group_->is3D() || group_->pm3D()->isEnabled()) {
    CBBox2D bbox = group_->getAxisBBox();

    xmin = bbox.getXMin();
    xmax = bbox.getXMax();
  //ymin = bbox.getYMin();
    ymax = bbox.getYMax();
  }
  else {
    const CBBox2D &region = group_->region();

    xmin = region.getXMin();
    xmax = region.getXMax();
  //ymin = region.getYMin();
    ymax = region.getYMax();
  }

//double pw = renderer->pixelWidthToWindowWidth  (1);
  double ph = renderer->pixelHeightToWindowHeight(1);

  CPoint2D p((xmin + xmax)/2, ymax);

  p += offset(); // offset units ?

  CRGBA c = color().color();

  if (isEnhanced()) {
    CGnuPlotText text(this->text());

    CBBox2D bbox = text.calcBBox(renderer);

    CBBox2D bbox1 = bbox.moveBy(p + CPoint2D(-bbox.getWidth()/2, bbox.getHeight()));

    bbox1.setYMax(p.y + bbox.getHeight());

    text.draw(renderer, bbox1, CHALIGN_TYPE_CENTER, c);

    bbox_ = bbox1;
  }
  else {
    CPoint2D p1(p.x, p.y - 8*ph);

    renderer->drawHAlignedText(p1, CHALIGN_TYPE_CENTER, 0,
                               CVALIGN_TYPE_BOTTOM, 0, this->text(), c);

    bbox_ = renderer->getHAlignedTextBBox(this->text()).moveBy(p1);
  }

  //renderer->drawRect(bbox_, CRGBA(1,0,0), 1);

  group_->updateMarginBBox(bbox_);
}
