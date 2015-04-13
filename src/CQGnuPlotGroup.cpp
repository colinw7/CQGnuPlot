#include <CQGnuPlotGroup.h>
#include <CQGnuPlotWindow.h>
#include <CQGnuPlotDevice.h>
#include <CQGnuPlotRenderer.h>
#include <CQGnuPlotUtil.h>
#include <CQGnuPlotLabel.h>
#include <CQGnuPlotRenderer.h>
#include <CGnuPlotObject.h>

CQGnuPlotGroup::
CQGnuPlotGroup(CQGnuPlotWindow *window) :
 CGnuPlotGroup(window), window_(window)
{
  setObjectName("group");
}

CQGnuPlotGroup::
~CQGnuPlotGroup()
{
}

void
CQGnuPlotGroup::
setPainter(QPainter *p)
{
  CQGnuPlotRenderer *renderer = qwindow()->qapp()->qdevice()->qrenderer();

  renderer->setPainter(p);
}

void
CQGnuPlotGroup::
redraw()
{
  window_->redraw();
}

double
CQGnuPlotGroup::
getRatio() const
{
  return plotSize_.xratio.getValue(1);
}

void
CQGnuPlotGroup::
setRatio(double r)
{
  plotSize_.xratio = r;
}

CQGnuPlot::HistogramStyle
CQGnuPlotGroup::
histogramStyle() const
{
  const CGnuPlotHistogramData &data = CGnuPlotGroup::getHistogramData();

  return CQGnuPlotUtil::histogramStyleConv(data.style());
}

void
CQGnuPlotGroup::
setHistogramStyle(const CQGnuPlot::HistogramStyle &s)
{
  CGnuPlotHistogramData data = CGnuPlotGroup::getHistogramData();

  data.setStyle(CQGnuPlotUtil::histogramStyleConv(s));

  CGnuPlotGroup::setHistogramData(data);
}

CQGnuPlot::DrawLayerType
CQGnuPlotGroup::
getBorderLayer() const
{
  return CQGnuPlotUtil::drawLayerTypeConv(CGnuPlotGroup::getBorderLayer());
}

void
CQGnuPlotGroup::
setBorderLayer(const DrawLayerType &layer)
{
  CGnuPlotGroup::setBorderLayer(CQGnuPlotUtil::drawLayerTypeConv(layer));
}

void
CQGnuPlotGroup::
draw()
{
  CGnuPlotGroup::draw();

  if (isSelected()) {
    CGnuPlotRenderer *renderer = app()->renderer();

    renderer->drawRect(getBBox(), CRGBA(1,0,0), 2);
  }
}

void
CQGnuPlotGroup::
mousePress(const QPoint &qp)
{
  CGnuPlotRenderer *renderer = app()->renderer();

  renderer->setRegion(region());

  for (auto &plot : plots()) {
    plot->initRenderer();

    CPoint2D p;

    renderer->pixelToWindow(CPoint2D(qp.x(), qp.y()), p);

    unmapLogPoint(&p.x, &p.y);

    CQGnuPlotPlot *qplot = static_cast<CQGnuPlotPlot *>(plot);

    qplot->mousePress(p);
  }

  renderer->setRange(getDisplayRange(1, 1));

  for (auto &annotation : annotations()) {
    CPoint2D p;

    renderer->pixelToWindow(CPoint2D(qp.x(), qp.y()), p);

    unmapLogPoint(&p.x, &p.y);

    if (annotation->inside(p)) {
      CQGnuPlotLabel *qann = static_cast<CQGnuPlotLabel *>(annotation.get());

      qwindow()->selectObject(qann);

      return;
    }
  }
}

void
CQGnuPlotGroup::
mouseMove(const QPoint &qp)
{
  CGnuPlotRenderer *renderer = app()->renderer();

  renderer->setRegion(region());

  for (auto &plot : plots()) {
    plot->initRenderer();

    CPoint2D p;

    renderer->pixelToWindow(CPoint2D(qp.x(), qp.y()), p);

    unmapLogPoint(&p.x, &p.y);

    CQGnuPlotPlot *qplot = static_cast<CQGnuPlotPlot *>(plot);

    qplot->mouseMove(p);
  }
}

bool
CQGnuPlotGroup::
mouseTip(const QPoint &qp, CQGnuPlot::TipRect &tip)
{
  CGnuPlotRenderer *renderer = app()->renderer();

  renderer->setRegion(region());

  for (auto &plot : plots()) {
    plot->initRenderer();

    CPoint2D p;

    renderer->pixelToWindow(CPoint2D(qp.x(), qp.y()), p);

    unmapLogPoint(&p.x, &p.y);

    CQGnuPlotPlot *qplot = static_cast<CQGnuPlotPlot *>(plot);

    if (qplot->mouseTip(p, tip))
      return true;
  }

  return false;
}
