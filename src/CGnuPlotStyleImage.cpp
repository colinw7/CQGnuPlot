#include <CGnuPlotStyleImage.h>
#include <CGnuPlotPlot.h>
#include <CGnuPlotGroup.h>
#include <CGnuPlotRenderer.h>
#include <CGnuPlotImageObject.h>
#include <CGnuPlotRectObject.h>
#include <CGnuPlotPolygonObject.h>
#include <CGnuPlotUtil.h>
#include <CMathGeom2D.h>
#include <CRotBBox2D.h>
#include <CHSV.h>

//------

CGnuPlotStyleImageBase::
CGnuPlotStyleImageBase(CGnuPlotTypes::PlotStyle style) :
 CGnuPlotStyleBase(style)
{
}

CGnuPlotStyleImage::
CGnuPlotStyleImage() :
 CGnuPlotStyleImageBase(CGnuPlot::PlotStyle::IMAGE)
{
}

CGnuPlotStyleRGBImage::
CGnuPlotStyleRGBImage() :
 CGnuPlotStyleImageBase(CGnuPlot::PlotStyle::RGBIMAGE)
{
}

CGnuPlotStyleRGBAlpha::
CGnuPlotStyleRGBAlpha() :
 CGnuPlotStyleImageBase(CGnuPlot::PlotStyle::RGBALPHA)
{
}

//------

void
CGnuPlotStyleImageBase::
draw2D(CGnuPlotPlot *plot, CGnuPlotRenderer *renderer)
{
  if (plot->isBinary())
    drawBinaryImage(plot, renderer);
  else
    drawImage2D(plot, renderer);
}

void
CGnuPlotStyleImageBase::
draw3D(CGnuPlotPlot *plot, CGnuPlotRenderer *renderer)
{
  if (plot->isBinary())
    drawBinaryImage(plot, renderer);
  else
    drawImage3D(plot, renderer);
}

//------

