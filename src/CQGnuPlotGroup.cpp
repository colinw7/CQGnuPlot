#include <CQGnuPlotGroup.h>
#include <CQGnuPlotWindow.h>
#include <CQGnuPlotCanvas.h>
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

CQGnuPlotEnum::HistogramStyle
CQGnuPlotGroup::
histogramStyle() const
{
  const CGnuPlotHistogramData &data = CGnuPlotGroup::getHistogramData();

  return CQGnuPlotUtil::histogramStyleConv(data.style());
}

void
CQGnuPlotGroup::
setHistogramStyle(const CQGnuPlotEnum::HistogramStyle &s)
{
  CGnuPlotHistogramData data = CGnuPlotGroup::getHistogramData();

  data.setStyle(CQGnuPlotUtil::histogramStyleConv(s));

  CGnuPlotGroup::setHistogramData(data);
}

CQGnuPlotEnum::DrawLayerType
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
  if (! inside(qp)) return;

  CGnuPlotRenderer *renderer = app()->renderer();

  renderer->setRegion(region());

  typedef std::vector<CQGnuPlotObject *> Objects;

  Objects objects;

  for (auto &plot : plots()) {
    plot->initRenderer(renderer);

    CPoint2D p;

    renderer->pixelToWindow(CPoint2D(qp.x(), qp.y()), p);

    double z = 0;

    unmapLogPoint(1, 1, 1, &p.x, &p.y, &z);

    CQGnuPlotPlot *qplot = static_cast<CQGnuPlotPlot *>(plot);

    qplot->mousePress(p, objects);
  }

  renderer->setRange(getMappedDisplayRange(1, 1));

  for (auto &annotation : annotations()) {
    CPoint2D p;

    renderer->pixelToWindow(CPoint2D(qp.x(), qp.y()), p);

    double z = 0;

    unmapLogPoint(1, 1, 1, &p.x, &p.y, &z);

    if (annotation->inside(p)) {
      CQGnuPlotLabel *qann = static_cast<CQGnuPlotLabel *>(annotation.get());

      objects.push_back(qann);
    }
  }

  qwindow()->selectObjects(objects);
}

void
CQGnuPlotGroup::
mouseMove(const QPoint &qp)
{
  if (! inside(qp)) return;

  CGnuPlotRenderer *renderer = app()->renderer();

  renderer->setRegion(region());

  for (auto &plot : plots()) {
    plot->initRenderer(renderer);

    CPoint2D p;

    renderer->pixelToWindow(CPoint2D(qp.x(), qp.y()), p);

    double z = 0;

    unmapLogPoint(1, 1, 1, &p.x, &p.y, &z);

    CQGnuPlotPlot *qplot = static_cast<CQGnuPlotPlot *>(plot);

    qplot->mouseMove(p);
  }
}

void
CQGnuPlotGroup::
mouseRelease(const QPoint &)
{
  //if (! inside(qp)) return;
}

bool
CQGnuPlotGroup::
mouseTip(const QPoint &qp, CQGnuPlot::TipRect &tip)
{
  if (! inside(qp)) return false;

  CGnuPlotRenderer *renderer = app()->renderer();

  renderer->setRegion(region());

  for (auto &plot : plots()) {
    plot->initRenderer(renderer);

    CPoint2D p;

    renderer->pixelToWindow(CPoint2D(qp.x(), qp.y()), p);

    double z = 0;

    unmapLogPoint(1, 1, 1, &p.x, &p.y, &z);

    CQGnuPlotPlot *qplot = static_cast<CQGnuPlotPlot *>(plot);

    if (qplot->mouseTip(p, tip))
      return true;
  }

  return false;
}

void
CQGnuPlotGroup::
pixelToWindow(const CPoint2D &p, CPoint2D &w)
{
  QPoint pos(p.x, p.y);

  CGnuPlotRenderer *renderer = app()->renderer();

  renderer->setRegion(region());

  renderer->setRange(getMappedDisplayRange(1, 1));

  renderer->pixelToWindow(p, w);

  double z = 0;

  unmapLogPoint(1, 1, 1, &w.x, &w.y, &z);
}

bool
CQGnuPlotGroup::
inside(const QPoint &qp) const
{
  CQGnuPlotCanvas *canvas = qwindow()->canvas();

  double xr = CGnuPlotUtil::map(qp.x(), 0, canvas->width () - 1, 0, 1);
  double yr = CGnuPlotUtil::map(qp.y(), 0, canvas->height() - 1, 1, 0);

  return region().inside(CPoint2D(xr, yr));
}
