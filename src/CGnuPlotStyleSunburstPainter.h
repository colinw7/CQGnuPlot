#ifndef CGnuPlotStyleSunburstPainter_H
#define CGnuPlotStyleSunburstPainter_H

#include <CSunburst.h>
#include <CGnuPlotRenderer.h>
#include <CGnuPlotPlot.h>
#include <CGnuPlotPieObject.h>

class CGnuPlotStyleSunburstPainter : public CSunburstPainter {
 public:
  typedef std::pair<CHAlignType,double> HAlignPos;
  typedef std::pair<CVAlignType,double> VAlignPos;

 public:
  CGnuPlotStyleSunburstPainter(CGnuPlotPlot *plot, CGnuPlotRenderer *renderer) :
   plot_(plot), renderer_(renderer) {
    assert(plot_);
  }

  const CRGBA &borderColor() const { return borderColor_; }
  void setBorderColor(const CRGBA &v) { borderColor_ = v; }

  const std::string &palette() const { return palette_; }
  void setPalette(const std::string &v) { palette_ = v; }

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
        stroke->setColor(borderColor());

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

      CRGBA tc1 = color.bwContrast();

      //---

      renderer_->fillPieSlice(pc, r1, r2, a1, a2, color);
      renderer_->drawPieSlice(pc, r1, r2, a1, a2, borderColor(), 1);

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
  CRGBA getColor(int i) {
    return CGnuPlotStyleInst->indexColor(palette(), i);
  }

 private:
  CGnuPlotPlot     *plot_        { 0 };
  CGnuPlotRenderer *renderer_    { 0 };
  int               ind_         { 0 };
  CRGBA             borderColor_ { 0, 0, 0 };
  std::string       palette_     { "subburst" };
};

#endif