void
CGnuPlotStyleImageBase::
drawBinaryImage(CGnuPlotPlot *plot, CGnuPlotRenderer *renderer)
{
  //bool is3D = plot->is3D();

  const CGnuPlotImageStyle      &imageStyle  = plot->imageStyle();
  const CGnuPlotPlot::YXColor   &imageColors = plot->imageColors();
  const CGnuPlotPlot::RowValues &imageValues = plot->imageValues();

  CGnuPlotGroup *group = plot->group();

  //---

  // image size
  int xo = 0, yo = 0;
  int nx = 0, ny = 0;

  if (style_ == CGnuPlotTypes::PlotStyle::IMAGE) {
    ny = imageValues.size();
    nx = (ny > 0 ? (*imageValues.begin()).second.size() : 0);
  }
  else {
    COptInt xmin, ymin, xmax, ymax;

    for (const auto &yxc : imageColors) {
      int py = yxc.first;

      ymin.updateMin(py);
      ymax.updateMax(py);

      for (const auto &xc : yxc.second) {
        int px = xc.first;

        xmin.updateMin(px);
        xmax.updateMax(px);
      }
    }

    xo = xmin.getValue(0);
    yo = ymin.getValue(0);

    nx = xmax.getValue(1) - xmin.getValue(0) + 1;
    ny = ymax.getValue(1) - ymin.getValue(0) + 1;
  }

  if (nx == 0 || ny == 0)
    return;

  //---

  // scale factor
  double idx = imageStyle.dx().getValue(1);
  double idy = imageStyle.dy().getValue(1);

  double ww = nx*idx;
  double hh = ny*idy;

  //---

  // angle
  double a = plot->imageAngle();

  //---

  CBBox2D bbox;

  // get center and origin (rotation) point
  CPoint3D center, origin;

  if      (imageStyle.center().isValid()) {
    center = imageStyle.center().getValue();

    if (imageStyle.origin().isValid())
      origin = imageStyle.origin().getValue();
    else
      origin = center;
  }
  else if (imageStyle.origin().isValid()) {
    //double rw = plot->imageRange().getWidth ();
    //double rh = plot->imageRange().getHeight();

    origin = imageStyle.origin().getValue();
    center = CPoint3D(origin.x + ww/2.0, origin.y + hh/2.0, origin.z);
  }
  else {
    CPoint2D c = plot->imageRange().getCenter();

    int iw = imageStyle.width ().getValue(1);
    int ih = imageStyle.height().getValue(1);

    center = CPoint3D(c.x, c.y, 0);
    origin = CPoint3D(iw/2.0, ih/2.0, 0);
  }

  CPoint2D p1(center.x - ww/2.0, center.y - hh/2.0);
  CPoint2D p2(center.x + ww/2.0, center.y + hh/2.0);

#if 0
  if (imageStyle.isFlipX()) {
    p1.x = origin.x - (p1.x - origin.x);
    p2.x = origin.x - (p2.x - origin.x);
  }

  if (imageStyle.isFlipY()) {
    p1.y = origin.y - (p1.y - origin.y);
    p2.y = origin.y - (p2.y - origin.y);
  }
#endif

  CRotBBox2D rbbox(CBBox2D(p1, p2), CAngle::Deg2Rad(a), CPoint2D(origin.x, origin.y));

  bbox = rbbox.boundingBox();

  //---

  // image style uses rgb palette
  COptReal pmin, pmax;

  if (style_ == CGnuPlotTypes::PlotStyle::IMAGE) {
    for (const auto &row : imageValues) {
      for (const auto &r : row.second) {
        pmin.updateMin(r);
        pmax.updateMax(r);
      }
    }
  }

  //---

  if (! renderer->isPseudo()) {
    plot->updateImageCacheSize(1);

    CGnuPlotImageObject *image = plot->imageObjects()[0];

    // fixed size ?
    image->setSize(CISize2D(nx, ny));

    if (! image->testAndSetUsed()) {
      image->setOrigin(origin);
      image->setCenter(center);
      image->setDelta (CSize2D(idx, idy));
      image->setAngle (a);
      image->setFlipY (imageStyle.isFlipY());

      image->setTipText(imageStyle.filename());
    }

    if (! plot->is3D()) {
      if (! image->image1().isValid()) {
        int iy = 0;

        if (style_ == CGnuPlotTypes::PlotStyle::IMAGE) {
          for (const auto &row : imageValues) {
            int iy1 = ny - 1 - iy;

            int ix = 0;

            for (const auto &r : row.second) {
              int ix1 = ix;

              double z = CGnuPlotUtil::map(r, pmin.getValue(0), pmax.getValue(1), 0, 1);

              CRGBA c = group->palette()->getColor(z).rgba();

              CPoint2D p(ix1, iy1);

              image->drawPoint(p, c);

              ++ix;
            }

            ++iy;
          }
        }
        else {
          int iy1 = 0;

          for (const auto &yxc : imageColors) {
            int iy2 = yxc.first - yo;

            int ix1 = 0;

            for (const auto &xc : yxc.second) {
              int ix2 = xc.first - xo;

              for (int iy = iy1; iy <= iy2; ++iy) {
                for (int ix = ix1; ix <= ix2; ++ix) {
                  int iy1 = ny - 1 - iy;

                  CPoint2D p(ix, iy1);

                  image->drawPoint(p, xc.second);
                }
              }

              ix1 = ix2;
            }

            iy1 = iy2;
          }
        }
      }

      image->draw2D(renderer);
    }
    else {
      CGnuPlotImageObject::Colors colors;

      colors.reserve(nx*ny);

      if (style_ == CGnuPlotTypes::PlotStyle::IMAGE) {
        for (const auto &xcolor : imageValues) {
          for (const auto &r : xcolor.second) {
            double z = CGnuPlotUtil::map(r, pmin.getValue(0), pmax.getValue(1), 0, 1);

            CRGBA c = group->palette()->getColor(z).rgba();

            colors.push_back(c);
          }
        }
      }
      else {
        for (const auto &yxc : imageColors) {
          for (const auto &xc : yxc.second)
            colors.push_back(xc.second);
        }
      }

      image->setColors(colors);

      image->draw3D(renderer);
#if 0
      CPoint2D o(origin.x, origin.y);

      double ra = CAngle::Deg2Rad(a);

      double w = nx*idx;
      double h = ny*idy;

      CPoint2D p1(center.x - w/2.0, center.y - h/2.0);
      CPoint2D p2(center.x + w/2.0, center.y + h/2.0);

      double z = 0;

      int i = 0;

      for (int iy = 0; iy < ny; ++iy) {
        int iy1 = ny - 1 - iy;

        for (int ix = 0; ix < nx; ++ix, ++i) {
          int ix1 = ix;

          int i = iy1*nx + ix1;

          CRGBA rgba;

          if (style_ == CGnuPlotTypes::PlotStyle::IMAGE) {
            double z = CGnuPlotUtil::map(imageValues[i], pmin.getValue(0), pmax.getValue(1), 0, 1);

            rgba = group->palette()->getColor(z).rgba();
          }
          else
            rgba = imageColors[i];

          double x1 = CGnuPlotUtil::map(ix, 0, nx - 1, p1.x, p2.x);
          double y1 = CGnuPlotUtil::map(iy, 0, ny - 1, p1.y, p2.y);
          double x2 = x1 + idx;
          double y2 = y1 + idy;

          CPoint2D pr1 = CMathGeom2D::RotatePoint(CPoint2D(x1, y1), ra, o);
          CPoint2D pr2 = CMathGeom2D::RotatePoint(CPoint2D(x2, y1), ra, o);
          CPoint2D pr3 = CMathGeom2D::RotatePoint(CPoint2D(x2, y2), ra, o);
          CPoint2D pr4 = CMathGeom2D::RotatePoint(CPoint2D(x1, y2), ra, o);

          CGnuPlotRenderer::Points2D poly({
            renderer->transform(CPoint3D(pr1.x, pr1.y, z)),
            renderer->transform(CPoint3D(pr2.x, pr2.y, z)),
            renderer->transform(CPoint3D(pr3.x, pr3.y, z)),
            renderer->transform(CPoint3D(pr4.x, pr4.y, z))
          });

          renderer->fillPolygon(poly, rgba);
        }
      }
#endif
    }
  }
  else {
    renderer->fillRect(bbox, CRGBA(0,0,0));
  }
}

