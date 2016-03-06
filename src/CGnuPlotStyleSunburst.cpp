#include <CGnuPlotStyleSunburst.h>
#include <CGnuPlotPlot.h>
#include <CGnuPlotGroup.h>
#include <CGnuPlotWindow.h>
#include <CGnuPlotRenderer.h>
#include <CGnuPlotKey.h>
#include <CSunburst.h>
#include <CFontMgr.h>
#include <CGnuPlotPieObject.h>

namespace {

CRGBA getColor(int i) {
#if 0
  static int numColors = 20;

  static CRGBA bg_colors[] = {
    // blue
    CRGBName::toRGBA("#3182BD"),
    CRGBName::toRGBA("#6BAED6"),
    CRGBName::toRGBA("#9ECAE1"),
    CRGBName::toRGBA("#C6DBEF"),

    // orange
    CRGBName::toRGBA("#E6550D"),
    CRGBName::toRGBA("#FD8D3C"),
    CRGBName::toRGBA("#FDAE6B"),
    CRGBName::toRGBA("#FDD0A2"),

    // green
    CRGBName::toRGBA("#31A354"),
    CRGBName::toRGBA("#74C476"),
    CRGBName::toRGBA("#A1D99B"),
    CRGBName::toRGBA("#C7E9C0"),

    // purple
    CRGBName::toRGBA("#756BB1"),
    CRGBName::toRGBA("#9E9AC8"),
    CRGBName::toRGBA("#BCBDDC"),
    CRGBName::toRGBA("#DADAEB"),

    // gray
    CRGBName::toRGBA("#636363"),
    CRGBName::toRGBA("#969696"),
    CRGBName::toRGBA("#BDBDBD"),
    CRGBName::toRGBA("#D9D9D9"),
  };
#endif
  static int numColors = 5;

  static CRGBA bg_colors[] = {
    CRGBName::toRGBA("#756BB1"), // purple
    CRGBName::toRGBA("#3182BD"), // blue
    CRGBName::toRGBA("#31A354"), // green
    CRGBName::toRGBA("#E6550D"), // orange
    CRGBName::toRGBA("#636363"), // gray
  };

  return bg_colors[i % numColors];
}

#if 0
CRGBA getBorderColor() {
  //static CRGBA border_color = CRGBA("#2078B4");
  static CRGBA border_color = CRGBName::toRGBA("#FFFFFF");

  return border_color;
}
#endif

//CRGBA fg_color = CRGBA("#FF7F0E");

}

//------

class CGnuPlotStyleSunburstPainter : public CSunburstPainter {
 public:
  typedef std::pair<CHAlignType,double> HAlignPos;
  typedef std::pair<CVAlignType,double> VAlignPos;

 public:
  CGnuPlotStyleSunburstPainter(CGnuPlotPlot *plot, CGnuPlotRenderer *renderer) :
   plot_(plot), renderer_(renderer) {
    assert(plot_);
  }

  void drawNode(CSunburst::Node *node) {
    double xc = 0.0;
    double yc = 0.0;

    double r1 = node->r();
    double r2 = r1 + node->dr();

    double a1 = node->a();
    double da = node->da();
    double a2 = a1 + da;

    CSunburst::HierNode *parent = 0;

    if (node->parent() && ! node->parent()->isRoot())
      parent = node->parent();

    CSunburst::HierNode *root = parent;

    while (root && root->parent() && ! root->parent()->isRoot())
      root = root->parent();

    int rcolorId = (root ? root->colorId() : node->colorId());

    double r = 0.0;

    if (node->isHier())
      r = (parent ? (1.0*parent->childIndex(node))/(parent->getChildren().size() - 1.0) : 0.0);
    else
      r = (parent ? (1.0*parent->nodeIndex(node))/(parent->getNodes().size() - 1.0) : 0.0);

    CPoint2D pc(xc, yc);

    CRGBA color = getColor(rcolorId) + r*CRGBA(0.5, 0.5, 0.5);

    if (plot_->isCacheActive()) {
      CGnuPlotPieObject *pie = plot_->pieObjects()[ind_];

      pie->setCenter(pc);
      pie->setRadius(r2);
      pie->setInnerRadius(r1/r2);
      pie->setAngle1(a1);
      pie->setAngle2(a2);

      if (! pie->testAndSetUsed()) {
        CGnuPlotFillP   fill  (pie->fill  ()->dup());
        CGnuPlotStrokeP stroke(pie->stroke()->dup());

        pie->setName       (node->name());
        pie->setValue      (node->size());
        pie->setLabelRadius((r1 + 0.01)/r2);

        pie->setRotatedText    (true);
        pie->setExplodeSelected(false);

        fill->setType (CGnuPlotTypes::FillType::SOLID);
        fill->setColor(color);

        stroke->setWidth(1);
        stroke->setColor(CRGBA(0,0,0));

        pie->setFill  (fill  );
        pie->setStroke(stroke);
      }
    }
    else {
      double ta = a1 + da/2.0;

      double tangle = CAngle::Deg2Rad(ta);

      double lr = r1 + 0.01;

      double tc = cos(tangle);
      double ts = sin(tangle);

      double tx = pc.x + lr*tc;
      double ty = pc.y + lr*ts;

      CPoint2D tp(tx, ty);

      CRGBA tc1(0,0,0);

      //---

      renderer_->fillPieSlice(pc, r1, r2, a1, a2, color);
      renderer_->drawPieSlice(pc, r1, r2, a1, a2, CRGBA(0,0,0), 1);

#if 0
      renderer_->drawText(tp, node->name(), tc1);
#endif

      if (tc >= 0)
        renderer_->drawRotatedText(tp, node->name(), ta,
                                   HAlignPos(CHALIGN_TYPE_LEFT  , 0),
                                   VAlignPos(CVALIGN_TYPE_CENTER, 0), tc1);
      else
        renderer_->drawRotatedText(tp, node->name(), 180.0 + ta,
                                   HAlignPos(CHALIGN_TYPE_RIGHT , 0),
                                   VAlignPos(CVALIGN_TYPE_CENTER, 0), tc1);
    }

    ++ind_;
  }

