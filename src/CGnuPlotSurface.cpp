#include <CGnuPlotSurface.h>
#include <CGnuPlotGroup.h>
#include <CGnuPlotPlot.h>
#include <CGnuPlotRenderer.h>
#include <CGnuPlotStroke.h>
#include <CGnuPlotPolygonObject.h>

CGnuPlotSurface::
CGnuPlotSurface(CGnuPlotPlot *plot) :
 plot_(plot)
{
}

void
CGnuPlotSurface::
setData(const COptReal &zmin, const COptReal &zmax, const ZPolygons &zpolygons,
        const IJPoints &ijpoints)
{
  zmin_      = zmin;
  zmax_      = zmax;
  zpolygons_ = zpolygons;
  ijpoints_  = ijpoints;
}

void
CGnuPlotSurface::
draw(CGnuPlotRenderer *renderer)
{
  bool pm3D = (plot_->getStyle() == CGnuPlot::PlotStyle::PM3D ||
               plot_->group()->pm3D()->isEnabled());

  CGnuPlotStroke stroke(plot_);

  CRGBA c = stroke.color();

  if (plot_->isSurfaceEnabled()) {
    if (plot_->group()->hidden3D()) {
      CRGBA fc(1,1,1);

      for (auto polys : zpolygons_) {
        for (auto poly : polys.second) {
          const CGnuPlotSurface::PointsColor &pc = poly.second;

          renderer->fillPolygon(pc.first, fc);
          renderer->drawPolygon(pc.first, 0, c, CLineDash());
        }
      }
    }
    else {
      int pattern = plot_->hidden3D().trianglePattern;

      int nx = ijpoints_.size();

      for (auto ijpoly : ijpoints_) {
        int ix = ijpoly.first;

        int ny = ijpoly.second.size();

        for (auto ipoly : ijpoly.second) {
          int iy = ipoly.first;

          const CGnuPlotSurface::Points &points = ipoly.second;

          if (pattern & 1) renderer->drawLine(points[0], points[1], 1, c);
          if (pattern & 2) renderer->drawLine(points[0], points[3], 1, c);
          if (pattern & 4) renderer->drawLine(points[0], points[2], 1, c);

          if (ix == nx - 1 || iy == ny - 1) {
            if (pattern & 2) renderer->drawLine(points[2], points[1], 1, c);

            if (pattern & 1) renderer->drawLine(points[2], points[3], 1, c);
          }
        }
      }
    }
  }

  if (pm3D) {
    int np = 0;

    for (auto polys : zpolygons_)
      np += polys.second.size();

    if (! renderer->isPseudo())
      plot_->updatePolygonCacheSize(np);

    int i = 0;

    for (auto polys : zpolygons_) {
      for (auto poly : polys.second) {
        CGnuPlotPolygonObject *polygon = 0;

        if (! renderer->isPseudo())
          polygon = plot_->polygonObjects()[i];

        double z = poly.first;

        const CGnuPlotSurface::PointsColor &pc = poly.second;

        CRGBA rgba;

        if      (isCalcColor()) {
          rgba = pc.second;
        }
        else if (isDataColor()) {
          rgba = pc.second;
        }
        else {
          double r = (z - zmin_.getValue())/(zmax_.getValue() - zmin_.getValue());

          CColor c = plot_->group()->palette()->getColor(r);

          rgba = c.rgba();
        }

        if (! renderer->isPseudo()) {
          polygon->setPoints(pc.first);

          if (! polygon->testAndSetUsed()) {
            CGnuPlotFillP fill(polygon->fill()->dup());

            polygon->setAliased(false);

            polygon->fill()->setColor(rgba);
            polygon->fill()->setType (CGnuPlotTypes::FillType::SOLID);

            polygon->stroke()->setEnabled(false);

            polygon->setTipText(CStrUtil::strprintf("%g", z));
          }
        }
        else {
          renderer->fillPolygon(pc.first, rgba);
        }

        ++i;
      }
    }

    if (! renderer->isPseudo()) {
      for (const auto &polygon : plot_->polygonObjects())
        polygon->draw(renderer);
    }
  }
}
