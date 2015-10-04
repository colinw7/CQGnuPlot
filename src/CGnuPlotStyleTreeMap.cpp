#include <CGnuPlotStyleTreeMap.h>
#include <CGnuPlotPlot.h>
#include <CGnuPlotGroup.h>
#include <CGnuPlotWindow.h>
#include <CGnuPlotRenderer.h>
#include <CGnuPlotKey.h>
#include <CGnuPlotRectObject.h>
#include <CTreeMap.h>

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

class CGnuPlotStyleTreeMapPainter : public CTreeMapPainter {
 public:
  CGnuPlotStyleTreeMapPainter(CGnuPlotPlot *plot, CGnuPlotRenderer *renderer) :
   plot_(plot), renderer_(renderer), ind_(0) {
  }

  void drawNode(CTreeMap::Node *node) {
    if (node->isHier()) return;

    double      x    = node->x();
    double      y    = node->y();
    double      w    = node->w();
    double      h    = node->h();
    std::string text = node->name();

    CTreeMap::HierNode *parent = node->parent();

    int pcolorId = parent->colorId();

    //CRGBA c = CGnuPlotStyleInst->indexColor(node->colorId());
    CRGBA c1 = getColor(pcolorId    );
    CRGBA c2 = getColor(pcolorId + 1);

    double r = 0.0;

    if (parent->getNodes().size() > 1)
      r = (node->colorId() - 1.0)/(parent->getNodes().size() - 1);

    CRGBA c = c1 + r*c2;

    if (! renderer_->isPseudo()) {
      CGnuPlotRectObject *rect = plot_->rectObjects()[ind_];

      rect->setRect     (CBBox2D(x, y, x + w, y + h));
      rect->setFillColor(c);
      rect->setLineColor(CRGBA(0,0,0));
      rect->setLineWidth(0);
      rect->setText     (text);
      rect->setTipText  (text + " : " + CStrUtil::toString(node->size()));

      ++ind_;
    }
  }

 private:
  CGnuPlotPlot     *plot_;
  CGnuPlotRenderer *renderer_;
  int               ind_;
};

//------

CGnuPlotStyleTreeMap::
CGnuPlotStyleTreeMap() :
 CGnuPlotStyleBase(CGnuPlot::PlotStyle::TREEMAP)
{
}

void
CGnuPlotStyleTreeMap::
draw2D(CGnuPlotPlot *plot, CGnuPlotRenderer *renderer)
{
  typedef std::map<std::string, CTreeMap::HierNode *> GroupMap;

  if (renderer->isPseudo())
    return;

  CTreeMap tree;
  GroupMap groups;

  plot->updateRectCacheSize(plot->getPoints2D().size());

  int i = 1;

  for (const auto &p : plot->getPoints2D()) {
    std::string name;
    double      value;

    if (! p.getValue(1, name) || ! p.getValue(2, value))
      continue;

    std::string group;

    if (p.getValue(3, group)) {
      auto p = groups.find(group);

      if (p == groups.end()) {
        CTreeMap::HierNode *hierNode = tree.addHierNode(group);

        p = groups.insert(p, GroupMap::value_type(group, hierNode));

        hierNode->setColorId(groups.size());
      }

      CTreeMap::HierNode *hierNode = (*p).second;

      CTreeMap::Node *node = tree.addNode(hierNode, name, value);

      node->setColorId(hierNode->getNodes().size());
    }
    else {
      CTreeMap::Node *node = tree.addNode(name, value);

      node->setColorId(i);
    }

    ++i;
  }

  tree.place(0, 0, 1, 1);

  CGnuPlotStyleTreeMapPainter painter(plot, renderer);

  tree.draw(&painter);

  for (const auto &rect : plot->rectObjects())
    rect->draw(renderer);
}

void
CGnuPlotStyleTreeMap::
drawKey(CGnuPlotPlot *, CGnuPlotRenderer *)
{
}

CBBox2D
CGnuPlotStyleTreeMap::
fit(CGnuPlotPlot *)
{
  return CBBox2D(0, 0, 1, 1);
}
