#include <CGnuPlotStyleChordDiagram.h>
#include <CGnuPlotChordDiagramStyleValue.h>
#include <CGnuPlotStyleValueMgr.h>
#include <CGnuPlotGroup.h>
#include <CGnuPlotPlot.h>
#include <CGnuPlotDevice.h>

//------

CGnuPlotStyleChordDiagram::
CGnuPlotStyleChordDiagram() :
 CGnuPlotStyleBase(CGnuPlot::PlotStyle::CHORDDIAGRAM)
{
  CGnuPlotStyleValueMgrInst->setId<CGnuPlotChordDiagramStyleValue>("chord_diagram");
}

void
CGnuPlotStyleChordDiagram::
draw2D(CGnuPlotPlot *plot, CGnuPlotRenderer *renderer)
{
  CGnuPlotChordDiagramStyleValue *value =
    CGnuPlotStyleValueMgrInst->getValue<CGnuPlotChordDiagramStyleValue>(plot);

  if (! value) {
    value = plot->app()->device()->createChordDiagramStyleValue(plot);

    value->initRenderer(renderer);

    CGnuPlotStyleValueMgrInst->setValue<CGnuPlotChordDiagramStyleValue>(plot, value);
  }

  //---

  plot->group()->setMargin(CGnuPlotMargin(0, 0, 0, 0));

  ChordDiagram *chord = value->chordDiagram();

  if (! value->isInited()) {
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

      if (point.hasParam("colorInd"))
        chord->setColorInd(iy, point.getParamInt("colorInd"));

      ++iy;
    }

    chord->init();

    value->setInited(true);
  }

  //---

  CGnuPlotStyleChordDiagramRenderer *crenderer = value->renderer();

  crenderer->setRenderer(renderer);

  crenderer->setPalette(value->palette());

  chord->draw(crenderer);
}

bool
CGnuPlotStyleChordDiagram::
mouseTip(CGnuPlotPlot *plot, const CGnuPlotMouseEvent &mouseEvent, CGnuPlotTipData &tipData)
{
  CGnuPlotChordDiagramStyleValue *value =
    CGnuPlotStyleValueMgrInst->getValue<CGnuPlotChordDiagramStyleValue>(plot);
  if (! value) return false;

  CGnuPlotStyleChordDiagramRenderer *cr = value->renderer();

  std::string name;
  CRGBA       c;

  if (! cr->getValueAtPos(mouseEvent.window(), name, c))
    return false;

  tipData.setBorderColor(c);

  tipData.setXStr(name);
  tipData.setYStr("");

  return true;
}

void
CGnuPlotStyleChordDiagram::
mousePress(CGnuPlotPlot *plot, const CGnuPlotMouseEvent &mouseEvent)
{
  CGnuPlotChordDiagramStyleValue *value =
    CGnuPlotStyleValueMgrInst->getValue<CGnuPlotChordDiagramStyleValue>(plot);
  if (! value) return;

  CGnuPlotStyleChordDiagramRenderer *cr = value->renderer();

  int ind = cr->getIndAtPos(mouseEvent.window());

  cr->setCurrentInd(ind);

  plot->app()->device()->redraw();
}

CBBox2D
CGnuPlotStyleChordDiagram::
fit(CGnuPlotPlot *)
{
  return CBBox2D(-1, -1, 1, 1);
}
