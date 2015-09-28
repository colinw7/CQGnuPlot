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
inside(const CGnuPlotTypes::InsideData &data) const
{
  double r = data.window.distanceTo(point_);

  if (r > pw_)
    return false;

  return true;
}

CGnuPlotTipData
CGnuPlotPointObject::
tip() const
{
  CGnuPlotTipData tip;

  tip.setXStr(CStrUtil::strprintf("%g", point_.x));

  if (size_.isValid())
    tip.setYStr(CStrUtil::strprintf("%g, %g", point_.y, size_.getValue()));
  else
    tip.setYStr(CStrUtil::strprintf("%g", point_.y));

  CPoint2D d(pw_, ph_);

  CBBox2D rect(point_ - d, point_ + d);

  tip.setBorderColor(color_);
  tip.setXColor(color_);

  tip.setRect(rect);

  return tip;
}

void
CGnuPlotPointObject::
draw(CGnuPlotRenderer *renderer) const
{
  bool highlighted = (isHighlighted() || isSelected());

  double size = size_.getValue(1);

  if (size <= 0)
    size = 1;

  if (highlighted)
    size = 1.5*size;

  double ss = sqrt(size);

  pw_ = ss/2;
  ph_ = ss/2;

  bool erasePoint = isErasePoint();

  if (highlighted)
    erasePoint = true;

  if (erasePoint) {
    CGnuPlotGroup *group = plot()->group();

    CRGBA bg = group->window()->backgroundColor();

    CPoint2D d(pw_, ph_);

    renderer->fillEllipse(CBBox2D(point_ - 2*d, point_ + 2*d), bg);
  }

  if (pointType_ == CGnuPlotTypes::SymbolType::STRING)
    renderer->drawHAlignedText(point_, CHALIGN_TYPE_CENTER, 0, CVALIGN_TYPE_CENTER, 0,
                               pointString_, color_);
  else
    renderer->drawSymbol(point_, pointType_, size, color_, lineWidth_, false);
}
