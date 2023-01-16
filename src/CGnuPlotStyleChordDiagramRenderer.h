#ifndef CGnuPlotStyleChordDiagramRenderer_H
#define CGnuPlotStyleChordDiagramRenderer_H

#include <CGnuPlotStyleChordDiagram.h>
#include <CGnuPlotRenderer.h>
#include <ChordDiagram.h>

class CGnuPlotStyleChordDiagramRenderer : public ChordDiagramRenderer {
 public:
  CGnuPlotStyleChordDiagramRenderer(CGnuPlotRenderer *renderer=0, ChordDiagram *chord=0) :
   renderer_(renderer), chord_(chord) {
  }

  const std::string &palette() const { return palette_; }
  void setPalette(const std::string &v) { palette_ = v; }

  void setRenderer(CGnuPlotRenderer *renderer) { renderer_ = renderer; }

  double pixelLengthToWindowLength(double l) override {
    return renderer_->pixelWidthToWindowWidth(l);
  }

  void drawLine(double x1, double y1, double x2, double y2) override {
    renderer_->drawLine(CPoint2D(x1, y1), CPoint2D(x2, y2), CRGBA(0,0,0), 0);
  }

  void drawRotatedText(double x, double y, const std::string &text, double a,
                       const HAlignPos &halignPos, const VAlignPos &valignPos) override {
    renderer_->drawRotatedText(CPoint2D(x, y), text, a, halignPos, valignPos, CRGBA(0,0,0));
  }

  void drawArc(const CPoint2D &p, double r1, double r2, double a1, double a2,
               const CRGBA &c) override {
    renderer_->drawArc(p, r1, r2, a1, a2, c);
  }

  void drawChord(const CPoint2D &p, double r, double a1, double a2,
                 const CRGBA &fc, const CRGBA &lc) override {
    renderer_->drawChord(p, r, a1, a2, fc, lc);
  }

  void drawComplexChord(const CPoint2D &p, double r, double a11, double a12,
                        double a21, double a22, const CRGBA &fc, const CRGBA &lc) override {
    renderer_->drawComplexChord(p, r, a11, a12, a21, a22, fc, lc);
  }

  int getIndAtPos(const CPoint2D &p) {
    return chord_->getIndAtPos(p.x, p.y);
  }

  bool getValueAtPos(const CPoint2D &p, std::string &name, CRGBA &c) {
    ChordDiagram::ValueSet *valueSet = chord_->getValueSetAtPos(p.x, p.y);
    if (! valueSet) return false;

    name = valueSet->name();
    c    = indexColor(valueSet->colorInd());

    return true;
  }

  void setCurrentInd(int ind) {
    chord_->setCurrentInd(ind);
  }

  CRGBA indexColor(int ind) const override {
    return CGnuPlotStyleInst->indexColor(palette(), ind);
  }

 private:
  CGnuPlotRenderer *renderer_ { 0 };
  ChordDiagram     *chord_    { 0 };
  std::string       palette_  { "chord" };
};

#endif