 private:
  CGnuPlotPlot     *plot_     { 0 };
  CGnuPlotRenderer *renderer_ { 0 };
  int               ind_      { 0 };
};

//------

class CGnuPlotStyleSunburstVisitor : public CSunburstVisitor {
 public:
  CGnuPlotStyleSunburstVisitor() { }

  int numNodes() const { return numNodes_; }

  void visitNode(CSunburst::Node *) {
    ++numNodes_;
  }

 private:
  int numNodes_ { 0 };
};

//------

CGnuPlotStyleSunburst::
CGnuPlotStyleSunburst() :
 CGnuPlotStyleBase(CGnuPlot::PlotStyle::SUNBURST)
{
}

void
CGnuPlotStyleSunburst::
draw2D(CGnuPlotPlot *plot, CGnuPlotRenderer *renderer)
{
  if (renderer->isPseudo())
    return;

  //---

  CFontPtr font = CFontMgrInst->lookupFont("helvetica", CFONT_STYLE_NORMAL, 6);
  renderer->setFont(font);

  CSunburst *sunburst = plot->sunburstData().sunburst();

  if (! sunburst) {
    sunburst = new CSunburst;

    plot->setSunburst(sunburst);

    //---

    typedef std::map<std::string, CSunburst::HierNode *> GroupMap;

    GroupMap  groups;

    sunburst->addRoot();

    int nr = 0;

    for (const auto &p : plot->getPoints2D()) {
      int nv = p.getNumValues();

      if (nv < 2) continue;

      std::string key;

      CSunburst::HierNode *parentNode = 0;

      for (int i = 0; i < nv - 2; ++i) {
        std::string group;

        if (! p.getValue(i + 1, group))
          continue;

        if (key != "")
          key += "#";

        key += group;

        auto p = groups.find(key);

        CSunburst::HierNode *hierNode = 0;

        if (p == groups.end()) {
          if (parentNode) {
            hierNode = sunburst->addHierNode(parentNode, group);

            hierNode->setColorId(-1);
          }
          else {
            hierNode = sunburst->addHierNode(group);

            ++nr;

            hierNode->setColorId(nr);
          }

          p = groups.insert(p, GroupMap::value_type(key, hierNode));
        }

        parentNode = (*p).second;
      }

      std::string name;

      if (! p.getValue(nv - 1, name))
        continue;

      double value;

      if (! p.getValue(nv, value))
        continue;

      std::string group;

      CSunburst::Node *node = 0;

      if (parentNode)
        node = sunburst->addNode(parentNode, name, value);
      else
        node = sunburst->addNode(name, value);

      node->setColorId(-1);
    }

    sunburst->pack();
  }

  //---

  if (! renderer->isPseudo()) {
    CGnuPlotStyleSunburstVisitor visitor;

    sunburst->visit(&visitor);

    plot->updatePieCacheSize(visitor.numNodes());
  }

  CGnuPlotStyleSunburstPainter painter(plot, renderer);

  sunburst->draw(&painter);

  if (! renderer->isPseudo()) {
    for (const auto &pie : plot->pieObjects())
      pie->draw(renderer);
  }
}

void
CGnuPlotStyleSunburst::
drawKey(CGnuPlotPlot *, CGnuPlotRenderer *)
{
}

CBBox2D
CGnuPlotStyleSunburst::
fit(CGnuPlotPlot *)
{
  return CBBox2D(-1, -1, 1, 1);
}
