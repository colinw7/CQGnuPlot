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
  const CGnuPlotSurfaceData &surfaceData = plot_->surfaceData();

  bool pm3D = (plot_->style() == CGnuPlot::PlotStyle::PM3D ||
               plot_->group()->pm3D()->isEnabled());

  CGnuPlotStroke stroke(plot_);

  CRGBA c = stroke.color();

  if (surfaceData.isEnabled()) {
    if (plot_->group()->isHidden3D() && ! pm3D) {
      CRGBA fc = surfaceData.color();

      for (auto polys : zpolygons_) {
        double zt = polys.first;

        for (auto poly : polys.second) {
          const CGnuPlotSurface::PointsIndColor &pic = poly.second;
          const CGnuPlotSurface::PointsInd      &pi  = pic.first;

          const Points &points = pi.first;

          CPolygonOrientation orient = CMathGeom2D::PolygonOrientation(points);

          renderer->fillHiddenPolygon(zt, points, fc);

          if (orient == 1)
            renderer->drawHiddenPolygon(zt, points, c, 0, CLineDash());
          else
            renderer->drawHiddenPolygon(zt, points, c.inverse(), 0, CLineDash());
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

          if (points.size() < 4) continue;

          if (pattern & 1) renderer->drawLine(points[0], points[1], c, 1);
          if (pattern & 2) renderer->drawLine(points[0], points[3], c, 1);
          if (pattern & 4) renderer->drawLine(points[0], points[2], c, 1);

          if (ix == nx - 1 || iy == ny - 1) {
            if (pattern & 1) renderer->drawLine(points[2], points[3], c, 1);
            if (pattern & 2) renderer->drawLine(points[2], points[1], c, 1);
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

    for (auto polys : zpolygons_) {
      double zt = polys.first;

      for (auto poly : polys.second) {
        const CGnuPlotSurface::PointsIndColor &pic = poly.second;
        const CGnuPlotSurface::PointsInd      &pi  = pic.first;

        CGnuPlotPolygonObject *polygon = 0;

        if (! renderer->isPseudo())
          polygon = plot_->polygonObjects()[pi.second];

        double z = poly.first;

        CRGBA rgba;

        if      (isCalcColor()) {
          rgba = pic.second;
        }
        else if (isDataColor()) {
          rgba = pic.second;
        }
        else {
          double r = (z - zmin_.getValue())/(zmax_.getValue() - zmin_.getValue());

          CColor c = plot_->group()->palette()->getColor(r);

          rgba = c.rgba();
        }

        if (! renderer->isPseudo()) {
          polygon->setPoints(pi.first);
          polygon->setZ(zt);

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
          renderer->fillPolygon(pi.first, rgba);
        }
      }
    }

    if (! renderer->isPseudo()) {
      for (const auto &polygon : plot_->polygonObjects())
        polygon->draw(renderer);
    }
  }
}
