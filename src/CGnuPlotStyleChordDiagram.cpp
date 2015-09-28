#include <CGnuPlotStyleChordDiagram.h>
#include <CGnuPlotPlot.h>
#include <CGnuPlotRenderer.h>
#include <ChordDiagram.h>

class CGnuPlotStyleChordDiagramRenderer : public ChordDiagramRenderer {
 public:
  CGnuPlotStyleChordDiagramRenderer(CGnuPlotRenderer *renderer) :
   renderer_(renderer) {
  }

  double pixelLengthToWindowLength(double l) {
    return renderer_->pixelWidthToWindowWidth(l);
  }

  void drawLine(double x1, double y1, double x2, double y2) {
    renderer_->drawLine(CPoint2D(x1, y1), CPoint2D(x2, y2), 0, CRGBA(0,0,0));
  }

  void drawRotatedText(double x, double y, const std::string &text, double a,
                       CHAlignType halign, CVAlignType valign) {
    renderer_->drawRotatedText(CPoint2D(x, y), text, a, halign, valign, CRGBA(0,0,0));
  }

  void drawArc(const CPoint2D &p, double r1, double r2, double a1, double a2, const CRGBA &c) {
    renderer_->drawArc(p, r1, r2, a1, a2, c);
  }

  void drawChord(const CPoint2D &p, double r, double a1, double a2, const CRGBA &c) {
    renderer_->drawChord(p, r, a1, a2, c);
  }

  void drawComplexChord(const CPoint2D &p, double r, double a11, double a12,
                        double a21, double a22, const CRGBA &c) {
    renderer_->drawComplexChord(p, r, a11, a12, a21, a22, c);
  }

 private:
  CGnuPlotRenderer *renderer_;
};

//------

CGnuPlotStyleChordDiagram::
CGnuPlotStyleChordDiagram() :
 CGnuPlotStyleBase(CGnuPlot::PlotStyle::CHORDDIAGRAM)
{
}

void
CGnuPlotStyleChordDiagram::
draw2D(CGnuPlotPlot *plot, CGnuPlotRenderer *renderer)
{
  ChordDiagram chord;

  CGnuPlotStyleChordDiagramRenderer cr(renderer);

  CRGBA fc(0.5,0.5,0.5,0.5);
  CRGBA lc(0,0,0);

  int ny = 0;
  int nx = -1;

  for (const auto &point : plot->getPoints2D()) {
    std::vector<double> reals;

    if (! point.getReals(reals))
      continue;

    int np = reals.size();

    nx = (nx >= 0 ? std::min(nx, np) : np);

    ++ny;
  }

  int dim = std::min(nx, ny);

  if (dim < 1)
    return;

  chord.setDimension(dim);

  int iy = 0;

  for (const auto &point : plot->getPoints2D()) {
    std::vector<double> reals;

    if (! point.getReals(reals))
      continue;

    if (ny > dim) break;

    reals.resize(dim);

    for (int ix = 0; ix < dim; ++ix)
      chord.setValue(ix, iy, reals[ix]);

    if (point.hasParam("name"))
      chord.setName(iy, point.getParamString("name"));

    if (point.hasParam("color"))
      chord.setColor(iy, point.getParamColor("color"));

    ++iy;
  }

  chord.init();

  chord.draw(&cr);
}

CBBox2D
CGnuPlotStyleChordDiagram::
fit(CGnuPlotPlot *)
{
  return CBBox2D(-1, -1, 1, 1);
}
