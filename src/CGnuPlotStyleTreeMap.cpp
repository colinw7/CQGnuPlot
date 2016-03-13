#include <CGnuPlotStyleTreeMap.h>
#include <CGnuPlotTreeMapStyleValue.h>
#include <CGnuPlotStyleValueMgr.h>
#include <CGnuPlotPlot.h>
#include <CGnuPlotGroup.h>
#include <CGnuPlotWindow.h>
#include <CGnuPlotRenderer.h>
#include <CGnuPlotKey.h>
#include <CGnuPlotRectObject.h>
#include <CGnuPlotDevice.h>
#include <CTreeMap.h>

class CGnuPlotStyleTreeMapPainter : public CTreeMapPainter {
 public:
  CGnuPlotStyleTreeMapPainter(CGnuPlotPlot *plot, CGnuPlotRenderer *renderer, bool hier) :
   plot_(plot), renderer_(renderer), hier_(hier), ind_(0) {
  }

  void drawHierNode(CTreeMap::HierNode *node, int depth) {
    if (! hier_) return;

    CGnuPlotTreeMapStyleValue *value =
      CGnuPlotStyleValueMgrInst->getValue<CGnuPlotTreeMapStyleValue>(plot_);
    if (! value) return;

    if (renderer_->isPseudo()) return;

    //---

    double x = node->x();
    double y = node->y();
    double w = node->w();
    double h = node->h();

    CBBox2D bbox(x, y, x + w, y + h);

    if (node->isOpen()) {
      renderer_->drawRect(bbox, value->openBorderColor(), value->openBorderWidth());
    }
    else {
      std::string text = node->name() + "...";

      CRGBA c = CGnuPlotStyleInst->indexColor(value->palette(), depth);

      renderer_->fillRect(bbox, c);
      renderer_->drawRect(bbox, value->closedBorderColor(), value->closedBorderWidth());

      CRGBA tc = c.bwContrast();

      renderer_->drawHAlignedText(bbox.getCenter(),
                                  CGnuPlotTypes::HAlignPos(CHALIGN_TYPE_CENTER, 0),
                                  CGnuPlotTypes::VAlignPos(CVALIGN_TYPE_CENTER, 0),
                                  text, tc);
    }
  }

  void drawNode(CTreeMap::Node *node, int depth) {
    if (renderer_->isPseudo()) return;

    CGnuPlotTreeMapStyleValue *value =
      CGnuPlotStyleValueMgrInst->getValue<CGnuPlotTreeMapStyleValue>(plot_);
    if (! value) return;

    //---

    double x = node->x();
    double y = node->y();
    double w = node->w();
    double h = node->h();

    //---

    CBBox2D bbox(x, y, x + w, y + h);

    //---

    std::string text = node->name();

    CTreeMap::HierNode *parent = node->parent();

    int pcolorId = parent->colorId();

    CRGBA c;

    if (! hier_) {
      CRGBA c1 = CGnuPlotStyleInst->indexColor(value->palette(), pcolorId    );
      CRGBA c2 = CGnuPlotStyleInst->indexColor(value->palette(), pcolorId + 1);

      double r = 0.0;

      if (parent->getNodes().size() > 1)
        r = (node->colorId() - 1.0)/(parent->getNodes().size() - 1);

      c = c1 + r*c2;
    }
    else {
      CRGBA c1 = CGnuPlotStyleInst->indexColor(value->palette(), depth    );
    //CRGBA c2 = CGnuPlotStyleInst->indexColor(value->palette(), depth + 1);

    //c = c1 + r*c2;
      c = c1;
    }

    if (! hier_) {
      CGnuPlotRectObject *rect = plot_->rectObjects()[ind_];

      rect->setBBox   (bbox);
      rect->setText   (text);
      rect->setTipText(text + " : " + CGnuPlotUtil::toString(node->size()));

      if (! rect->testAndSetUsed()) {
        rect->fill()->setType (CGnuPlotTypes::FillType::SOLID);
        rect->fill()->setColor(c);

        rect->stroke()->setColor(value->openBorderColor());
        rect->stroke()->setWidth(value->openBorderWidth());
      }
    }
    else {
      renderer_->fillRect(bbox, c);
      renderer_->drawRect(bbox, value->closedBorderColor(), value->closedBorderWidth());

      CRGBA tc = c.bwContrast();

      renderer_->drawHAlignedText(bbox.getCenter(),
                                  CGnuPlotTypes::HAlignPos(CHALIGN_TYPE_CENTER, 0),
                                  CGnuPlotTypes::VAlignPos(CVALIGN_TYPE_CENTER, 0),
                                  text, tc);
    }

    ++ind_;
  }

 private:
  CGnuPlotPlot     *plot_;
  CGnuPlotRenderer *renderer_;
  bool              hier_;
  int               ind_;
};

//------

CGnuPlotStyleTreeMap::
CGnuPlotStyleTreeMap(bool hier) :
 CGnuPlotStyleBase(hier ? CGnuPlot::PlotStyle::HIERTREEMAP : CGnuPlot::PlotStyle::TREEMAP),
 hier_(hier)
{
  CGnuPlotStyleValueMgrInst->setId<CGnuPlotTreeMapStyleValue>("tree_map");
}