void
CGnuPlotStyleImageBase::
drawImage2D(CGnuPlotPlot *plot, CGnuPlotRenderer *renderer)
{
  const CGnuPlotImageStyle &imageStyle = plot->imageStyle();

  CGnuPlotGroup *group = plot->group();

  //---

  // image size
  int iw = imageStyle.width ().getValue(1);
  int ih = imageStyle.height().getValue(1);

  //---

  double dx = 0, dy = 0;

  if (imageStyle.center().isValid()) {
    dx = imageStyle.center().getValue().x - iw/2.0;
    dy = imageStyle.center().getValue().y - ih/2.0;
  }

  CPoint3D po = imageStyle.origin().getValue(CPoint3D(iw/2.0, ih/2.0, 0));

  double xo = po.x;
  double yo = po.y;

  CPoint2D o(xo + dx, yo + dy);

  const CGnuPlotColorBoxP &cb = group->colorBox();

  //---

  int numSets = 1;

  for (const auto &p : plot->getPoints2D()) {
    if (p.isDiscontinuity())
      ++numSets;
  }

  //---

  if (numSets == 1) {
#if 0
    int ny = plot->getPoints2D().size();
#endif

    if (! renderer->isPseudo()) {
#if 0
      // count number of rects
      int n = 0;

      for (const auto &p : plot->getPoints2D()) {
        std::vector<double> reals;

        (void) p.getReals(reals);

        n += reals.size();
      }
#else
      int n = plot->numPoints2D();
#endif

      // udpate cache size
      plot->updateRectCacheSize(n);
    }

    //---

#if 0
    int y = 0;
#endif

    CBBox2D bbox;

    for (const auto &p : plot->getPoints2D()) {
      std::vector<double> reals;

      (void) p.getReals(reals);

      double x = (reals.size() > 0 ? reals[0] : 0);
      double y = (reals.size() > 1 ? reals[1] : 0);

      bbox.add(CPoint2D(x, y));
    }

    int nx = std::max(int(sqrt(plot->numPoints2D())), 1);
    int ny = plot->numPoints2D()/nx;

    double pw = (nx > 1 ? bbox.getWidth ()/(nx - 1) : 1);
    double ph = (ny > 1 ? bbox.getHeight()/(ny - 1) : 1);

    int i = 0;

    for (const auto &p : plot->getPoints2D()) {
      std::vector<double> reals;

      (void) p.getReals(reals);

#if 0
      double y1 = (imageStyle.isFlipY() ? ny - 1 - y : y);

      int x = 0;

      for (const auto &v : reals) {
        double x1 = x;

        CRGBA rgba;

        if (renderer->isPseudo())
          renderer->setCBValue(v);
        else {
          rgba = cb->valueToColor(v).rgba();
          //rgba = CGnuPlotStyleInst->indexColor(int(v));
        }

        CPoint2D p(x1 + dx, y1 + dy);

        //if (! renderer->isPseudo() && (renderer->clip().isSet() && ! renderer->clip().inside(p)))
        //  continue;

        CBBox2D bbox(p.x - 0.5, p.y - 0.5, p.x + 0.5, p.y + 0.5);

        if (! renderer->isPseudo()) {
          CGnuPlotRectObject *rect = plot->rectObjects()[i];

          rect->setBBox(bbox);

          if (! rect->testAndSetUsed()) {
            rect->fill()->setType (CGnuPlotTypes::FillType::SOLID);
            rect->fill()->setColor(rgba);

            rect->stroke()->setEnabled(false);

            rect->setTipText(CStrUtil::strprintf("%g", v));
          }

          ++i;
        }
        else
          renderer->fillRect(bbox, rgba);

        ++x;
      }
#else
      double x = (reals.size() > 0 ? reals[0] : 0);
      double y = (reals.size() > 1 ? reals[1] : 0);
      double z = 0;

      CRGBA c;

      if      (style_ == CGnuPlot::PlotStyle::IMAGE) {
        z = (reals.size() > 2 ? reals[2] : 0);

        c = cb->valueToColor(z).rgba();
      }
      else if (style_ == CGnuPlot::PlotStyle::RGBIMAGE) {
        double r = (reals.size() > 2 ? reals[2] : 0);
        double g = (reals.size() > 3 ? reals[3] : 0);
        double b = (reals.size() > 4 ? reals[4] : 0);

        c = CRGBA(r, g, b);
      }
      else if (style_ == CGnuPlot::PlotStyle::RGBALPHA) {
        double r = (reals.size() > 2 ? reals[2] : 0);
        double g = (reals.size() > 3 ? reals[3] : 0);
        double b = (reals.size() > 4 ? reals[4] : 0);
        double a = (reals.size() > 5 ? reals[5] : 1);

        c = CRGBA(r, g, b, a);
      }

      CBBox2D bbox(x - pw/2, y - ph/2, x + pw/2, y + ph/2);

      if (! renderer->isPseudo()) {
        CGnuPlotRectObject *rect = plot->rectObjects()[i];

        rect->setBBox(bbox);

        if (! rect->testAndSetUsed()) {
          rect->fill()->setType (CGnuPlotTypes::FillType::SOLID);
          rect->fill()->setColor(c);

          rect->stroke()->setEnabled(false);

          rect->setTipText(CStrUtil::strprintf("%g", z));
        }

        ++i;
      }
      else
        renderer->fillRect(bbox, c);
#endif

      ++y;
    }
  }
  else {
    int i = 0;

    struct XYColor {
      double x;
      double y;
      CRGBA  c;
    };

    typedef std::vector<XYColor> Points;
    typedef std::vector<Points>  PointsArray;

    Points      points;
    PointsArray pointsArray;

    for (const auto &p : plot->getPoints2D()) {
      if (p.isDiscontinuity()) {
        if (! points.empty())
          pointsArray.push_back(points);

        points.clear();
      }

      std::vector<double> reals;

      if (! p.getReals(reals))
        continue;

      if (reals.size() < 3)
        continue;

      XYColor c;

      if (reals.size() < 5) {
        c.x = reals[0];
        c.y = reals[1];

        double z = reals[2];

        if (renderer->isPseudo())
          renderer->setCBValue(z);
        else
          c.c = cb->valueToColor(z).rgba();
      }
      else {
        c.x = reals[0];
        c.y = reals[1];
        c.c = CRGBA(reals[2]/255.0, reals[3]/255.0, reals[4]/255.0);
      }

      points.push_back(c);
    }

    if (! points.empty())
      pointsArray.push_back(points);

    //---

    if (! renderer->isPseudo()) {
      // count number of rects
      int n = 0;

      for (const auto &points : pointsArray)
        n += points.size();

      // udpate cache size
      plot->updateRectCacheSize(n);
    }

    //---

    for (const auto &points : pointsArray) {
      for (const auto &point : points) {
        double x1 = point.x;
        double y1 = point.y;

        CPoint2D p(x1 + dx, y1 + dy);

        CBBox2D bbox(p.x - 0.5, p.y - 0.5, p.x + 0.5, p.y + 0.5);

        if (! renderer->isPseudo()) {
          CGnuPlotRectObject *rect = plot->rectObjects()[i];

          rect->setBBox(bbox);

          if (! rect->testAndSetUsed()) {
            rect->fill()->setType (CGnuPlotTypes::FillType::SOLID);
            rect->fill()->setColor(point.c);

            rect->stroke()->setEnabled(false);

            rect->setTipText(point.c.toString());
          }

          ++i;
        }
        else
          renderer->fillRect(bbox, point.c);
      }
    }
  }

  if (! renderer->isPseudo()) {
    for (const auto &rect : plot->rectObjects())
      rect->draw(renderer);
  }
}

