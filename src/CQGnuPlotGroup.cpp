#include <CQGnuPlotGroup.h>
#include <CQGnuPlotWindow.h>
#include <CQGnuPlotPlot.h>
#include <CQGnuPlotCanvas.h>
#include <CQGnuPlotDevice.h>
#include <CQGnuPlotRenderer.h>
#include <CQGnuPlotEnum.h>
#include <CQGnuPlotLabel.h>
#include <CQGnuPlotKey.h>
#include <CQGnuPlotRenderer.h>
#include <CQGnuPlotArrow.h>
#include <CQGnuPlotCircle.h>
#include <CQGnuPlotEllipse.h>
#include <CQGnuPlotPolygon.h>
#include <CQGnuPlotRectangle.h>
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

  return CQGnuPlotEnum::histogramStyleConv(data.style());
}

void
CQGnuPlotGroup::
setHistogramStyle(const CQGnuPlotEnum::HistogramStyle &s)
{
  CGnuPlotHistogramData data = CGnuPlotGroup::getHistogramData();

  data.setStyle(CQGnuPlotEnum::histogramStyleConv(s));

  CGnuPlotGroup::setHistogramData(data);
}

CQGnuPlotEnum::DrawLayerType
CQGnuPlotGroup::
getBorderLayer() const
{
  return CQGnuPlotEnum::drawLayerTypeConv(CGnuPlotGroup::getBorderLayer());
}

void
CQGnuPlotGroup::
setBorderLayer(const DrawLayerType &layer)
{
  CGnuPlotGroup::setBorderLayer(CQGnuPlotEnum::drawLayerTypeConv(layer));
}

void
CQGnuPlotGroup::
draw()
{
  CGnuPlotGroup::draw();

  if (isSelected()) {
    CGnuPlotRenderer *renderer = app()->renderer();

    if (! is3D())
      renderer->drawRect(bbox2D(), CRGBA(1,0,0), 2);
    else
      renderer->drawRect(bbox3D(), CRGBA(1,0,0), 2);
  }
}

