#include <CGnuPlotStyleDelaunay.h>
#include <CGnuPlotPlot.h>
#include <CGnuPlotGroup.h>
#include <CGnuPlotWindow.h>
#include <CGnuPlotRenderer.h>
#include <CGnuPlotKey.h>
#include <CDelaunay.h>

class CGnuPlotStyleDelaunayValue : public CGnuPlotPlot::StyleValue {
 public:
  CGnuPlotStyleDelaunayValue(CDelaunay *delaunay) :
   delaunay_(delaunay) {
  }

  CDelaunay *delaunay() const { return delaunay_; }

 private:
  CDelaunay *delaunay_;
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
  const CGnuPlotClip &clip = plot->group()->clip();

  CGnuPlotStyleDelaunayValue *value =
    dynamic_cast<CGnuPlotStyleDelaunayValue *>(plot->styleValue("delaunay"));

  CDelaunay *delaunay;

  if (! value) {
    delaunay = new CDelaunay;

    value = new CGnuPlotStyleDelaunayValue(delaunay);

    plot->setStyleValue("delaunay", value);

    //---

    for (const auto &point : plot->getPoints2D()) {
      std::vector<double> reals;

      (void) point.getReals(reals);

      if (reals.size() != 2)
        continue;

      CPoint2D p(reals[0], reals[1]);

      if (clip.isOn() && ! clip.rect().inside(p))
        continue;

      delaunay->addVertex(p.x, p.y);
    }

    delaunay->calc();
  }

  //---

  delaunay = value->delaunay();

  //---

  double size = plot->pointSize();

  for (CHull3D::VertexIterator pv = delaunay->verticesBegin();
         pv != delaunay->verticesEnd(); ++pv) {
    const CHull3D::Vertex *v = *pv;

    CPoint2D p(v->x(), v->y());

    CRGBA c(1,0,0);

    renderer->drawSymbol(p, plot->pointType(), size, c);
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
  for (CHull3D::EdgeIterator pve = delaunay->voronoiEdgesBegin();
         pve != delaunay->voronoiEdgesEnd(); ++pve) {
    const CHull3D::Edge *e = *pve;

    auto *v1 = e->start();
    auto *v2 = e->end  ();

    CRGBA c(0,0,0.8);

    renderer->drawLine(CPoint2D(v1->x(), v1->y()), CPoint2D(v2->x(), v2->y()), 1, c);
  }
}

void
CGnuPlotStyleDelaunay::
drawKey(CGnuPlotPlot *, CGnuPlotRenderer *)
{
}

CBBox2D
CGnuPlotStyleDelaunay::
fit(CGnuPlotPlot *)
{
  return CBBox2D(-1, -1, 1, 1);
}
