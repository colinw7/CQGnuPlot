#include <CGnuPlotStyleAdjacency.h>
#include <CGnuPlotPlot.h>
#include <CGnuPlotGroup.h>
#include <CGnuPlotRenderer.h>
#include <CAdjacency.h>

class CGnuPlotStyleAdjacencyRenderer : public CAdjacencyRenderer {
 public:
  CGnuPlotStyleAdjacencyRenderer(CGnuPlotRenderer *renderer) :
   renderer_(renderer) {
 }

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

  void fillRect(const CBBox2D &r, const CRGBA &c) {
    renderer_->fillRect(r, c);
  }

  void drawRect(const CBBox2D &r, const CRGBA &c) {
    renderer_->drawRect(r, c, 1);
  }

  void drawText(const CPoint2D &p, const std::string &text, const CRGBA &c) {
    renderer_->drawText(p, text, c);
  }

  void drawRotatedText(const CPoint2D &p, const std::string &text, double a,
                       CHAlignType halign, CVAlignType valign, const CRGBA &c) {
    renderer_->drawRotatedText(p, text, a, halign, valign, c);
  }

 public:
  CGnuPlotRenderer *renderer_;
};

//---

CGnuPlotStyleAdjacency::
CGnuPlotStyleAdjacency() :
 CGnuPlotStyleBase(CGnuPlot::PlotStyle::ADJACENCY)
{
}

void
CGnuPlotStyleAdjacency::
draw2D(CGnuPlotPlot *plot, CGnuPlotRenderer *renderer)
{
  plot->group()->setMargin(CGnuPlot::Margin(0, 0, 0, 0));

  CAdjacency adjacency;

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

      adjacency.addNode(id, name, group);
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

      adjacency.connectNodes(source, target, value);
    }
  }

  CGnuPlotStyleAdjacencyRenderer arenderer(renderer);

  adjacency.sortNodes();

  adjacency.draw(&arenderer);
}

CBBox2D
CGnuPlotStyleAdjacency::
fit(CGnuPlotPlot *)
{
  return CBBox2D(0, 0, 1, 1);
}
