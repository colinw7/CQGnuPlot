#ifndef CGnuPlotStyleTreeMapPainter_H
#define CGnuPlotStyleTreeMapPainter_H

#include <CGnuPlotTreeMapStyleValue.h>
#include <CGnuPlotStyleValueMgr.h>
#include <CGnuPlotRenderer.h>
#include <CGnuPlotRectObject.h>
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
        r = (node->colorId() - 1.0)/double(parent->getNodes().size() - 1);

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

#endif