void
CQGnuPlotGroup::
mousePress(const CGnuPlotMouseEvent &mouseEvent)
{
  if (! inside(mouseEvent)) return;

  CGnuPlotRenderer *renderer = app()->renderer();

  renderer->setRegion(region());

  typedef std::vector<QObject *> Objects;

  Objects objects;

  //---

  for (const auto &vann : varAnnotations_) {
    for (const auto &ann : vann.second) {
      if (ann->inside(mouseEvent))
        continue;

      CGnuPlotArrow     *arrow   = nullptr;
      CGnuPlotCircle    *circle  = nullptr;
      CGnuPlotEllipse   *ellipse = nullptr;
      CGnuPlotLabel     *label   = nullptr;
      CGnuPlotPolygon   *poly    = nullptr;
      CGnuPlotRectangle *rect    = nullptr;

      if      ((arrow = dynamic_cast<CGnuPlotArrow *>(ann.get()))) {
        CQGnuPlotArrow *qarrow = static_cast<CQGnuPlotArrow *>(arrow);

        objects.push_back(qarrow);
      }
      else if ((circle = dynamic_cast<CGnuPlotCircle *>(ann.get()))) {
        CQGnuPlotCircle *qcircle = static_cast<CQGnuPlotCircle *>(circle);

        objects.push_back(qcircle);
      }
      else if ((ellipse = dynamic_cast<CGnuPlotEllipse *>(ann.get()))) {
        CQGnuPlotEllipse *qellipse = static_cast<CQGnuPlotEllipse *>(ellipse);

        objects.push_back(qellipse);
      }
      else if ((label = dynamic_cast<CGnuPlotLabel *>(ann.get()))) {
        CQGnuPlotLabel *qlabel = static_cast<CQGnuPlotLabel *>(label);

        objects.push_back(qlabel);
      }
      else if ((poly = dynamic_cast<CGnuPlotPolygon *>(ann.get()))) {
        CQGnuPlotPolygon *qpoly = static_cast<CQGnuPlotPolygon *>(poly);

        objects.push_back(qpoly);
      }
      else if ((rect = dynamic_cast<CGnuPlotRectangle *>(ann.get()))) {
        CQGnuPlotRectangle *qrect = static_cast<CQGnuPlotRectangle *>(rect);

        objects.push_back(qrect);
      }
    }
  }

  //---

  for (auto &plot : plots()) {
    if (! plot->isDisplayed())
      continue;

    plot->initRenderer(renderer);

    CPoint2D window;

    renderer->pixelToWindow(mouseEvent.pixel(), window);

    double z = 0;

    unmapLogPoint(1, 1, 1, &window.x, &window.y, &z);

    CQGnuPlotPlot *qplot = static_cast<CQGnuPlotPlot *>(plot.get());

    CGnuPlotMouseEvent mouseEvent2 = mouseEvent;

    mouseEvent2.setWindow(window);

    qplot->mousePress(mouseEvent2);

    qplot->mouseObjects(mouseEvent2, objects);
  }

  renderer->setRange(getMappedDisplayRange(1, 1));

  //---

  CGnuPlotMouseEvent mouseEvent1 = mouseEvent;

  CPoint2D window;

  renderer->pixelToWindow(mouseEvent.pixel(), window);

  mouseEvent1.setWindow(window);

  //---

  for (auto &vann : annotations()) {
    for (auto &annotation : vann.second) {
      if (annotation->getLayer() == CGnuPlotTypes::DrawLayer::BEHIND)
        continue;

      double z = 0;

      unmapLogPoint(1, 1, 1, &window.x, &window.y, &z);

      if (annotation->inside(mouseEvent1)) {
        CQGnuPlotLabel *qann = static_cast<CQGnuPlotLabel *>(annotation.get());

        objects.push_back(qann);
      }
    }
  }

  if (! objects.empty()) {
    qwindow()->selectObjects(objects);
  }
  else {
    CQGnuPlotKey *qkey = dynamic_cast<CQGnuPlotKey *>(key().get());

    if (qkey) {
      if (qkey->inside(mouseEvent1)) {
        if (! qkey->mousePress(mouseEvent1))
          objects.push_back(qkey);
      }
    }

    if (! objects.empty())
      qwindow()->selectObjects(objects);
  }

  //---

  CGnuPlotGroup::mousePress(mouseEvent1);
}

void
CQGnuPlotGroup::
mouseMove(const CGnuPlotMouseEvent &mouseEvent, bool pressed)
{
  if (! inside(mouseEvent)) return;

  CGnuPlotRenderer *renderer = app()->renderer();

  renderer->setRegion(region());

  //---

  for (auto &plot : plots()) {
    if (! plot->isDisplayed())
      continue;

    plot->initRenderer(renderer);

    CPoint2D window;

    renderer->pixelToWindow(mouseEvent.pixel(), window);

    double z = 0;

    unmapLogPoint(1, 1, 1, &window.x, &window.y, &z);

    CQGnuPlotPlot *qplot = static_cast<CQGnuPlotPlot *>(plot.get());

    CGnuPlotMouseEvent mouseEvent1 = mouseEvent;

    mouseEvent1.setWindow(window);

    qplot->mouseMove(mouseEvent1, pressed);
  }
}

void
CQGnuPlotGroup::
mouseRelease(const CGnuPlotMouseEvent &mouseEvent)
{
  if (! inside(mouseEvent)) return;

  CGnuPlotRenderer *renderer = app()->renderer();

  renderer->setRegion(region());

  //---

  for (auto &plot : plots()) {
    if (! plot->isDisplayed())
      continue;

    plot->initRenderer(renderer);

    CPoint2D window;

    renderer->pixelToWindow(mouseEvent.pixel(), window);

    double z = 0;

    unmapLogPoint(1, 1, 1, &window.x, &window.y, &z);

    CQGnuPlotPlot *qplot = static_cast<CQGnuPlotPlot *>(plot.get());

    CGnuPlotMouseEvent mouseEvent1 = mouseEvent;

    mouseEvent1.setWindow(window);

    qplot->mouseRelease(mouseEvent1);
  }
}

