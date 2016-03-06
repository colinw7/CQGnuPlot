#include <CGnuPlotStyleForceDirected.h>
#include <CGnuPlotPlot.h>
#include <CGnuPlotGroup.h>
#include <CGnuPlotRenderer.h>
#include <CForceDirected.h>

class CGnuPlotStyleForceDirectedValue : public CGnuPlotPlot::StyleValue {
 public:
  CGnuPlotStyleForceDirectedValue() {
    forceDirected_ = new CForceDirected;
  }

 ~CGnuPlotStyleForceDirectedValue() {
    delete forceDirected_;
  }

  CForceDirected *forceDirected() const { return forceDirected_; }

  bool isInited() const { return inited_; }
  void setInited(bool b) { inited_ = b; }

  void init() {
    delete forceDirected_;

    forceDirected_ = new CForceDirected;
  }

 private:
  CForceDirected *forceDirected_ { 0 };
  bool            inited_        { false };
};

//------

static CRGBA colors[] = {
  CRGBA(CRGBA::IVal(),0x88,0x88,0x88),
  CRGBA(CRGBA::IVal(),0xFF,0x7F,0x0E),
  CRGBA(CRGBA::IVal(),0x2C,0xA0,0x2C),
  CRGBA(CRGBA::IVal(),0xD6,0x27,0x28),
  CRGBA(CRGBA::IVal(),0x94,0x67,0xBD),
  CRGBA(CRGBA::IVal(),0x8C,0x56,0x4B),
  CRGBA(CRGBA::IVal(),0xEA,0xCF,0xE2),
  CRGBA(CRGBA::IVal(),0xD1,0xD1,0xD1),
  CRGBA(CRGBA::IVal(),0xBC,0xBD,0x22),
  CRGBA(CRGBA::IVal(),0x4C,0xDA,0xD6),
  CRGBA(CRGBA::IVal(),0x1F,0x77,0xB4),
};

//------

CGnuPlotStyleForceDirected::
CGnuPlotStyleForceDirected() :
 CGnuPlotStyleBase(CGnuPlot::PlotStyle::FORCEDIRECTED)
{
}

void
CGnuPlotStyleForceDirected::
draw2D(CGnuPlotPlot *plot, CGnuPlotRenderer *renderer)
{
  CGnuPlotStyleForceDirectedValue *value =
    dynamic_cast<CGnuPlotStyleForceDirectedValue *>(plot->styleValue("force_directed"));

  if (! value) {
    value = new CGnuPlotStyleForceDirectedValue;

    plot->setStyleValue("force_directed", value);
  }

  //---

  CForceDirected *forceDirected { 0 };

  if (! value->isInited()) {
    value->init();

    forceDirected = value->forceDirected();

    int state = 0;

    for (const auto &point : plot->getPoints2D()) {
      if (point.isDiscontinuity())
        ++state;

      // node id, name and group
      if      (state == 0) {
        if (point.getNumValues() != 3)
          continue;

        int id = 0;

        if (! point.getValue(1, id))
          continue;

        std::string name;

        if (! point.getValue(2, name))
          continue;

        int group;

        if (! point.getValue(3, group))
          continue;

        Springy::Node *node = forceDirected->newNode(name);

        node->setMass(1.0);
        node->setValue(group);
      }
      // source, target, value
      else if (state == 1) {
        int source = 0;

        if (! point.getValue(1, source))
          continue;

        int target = 0;

        if (! point.getValue(2, target))
          continue;

        int value = 0;

        if (! point.getValue(3, value))
          continue;

        Springy::Node *node1 = forceDirected->getNode(source);
        Springy::Node *node2 = forceDirected->getNode(target);

        Springy::Edge *edge = forceDirected->newEdge(node1, node2);

        edge->setLength(1.0/value);
        edge->setValue(value);
      }
    }

    value->setInited(true);
  }
  else
    forceDirected = value->forceDirected();

  //---

  for (auto edge : forceDirected->edges()) {
    auto spring = forceDirected->spring(edge);

    const Springy::Vector &p1 = spring->point1()->p();
    const Springy::Vector &p2 = spring->point2()->p();

    //double px1, py1, px2, py2;

    //renderer_->windowToPixel(p1.x(), p1.y(), &px1, &py1);
    //renderer_->windowToPixel(p2.x(), p2.y(), &px2, &py2);

    double w = sqrt(edge->value());

    //renderer_.drawLine(CPoint2D(px1, py1), CPoint2D(px2, py2), CRGBA(0,0,0,0.5), 1);
    renderer->drawLine(CPoint2D(p1.x(), p1.y()), CPoint2D(p2.x(), p2.y()), CRGBA(0,0,0,0.5), w);
  }

  double CIRCLE_SIZE = 12;

  double dw = renderer->pixelWidthToWindowWidth  (CIRCLE_SIZE);
  double dh = renderer->pixelHeightToWindowHeight(CIRCLE_SIZE);

  for (auto node : forceDirected->nodes()) {
    auto point = forceDirected->point(node);

    const Springy::Vector &p1 = point->p();

    //double px, py;

    //renderer_->windowToPixel(p1.x(), p1.y(), &px, &py);

    CRGBA c;

    if (node == forceDirected->currentNode())
      c = CRGBA(1,0,0);
    else
      c = colors[node->value()];

    renderer->fillEllipse(CPoint2D(p1.x(), p1.y()), dw/2, dh/2, 0, c);
    renderer->drawEllipse(CPoint2D(p1.x(), p1.y()), dw/2, dh/2, 0, CRGBA(0,0,0), 1);
  }
}

