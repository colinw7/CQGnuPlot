#include <CQGnuPlotAxis.h>
#include <CQGnuPlotGroup.h>
#include <CQGnuPlotUtil.h>
#include <CGnuPlotRenderer.h>

CQGnuPlotAxis::
CQGnuPlotAxis(CQGnuPlotGroup *group, const std::string &id, COrientation dir,
              double start, double end) :
 CGnuPlotAxis(group, id, dir, start, end)
{
}

CQGnuPlotAxis::
~CQGnuPlotAxis()
{
}

QString
CQGnuPlotAxis::
getLabel() const
{
  return QString(CGnuPlotAxis::getLabel().c_str());
}

void
CQGnuPlotAxis::
setLabel(const QString &label)
{
  CGnuPlotAxis::setLabel(label.toStdString());
}

void
CQGnuPlotAxis::
setStart(double r)
{
  group_->setAxisStart(id_, r);
}

void
CQGnuPlotAxis::
setEnd(double r)
{
  group_->setAxisEnd(id_, r);
}

CQGnuPlot::DrawLayerType
CQGnuPlotAxis::
getGridLayer() const
{
  return CQGnuPlotUtil::drawLayerTypeConv(CGnuPlotAxis::getGridLayer());
}

void
CQGnuPlotAxis::
setGridLayer(const DrawLayerType &layer)
{
  CGnuPlotAxis::setGridLayer(CQGnuPlotUtil::drawLayerTypeConv(layer));
}

void
CQGnuPlotAxis::
drawAxis(double pos, bool first)
{
  CGnuPlotAxis::drawAxis(pos, first);

  if (isSelected()) {
    CGnuPlotRenderer *renderer = app()->renderer();

    renderer->drawRect(getBBox(), CRGBA(1,0,0), 2);
  }
}
