#include <CGnuPlotPointObject.h>
#include <CGnuPlotWindow.h>
#include <CGnuPlotGroup.h>
#include <CGnuPlotPlot.h>
#include <CGnuPlotRenderer.h>
#include <CGnuPlotUtil.h>

CGnuPlotPointObject::
CGnuPlotPointObject(CGnuPlotPlot *plot) :
 CGnuPlotPlotObject(plot)
{
}

bool
CGnuPlotPointObject::
inside(const CGnuPlotMouseEvent &mouseEvent) const
{
  double r = mouseEvent.pixel().distanceTo(pc_);

  return (r < 4);
}

CGnuPlotTipData
CGnuPlotPointObject::
tip() const
{
  CGnuPlotTipData tip;

  tip.setXStr(CStrUtil::strprintf("%g", point().x));

  if (size().isValid())
    tip.setYStr(CStrUtil::strprintf("%g, %g", point().y, size().getValue()));
  else
    tip.setYStr(CStrUtil::strprintf("%g", point().y));

  tip.setBorderColor(color());
  tip.setXColor     (color());

  tip.setBBox(bbox_);

  return tip;
}

void
CGnuPlotPointObject::
draw(CGnuPlotRenderer *renderer) const
{
  if (! isVisible()) return;

  bool highlighted = (isHighlighted() || isSelected());

  double size = this->size().getValue(1);

  if (size <= 0)
    size = 1;

  double wxsize, wysize;

  if (isPixelSize()) {
    pw_ = 8*size;
    ph_ = 8*size;

    wxsize = renderer->pixelWidthToWindowWidth  (pw_);
    wysize = renderer->pixelHeightToWindowHeight(ph_);
  }
  else {
    double size1 = sqrt(size);

    pw_ = renderer->windowWidthToPixelWidth  (size1);
    ph_ = renderer->windowHeightToPixelHeight(size1);

    wxsize = size1;
    wysize = size1;
  }

  CRGBA c = color();

  double scale = 1;

  if (highlighted) {
    scale = 1.5;
    c     = plot_->window()->getLightRGBA(c);
  }

  bool erasePoint = isErasePoint();

  if (highlighted)
    erasePoint = true;

  CPoint2D d(scale*wxsize/2, scale*wysize/2);

  bbox_ = CBBox2D(point() - d, point() + d);

  //---

  if (renderer->clip().isValid() & ! renderer->clip().getValue().inside(bbox_))
    return;

  //---

  if (erasePoint) {
    CGnuPlotGroup *group = plot()->group();

    CRGBA bg = group->window()->backgroundColor();

    CBBox2D ebbox(point() - 2*d, point() + 2*d);

    renderer->fillEllipse(ebbox, bg);
  }

  if (pointType() == CGnuPlotTypes::SymbolType::STRING)
    renderer->drawHAlignedText(point(), HAlignPos(CHALIGN_TYPE_CENTER, 0),
                               VAlignPos(CVALIGN_TYPE_CENTER, 0), pointString(), c);
  else
    renderer->drawSymbol(point(), pointType(), scale*pw_/8.0, c, lineWidth(), true);

  //---

  double px, py;

  renderer->windowToPixel(point().x, point().y, &px, &py);

  pc_ = CPoint2D(px, py);
//renderer->drawRect(bbox_, CRGBA(1,0,0), 1);
}