bool
CQGnuPlotGroup::
mouseTip(const CGnuPlotMouseEvent &mouseEvent, CGnuPlotTipData &tip)
{
  if (! inside(mouseEvent)) return false;

  CGnuPlotRenderer *renderer = app()->renderer();

  renderer->setRegion(region());

  for (auto &plot : plots()) {
    if (! plot->isDisplayed())
      continue;

    plot->initRenderer(renderer);

    CPoint2D window;

    renderer->pixelToWindow(mouseEvent.pixel(), window);

    double z = 0;

    unmapLogPoint(1, 1, 1, &window.x, &window.y, &z);

    CQGnuPlotPlot *qplot = static_cast<CQGnuPlotPlot *>(plot.get());

    CGnuPlotMouseEvent mouseEvent1 = mouseEvent;

    mouseEvent1.setWindow(window);

    if (qplot->mouseTip(mouseEvent1, tip))
      return true;
  }

  renderer->setRange(getMappedDisplayRange(1, 1));

  //---

  CPoint2D window;

  renderer->pixelToWindow(mouseEvent.pixel(), window);

  for (auto &vann : annotations()) {
    for (auto &annotation : vann.second) {
      if (annotation->getLayer() == CGnuPlotTypes::DrawLayer::BEHIND)
        continue;

      double z = 0;

      unmapLogPoint(1, 1, 1, &window.x, &window.y, &z);

      CQGnuPlotAnnotation *qann = dynamic_cast<CQGnuPlotAnnotation *>(annotation.get());

      CGnuPlotMouseEvent mouseEvent1 = mouseEvent;

      mouseEvent1.setWindow(window);

      if (qann->mouseTip(mouseEvent1, tip))
        return true;
    }
  }

  //---

  return false;
}

void
CQGnuPlotGroup::
keyPress(const CGnuPlotKeyEvent &keyEvent)
{
  CGnuPlotRenderer *renderer = app()->renderer();

  renderer->setRegion(region());

  CPoint2D window;

  renderer->pixelToWindow(keyEvent.pixel(), window);

  CGnuPlotKeyEvent keyEvent1 = keyEvent;

  keyEvent1.setWindow(window);

  CGnuPlotGroup::keyPress(keyEvent1);
}

void
CQGnuPlotGroup::
moveObjects(int key)
{
  for (const auto &vann : varAnnotations_) {
    for (const auto &annotation : vann.second) {
      CQGnuPlotAnnotation *qann = dynamic_cast<CQGnuPlotAnnotation *>(annotation.get());

      if (qann->obj()->isSelected())
        qann->move(key);
    }
  }

  //---

  for (auto &plot : plots()) {
    if (! plot->isDisplayed())
      continue;

    CQGnuPlotPlot *qplot = static_cast<CQGnuPlotPlot *>(plot.get());

    qplot->moveObjects(key);
  }
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

void
CQGnuPlotGroup::
windowToPixel(const CPoint2D &w, CPoint2D &p)
{
  QPointF pos(w.x, w.y);

  CGnuPlotRenderer *renderer = app()->renderer();

  renderer->setRegion(region());

  renderer->setRange(getMappedDisplayRange(1, 1));

  CPoint2D w1 = w;

  double z = 0;

  mapLogPoint(1, 1, 1, &w1.x, &w1.y, &z);

  renderer->windowToPixel(w1, p);
}

bool
CQGnuPlotGroup::
inside(const CGnuPlotMouseEvent &mouseEvent) const
{
  double xr = CGnuPlotUtil::map(mouseEvent.pixel().x, 0, qwindow()->pixelWidth () - 1, 0, 1);
  double yr = CGnuPlotUtil::map(mouseEvent.pixel().y, 0, qwindow()->pixelHeight() - 1, 1, 0);

  return region().inside(CPoint2D(xr, yr));
}

void
CQGnuPlotGroup::
fitSlot()
{
  CGnuPlotGroup::fit();

  window_->redraw();
}
