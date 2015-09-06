#ifndef CGnuPlotStyleAdjacencyRenderer_H
#define CGnuPlotStyleAdjacencyRenderer_H

#include <CAdjacency.h>

class CGnuPlotStyleAdjacencyRenderer : public CAdjacencyRenderer {
 public:
  CGnuPlotStyleAdjacencyRenderer(CGnuPlotRenderer *renderer=0, CAdjacency *adjacency=0) :
   renderer_(renderer), adjacency_(adjacency) {
  }

  CGnuPlotRenderer *renderer() const { return renderer_; }
  void setRenderer(CGnuPlotRenderer *r) { renderer_ = r; }

  int pixelWidth () const { return renderer_->width (); }
  int pixelHeight() const { return renderer_->height(); }

  void setFontSize(int h) {
    renderer_->setFontSize(h);
  }

  int textWidth(const std::string &str) {
    return renderer_->getFont()->getStringWidth(str);
  }

  int charDescent() {
    return renderer_->getFont()->getCharDescent();
  }

  void clearRects() { rects_.clear(); }

  void fillRect(const CBBox2D &r, const CRGBA &c) {
    renderer_->fillRect(r, c);
  }

  void drawNodeRect(const CBBox2D &r, const CRGBA &c, int value,
                    CAdjacencyNode *node1, CAdjacencyNode *node2) {
    CRGBA lc = c.getLightRGBA();

    renderer_->fillRect(r, c);
    renderer_->drawRect(r, lc, 1);

    rects_.push_back(ValueNodeNodeRect(value,
      NodeNodeRect(node1, NodeRect(node2, r))));
  }

  void drawText(const CPoint2D &p, const std::string &text, const CRGBA &c) {
    renderer_->drawText(p, text, c);
  }

  void drawRotatedText(const CPoint2D &p, const std::string &text, double a,
                       CHAlignType halign, CVAlignType valign, const CRGBA &c) {
    renderer_->drawRotatedText(p, text, a, halign, valign, c);
  }

  bool getValueAtPos(const CPoint2D &p, int &value, std::string &name1,
                     std::string &name2, CRGBA &c) {
    for (const auto &r : rects_) {
      const NodeNodeRect &nnr = r  .second;
      const NodeRect     &nr  = nnr.second;

      if (nr.second.inside(p)) {
        value = r  .first;
        name1 = nnr.first->name();
        name2 = nr .first->name();
        c     = adjacency_->nodeColor(nnr.first, nr.first);
        return true;
      }
    }

    return false;
  }

 public:
  typedef std::pair<CAdjacencyNode*,CBBox2D>  NodeRect;
  typedef std::pair<CAdjacencyNode*,NodeRect> NodeNodeRect;
  typedef std::pair<int,NodeNodeRect>         ValueNodeNodeRect;
  typedef std::vector<ValueNodeNodeRect>      ValueNodeNodeRects;

  CGnuPlotRenderer   *renderer_;
  CAdjacency         *adjacency_;
  ValueNodeNodeRects  rects_;
};

#endif
