#include <CGnuPlotStyleDelaunay.h>
#include <CGnuPlotPlot.h>
#include <CGnuPlotGroup.h>
#include <CGnuPlotWindow.h>
#include <CGnuPlotRenderer.h>
#include <CGnuPlotKey.h>
#include <CDelaunay.h>

class CGnuPlotStyleDelaunayValue : public CGnuPlotPlot::StyleValue {
 public:
  CGnuPlotStyleDelaunayValue() {
    delaunay_ = new CDelaunay;
  }

 ~CGnuPlotStyleDelaunayValue() {
    delete delaunay_;
  }

  CDelaunay *delaunay() const { return delaunay_; }

  bool isInited() const { return inited_; }
  void setInited(bool b) { inited_ = b; }

  bool isClipped() const { return clipped_; }
  void setClipped(bool b) { clipped_ = b; }

  void init() {
    delete delaunay_;

    delaunay_ = new CDelaunay;
  }

 private:
  CDelaunay *delaunay_ { 0 };
  bool       inited_   { false };
  bool       clipped_  { false };
};

//------

CGnuPlotStyleDelaunay::
CGnuPlotStyleDelaunay() :
 CGnuPlotStyleBase(CGnuPlot::PlotStyle::DELAUNAY)
{
}

void
CGnuPlotStyleDelaunay::
draw2D(CGnuPlotPlot *plot, CGnuPlotRenderer *renderer)
{
  // just use points for range
  if (renderer->isPseudo()) {
    CRGBA c(0,0,0);

    for (const auto &point : plot->getPoints2D()) {
      std::vector<double> reals;

      (void) point.getReals(reals);

      if (reals.size() < 2)
        continue;

      CPoint2D p(reals[0], reals[1]);

      renderer->drawPoint(p, c);
    }

    return;
  }

  //---

  CGnuPlotGroup *group = plot->group();

  const CGnuPlotClip &clip = group->clip();

  const CGnuPlotLineStyle &lineStyle = plot->lineStyle();

  CGnuPlotStroke stroke(plot);

  //---

  CGnuPlotStyleDelaunayValue *value =
    dynamic_cast<CGnuPlotStyleDelaunayValue *>(plot->styleValue("delaunay"));

  if (! value) {
    value = new CGnuPlotStyleDelaunayValue;

    plot->setStyleValue("delaunay", value);
  }

  //---

  CDelaunay *delaunay { 0 };

  bool clipped = clip.isOn();

  if (! value->isInited() || clipped != value->isClipped()) {
    value->init();

    delaunay = value->delaunay();

    for (const auto &point : plot->getPoints2D()) {
      std::vector<double> reals;

      (void) point.getReals(reals);

      if (reals.size() < 2)
        continue;

      CPoint2D p(reals[0], reals[1]);

      if (clipped && ! clip.bbox().inside(p))
        continue;

      delaunay->addVertex(p.x, p.y);
    }

    delaunay->calc();

    value->setInited (true);
    value->setClipped(clipped);
  }

  delaunay = value->delaunay();

  //---

  double pointSize = plot->pointSize();

  CGnuPlotTypes::SymbolType pointType = lineStyle.calcPointType();

  CRGBA c = stroke.color();

  for (CHull3D::VertexIterator pv = delaunay->verticesBegin();
         pv != delaunay->verticesEnd(); ++pv) {
    const CHull3D::Vertex *v = *pv;

    CPoint2D p(v->x(), v->y());

    renderer->drawSymbol(p, pointType, pointSize, c, 1, true);
  }

  //---

#if 0
  for (CHull3D::FaceIterator pf = delaunay->facesBegin(); pf != delaunay->facesEnd(); ++pf) {
    const CHull3D::Face *f = *pf;

    if (! f->isLower()) continue;

    auto *v1 = f->vertex(0);
    auto *v2 = f->vertex(1);
    auto *v3 = f->vertex(2);

    std::vector<CPoint2D> points;

    points.push_back(CPoint2D(v1->x(), v1->y()));
    points.push_back(CPoint2D(v2->x(), v2->y()));
    points.push_back(CPoint2D(v3->x(), v3->y()));

    CRGBA c(0,0,0.8);

    renderer->drawPolygon(points, 1, c);
  }
#endif
  // TODO: calc line color
  CRGBA lc(0,0,0.8);

  for (CHull3D::EdgeIterator pve = delaunay->voronoiEdgesBegin();
         pve != delaunay->voronoiEdgesEnd(); ++pve) {
    const CHull3D::Edge *e = *pve;

    auto *v1 = e->start();
    auto *v2 = e->end  ();

    renderer->drawLine(CPoint2D(v1->x(), v1->y()), CPoint2D(v2->x(), v2->y()), 1, lc);
  }
}
