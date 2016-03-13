#ifndef CGnuPlotAdjacencyStyleValue_H
#define CGnuPlotAdjacencyStyleValue_H

#include <CGnuPlotStyleValue.h>
#include <CGnuPlotStyleAdjacencyRenderer.h>
#include <string>

class CGnuPlotAdjacencyStyleValue : public CGnuPlotStyleValue {
 public:
  CGnuPlotAdjacencyStyleValue(CGnuPlotPlot *plot=0) :
   CGnuPlotStyleValue(plot) {
  }

 ~CGnuPlotAdjacencyStyleValue() {
    delete adjacency_;
    delete renderer_;
  }

  void initRenderer(CGnuPlotRenderer *renderer) {
    adjacency_ = new CAdjacency;
    renderer_  = new CGnuPlotStyleAdjacencyRenderer(renderer, adjacency_);
  }

  CAdjacency *adjacency() const { return adjacency_; }
  void setAdjacency(CAdjacency *a) { adjacency_ = a; }

  CGnuPlotStyleAdjacencyRenderer *renderer() const { return renderer_; }
  void setRenderer(CGnuPlotStyleAdjacencyRenderer *r) { renderer_ = r; }

  bool isInited() const { return inited_; }
  void setInited(bool b) { inited_ = b; }

  const std::string &palette() const { return palette_; }
  void setPalette(const std::string &v) { palette_ = v; }

 private:
  CAdjacency                     *adjacency_ { 0 };
  CGnuPlotStyleAdjacencyRenderer *renderer_  { 0 };
  bool                            inited_    { false };
  std::string                     palette_   { "adjacency" };
};

#endif
