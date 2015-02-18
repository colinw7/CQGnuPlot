#include <CGnuPlotLabel.h>
#include <CGnuPlotRenderer.h>
#include <CGnuPlotGroup.h>
#include <CFontMgr.h>

CGnuPlotLabel::
CGnuPlotLabel(CGnuPlotGroup *group) :
 CGnuPlotGroupAnnotation(group)
{
  font_ = CFontMgrInst->lookupFont("helvetica", CFONT_STYLE_NORMAL, 12);
}

void
CGnuPlotLabel::
draw() const
{
  CGnuPlotRenderer *renderer = group_->app()->renderer();

  //CVAlignType valign = (getFront() ? CVALIGN_TYPE_TOP : CVALIGN_TYPE_CENTER);
  CVAlignType valign = CVALIGN_TYPE_CENTER;

  if (font_.isValid())
    renderer->setFont(font_);

  renderer->drawHAlignedText(getPos(), getAlign(), 0, valign, 0, getText(), getStrokeColor());
}

bool
CGnuPlotLabel::
inside(const CPoint2D &) const
{
  return false;
}