CBBox2D
CGnuPlotStyleForceDirected::
fit(CGnuPlotPlot *)
{
  return CBBox2D(-20, -20, 20, 20);
}

void
CGnuPlotStyleForceDirected::
mousePress(CGnuPlotPlot *plot, const CGnuPlotMouseEvent &mouseEvent)
{
  pressed_ = true;

  CGnuPlotStyleForceDirectedValue *value =
    dynamic_cast<CGnuPlotStyleForceDirectedValue *>(plot->styleValue("force_directed"));

  if (! value)
    return;

  CForceDirected *forceDirected = value->forceDirected();

  Springy::Vector v(mouseEvent.window().x, mouseEvent.window().y);

  Springy::NodePoint nodePoint = forceDirected->nearest(v);

  forceDirected->setCurrentNode (nodePoint.first );
  forceDirected->setCurrentPoint(nodePoint.second);
}

void
CGnuPlotStyleForceDirected::
mouseMove(CGnuPlotPlot *plot, const CGnuPlotMouseEvent &mouseEvent, bool pressed)
{
  if (! pressed)
    return;

  CGnuPlotStyleForceDirectedValue *value =
    dynamic_cast<CGnuPlotStyleForceDirectedValue *>(plot->styleValue("force_directed"));

  if (! value)
    return;

  CForceDirected *forceDirected = value->forceDirected();

  Springy::Vector v(mouseEvent.window().x, mouseEvent.window().y);

  //Springy::NodePoint nodePoint = forceDirected->nearest(v);

  if (forceDirected->currentPoint())
    forceDirected->currentPoint()->setP(v);
}

void
CGnuPlotStyleForceDirected::
mouseRelease(CGnuPlotPlot *plot, const CGnuPlotMouseEvent &mouseEvent)
{
  pressed_ = false;

  CGnuPlotStyleForceDirectedValue *value =
    dynamic_cast<CGnuPlotStyleForceDirectedValue *>(plot->styleValue("force_directed"));

  if (! value)
    return;

  CForceDirected *forceDirected = value->forceDirected();

  Springy::Vector v(mouseEvent.window().x, mouseEvent.window().y);

  if (forceDirected->currentPoint())
    forceDirected->currentPoint()->setP(v);

  forceDirected->setCurrentNode (0);
  forceDirected->setCurrentPoint(0);
}

void
CGnuPlotStyleForceDirected::
animate(CGnuPlotPlot *plot) const
{
  if (pressed_)
    return;

  CGnuPlotStyleForceDirectedValue *value =
    dynamic_cast<CGnuPlotStyleForceDirectedValue *>(plot->styleValue("force_directed"));

  if (! value)
    return;

  CForceDirected *forceDirected = value->forceDirected();

  forceDirected->step(0.01);
}
