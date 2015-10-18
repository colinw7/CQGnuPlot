#include <CGnuPlotStyleChordDiagram.h>
#include <CGnuPlotGroup.h>
#include <CGnuPlotPlot.h>
#include <CGnuPlotDevice.h>
#include <ChordDiagram.h>
#include <CGnuPlotStyleChordDiagramRenderer.h>

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
  plot->group()->setMargin(CGnuPlotMargin(0, 0, 0, 0));

  ChordDiagram *chord = plot->chordDiagramData().chordDiagram();

  if (! chord) {
    chord = new ChordDiagram;

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

    chord->setDimension(dim);

    int iy = 0;

    for (const auto &point : plot->getPoints2D()) {
      std::vector<double> reals;

      if (! point.getReals(reals))
        continue;

      if (ny > dim) break;

      reals.resize(dim);

      for (int ix = 0; ix < dim; ++ix)
        chord->setValue(ix, iy, reals[ix]);

      if (point.hasParam("name"))
        chord->setName(iy, point.getParamString("name"));

      if (point.hasParam("color"))
        chord->setColor(iy, point.getParamColor("color"));

      ++iy;
    }

    chord->init();

    plot->setChordDiagram(chord);
  }

  //---

  CGnuPlotStyleChordDiagramRenderer *cr = plot->chordDiagramData().renderer();

  if (! cr) {
    cr = new CGnuPlotStyleChordDiagramRenderer(renderer, chord);

    plot->setChordDiagramRenderer(cr);
  }
  else
    cr->setRenderer(renderer);

  chord->draw(cr);
}

bool
CGnuPlotStyleChordDiagram::
mouseTip(CGnuPlotPlot *plot, const CGnuPlotTypes::InsideData &insideData, CGnuPlotTipData &tipData)
{
  CGnuPlotStyleChordDiagramRenderer *cr = plot->chordDiagramData().renderer();

  std::string name;
  CRGBA       c;

  if (! cr->getValueAtPos(insideData.window, name, c))
    return false;

  tipData.setBorderColor(c);

  tipData.setXStr(name);
  tipData.setYStr("");

  return true;
}

void
CGnuPlotStyleChordDiagram::
mousePress(CGnuPlotPlot *plot, const CGnuPlotTypes::InsideData &insideData)
{
  CGnuPlotStyleChordDiagramRenderer *cr = plot->chordDiagramData().renderer();

  int ind = cr->getIndAtPos(insideData.window);

  cr->setCurrentInd(ind);

  plot->app()->device()->redraw();
}

CBBox2D
CGnuPlotStyleChordDiagram::
fit(CGnuPlotPlot *)
{
  return CBBox2D(-1, -1, 1, 1);
}