void
CGnuPlotStyleTreeMap::
draw2D(CGnuPlotPlot *plot, CGnuPlotRenderer *renderer)
{
  if (renderer->isPseudo())
    return;

  //---

  CGnuPlotTreeMapStyleValue *value =
    CGnuPlotStyleValueMgrInst->getValue<CGnuPlotTreeMapStyleValue>(plot);

  if (! value) {
    value = plot->app()->device()->createTreeMapStyleValue(plot);

    CGnuPlotStyleValueMgrInst->setValue<CGnuPlotTreeMapStyleValue>(plot, value);
  }

  //---

  CTreeMap *tree { 0 };

  if (! value->isInited()) {
    value->init();

    tree = value->tree();

    //---

    if (! hier_) {
      if (! tree->root())
        tree->createRootNode();

      typedef std::map<std::string, CTreeMap::HierNode *> GroupMap;

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
            CTreeMap::HierNode *hierNode = tree->addHierNode(group);

            p = groups.insert(p, GroupMap::value_type(group, hierNode));

            hierNode->setColorId(groups.size());
          }

          CTreeMap::HierNode *hierNode = (*p).second;

          CTreeMap::Node *node = tree->addNode(hierNode, name, value);

          node->setColorId(hierNode->getNodes().size());
        }
        else {
          CTreeMap::Node *node = tree->addNode(name, value);

          node->setColorId(i);
        }

        ++i;
      }
    }
    else {
      for (const auto &point : plot->getPoints2D()) {
        if (point.getNumValues() < 2)
          continue;

        // get names
        std::string name;

        if (! point.getValue(1, name))
          continue;

        std::vector<std::string> names;

        auto p = name.find('/');

        while (p != std::string::npos) {
          std::string lhs = name.substr(0, p);
          std::string rhs = name.substr(p + 1);

          names.push_back(lhs);

          name = rhs;

          p = name.find('/');
        }

        //---

        std::string op;
        double      value;

        if (name != "...") {
          if (! point.getValue(2, value))
            continue;
        }
        else {
          if (! point.getValue(2, op))
            continue;
        }

        //---

        // create nodes
        CTreeMap::HierNode *hierNode = 0;

        for (const auto &n : names) {
          if (! hierNode) {
            if (! tree->root()) {
              hierNode = tree->createRootNode(n);

              //hierNode->setOpen(false);
            }
            else
              hierNode = tree->root();
          }
          else {
            CTreeMap::HierNode *hierNode1 = hierNode->findChild(n);

            if (! hierNode1) {
              hierNode = tree->addHierNode(hierNode, n);

              //hierNode->setOpen(false);
            }
            else
              hierNode = hierNode1;
          }
        }

        if (name != "...") {
          if (! hierNode) {
            hierNode = tree->createRootNode(name);

            //hierNode->setOpen(false);
          }
          else
            (void) tree->addNode(hierNode, name, value);
        }
        else {
          if (! hierNode)
            hierNode = tree->root();

          if (op == "open") {
            //hierNode->setOpen(true);
          }
        }
      }
    }

    tree->place(0, 0, 1, 1);

    value->setInited(true);
  }
  else {
    tree = value->tree();
  }

  //---

  CGnuPlotStyleTreeMapPainter painter(plot, renderer, hier_);

  tree->draw(&painter);

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

bool
CGnuPlotStyleTreeMap::
mouseTip(CGnuPlotPlot *plot, const CGnuPlotMouseEvent &mouseEvent, CGnuPlotTipData &tipData)
{
  CGnuPlotTreeMapStyleValue *value =
    CGnuPlotStyleValueMgrInst->getValue<CGnuPlotTreeMapStyleValue>(plot);
  if (! value) return false;

  CTreeMap *tree = value->tree();

  CTreeMap::Node *node = tree->getNodeAt(mouseEvent.window().x, mouseEvent.window().y);

  if (! node)
    return false;

  tipData.setXStr(node->name());
  tipData.setYStr(CStrUtil::strprintf("%g", node->size()));

  return true;
}

void
CGnuPlotStyleTreeMap::
mousePress(CGnuPlotPlot *plot, const CGnuPlotMouseEvent &mouseEvent)
{
  CGnuPlotTreeMapStyleValue *value =
    CGnuPlotStyleValueMgrInst->getValue<CGnuPlotTreeMapStyleValue>(plot);
  if (! value) return;

  CTreeMap *tree = value->tree();

  CTreeMap::Node *node = tree->getNodeAt(mouseEvent.window().x, mouseEvent.window().y);

  if (! node)
    return;

  if (! hier_)
    return;

  if      (mouseEvent.button() == 1) {
    if (node->isHier())
      node->setOpen(true);

    //plot->clearRectCache();
  }
  else if (mouseEvent.button() == 3) {
    if (node->parent())
      node->parent()->setOpen(false);

    //plot->clearRectCache();
  }
}
