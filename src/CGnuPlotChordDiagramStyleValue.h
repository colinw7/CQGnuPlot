#ifndef CGnuPlotChordDiagramStyleValue_H
#define CGnuPlotChordDiagramStyleValue_H

#include <CGnuPlotStyleValue.h>
#include <CGnuPlotStyleChordDiagramRenderer.h>
#include <string>

class CGnuPlotChordDiagramStyleValue : public CGnuPlotStyleValue {
 public:
  CGnuPlotChordDiagramStyleValue(CGnuPlotPlot *plot=0) :
   CGnuPlotStyleValue(plot) {
  }

 ~CGnuPlotChordDiagramStyleValue() {
    delete chord_;
    delete renderer_;
  }

  void initRenderer(CGnuPlotRenderer *renderer) {
    chord_    = new ChordDiagram;
    renderer_ = new CGnuPlotStyleChordDiagramRenderer(renderer, chord_);
  }

  ChordDiagram *chordDiagram() const { return chord_; }
  void setChordDiagram(ChordDiagram *c) { chord_ = c; }

  CGnuPlotStyleChordDiagramRenderer *renderer() const { return renderer_; }
  void setRenderer(CGnuPlotStyleChordDiagramRenderer *r) { renderer_ = r; }

  bool isInited() const { return inited_; }
  void setInited(bool b) { inited_ = b; }

  const std::string &palette() const { return palette_; }
  void setPalette(const std::string &v) { palette_ = v; }

 private:
  ChordDiagram                      *chord_    { 0 };
  CGnuPlotStyleChordDiagramRenderer *renderer_ { 0 };
  bool                               inited_   { false };
  std::string                        palette_  { "chord" };
};

#endif