void
CGnuPlotStyleImageBase::
drawImage3D(CGnuPlotPlot *plot, CGnuPlotRenderer *renderer)
{
  const CGnuPlotImageStyle &imageStyle = plot->imageStyle();

  CGnuPlotGroup *group = plot->group();

  CGnuPlotCameraP camera = group->camera();

  //---

  // image size
  int iw = imageStyle.width ().getValue(1);
  int ih = imageStyle.height().getValue(1);

  //---

  double dx = 0, dy = 0;

  if (imageStyle.center().isValid()) {
    dx = imageStyle.center().getValue().x - iw/2.0;
    dy = imageStyle.center().getValue().y - ih/2.0;
  }

  CPoint3D po = imageStyle.origin().getValue(CPoint3D(iw/2.0, ih/2.0, 0));

  double xo = po.x;
  double yo = po.y;

  CPoint2D o(xo + dx, yo + dy);

  //---

  std::pair<int,int> np = plot->numPoints3D();

  if (! renderer->isPseudo()) {
    // count number of rects
    int n = np.first*np.second;

    // udpate cache size
    plot->updatePolygonCacheSize(n);
  }

  //---

  const CGnuPlotColorBoxP &cb = group->colorBox();

  int i = 0;

  for (int iy = 0; iy < np.second; ++iy) {
    double y1 = (imageStyle.isFlipY() ? np.second - 1 - iy : iy);

    for (int ix = 0; ix < np.first; ++ix) {
      double x1 = ix;

      // get surface points
      const CGnuPlotPoint &ppoint = plot->getPoint3D(ix, iy);

      CPoint3D point;

      ppoint.getPoint(point);

      CRGBA c;

      if (renderer->isPseudo())
        renderer->setCBValue(point.z);
      else
        c = cb->valueToColor(point.z).rgba();

      CPoint3D p(x1 + dx, y1 + dy, 0);

      CPoint3D p1 = camera->transform(CPoint3D(p.x - 0.5, p.y - 0.5, 0));
      CPoint3D p2 = camera->transform(CPoint3D(p.x + 0.5, p.y - 0.5, 0));
      CPoint3D p3 = camera->transform(CPoint3D(p.x + 0.5, p.y + 0.5, 0));
      CPoint3D p4 = camera->transform(CPoint3D(p.x - 0.5, p.y + 0.5, 0));

      CPoint3D pt1 = camera->transform(p1);
      CPoint3D pt2 = camera->transform(p2);
      CPoint3D pt3 = camera->transform(p3);
      CPoint3D pt4 = camera->transform(p4);

      //---

      // create polygon
      std::vector<CPoint2D> poly;

      poly.push_back(CPoint2D(pt1.x, pt1.y));
      poly.push_back(CPoint2D(pt2.x, pt2.y));
      poly.push_back(CPoint2D(pt3.x, pt3.y));
      poly.push_back(CPoint2D(pt4.x, pt4.y));

      if (! renderer->isPseudo()) {
        CGnuPlotPolygonObject *polygon = plot->polygonObjects()[i];

        polygon->setPoints(poly);

        if (! polygon->testAndSetUsed()) {
          polygon->fill()->setType (CGnuPlotTypes::FillType::SOLID);
          polygon->fill()->setColor(c);

          polygon->stroke()->setEnabled(false);

          polygon->setTipText(CStrUtil::strprintf("%g", point.z));
        }
      }
      else
        renderer->fillPolygon(poly, c);

      ++i;
    }
  }

  if (! renderer->isPseudo()) {
    for (const auto &polygon : plot->polygonObjects())
      polygon->draw(renderer);
  }
}

double
CGnuPlotStyleImageBase::
decodeImageUsingColor(CGnuPlotPlot *plot, int col, const CRGBA &rgba) const
{
  CExpr *expr = plot->app()->expr();

  const CGnuPlotImageStyle &imageStyle = plot->imageStyle();

  int nc = imageStyle.usingCols().numCols();

  if (col < nc) {
    if (imageStyle.usingCols().getCol(col).isInt())
      col = imageStyle.usingCols().getCol(col).ival();
    else {
      bool lookup = true;

      std::string exprStr = imageStyle.usingCols().getCol(col).str();

      // replace $N variables
      // TODO: easier to define $1 variables
      auto pos = exprStr.find('$');

      while (pos != std::string::npos) {
        lookup = false;

        int pos1 = ++pos;

        while (isdigit(exprStr[pos1]))
          ++pos1;

        std::string numStr = exprStr.substr(pos, pos1 - pos);

        int icol = 0;

        (void) CStrUtil::toInteger(numStr, &icol);

        double r = indexImageColor(icol, rgba);

        exprStr = exprStr.substr(0, pos - 1) + CGnuPlotUtil::toString(r) + exprStr.substr(pos1);

        pos = exprStr.find('$');
      }

      if (exprStr.size() > 2 && exprStr[0] == '(' && exprStr[exprStr.size() - 1] == ')') {
        lookup = false;

        exprStr = exprStr.substr(1, exprStr.size() - 2);
      }

      CExprValuePtr value;

      if (! expr->evaluateExpression(exprStr, value))
        value = CExprValuePtr();

      if (! lookup) {
        double r;

        if (value.isValid() && value->getRealValue(r))
          return r;
      }
      else {
        CExprValuePtr value;

        if (! expr->evaluateExpression(exprStr, value))
          value = CExprValuePtr();

        long l;

        if (value.isValid() && value->getIntegerValue(l))
          return indexImageColor(l, rgba);
      }

      return 1;
    }
  }

  return indexImageColor(col, rgba);
}

double
CGnuPlotStyleImageBase::
indexImageColor(int i, const CRGBA &rgba) const
{
  switch (i) {
    case 1 : return rgba.getRed  ();
    case 2 : return rgba.getGreen();
    case 3 : return rgba.getBlue ();
    default: return 1;
  }
}
