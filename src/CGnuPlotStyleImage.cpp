#include <CGnuPlotStyleImage.h>
#include <CGnuPlotPlot.h>
#include <CGnuPlotGroup.h>
#include <CGnuPlotRenderer.h>
#include <CGnuPlotImageObject.h>
#include <CGnuPlotRectObject.h>
#include <CGnuPlotPolygonObject.h>
#include <CGnuPlotUtil.h>
#include <CMathGeom2D.h>
#include <CHSV.h>

namespace {
  CRGBA applyCbRange(const CRGBA &rgba, double min, double max) {
#if RGB_UTIL
    CHSV hsv = CRGBUtil::RGBtoHSV(rgba.getRGB());
#else
    CHSV hsv = hsv.fromRGB(rgba);
#endif

    double v = CGnuPlotUtil::clamp(255*hsv.getValue()/(max - min) + min, 0.0, 1.0);

    hsv.setValue(v);

#if RGB_UTIL
    return CRGBUtil::HSVtoRGB(hsv).clamp();
#else
    return hsv.toRGB().clamp();
#endif
  }
}

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

  //double pw = 1, ph = 1;

  //if (! renderer->isPseudo()) {
  //  pw = renderer->pixelWidthToWindowWidth  (1);
  //  ph = renderer->pixelHeightToWindowHeight(1);
  //}

  //int npx = (pw < 1 && pw > 0 ? int(1.0/pw + 0.5) : 1);
  //int npy = (ph < 1 && ph > 0 ? int(1.0/ph + 0.5) : 1);

  const CGnuPlotPlot::ImageData &imageData  = plot->imageData();
  const CGnuPlotImageStyle      &imageStyle = plot->imageStyle();

  CGnuPlotGroup *group = plot->group();

  const CGnuPlotAxisData &cbaxis = group->colorBox()->axis();

  COptReal cbmin = cbaxis.min();
  COptReal cbmax = cbaxis.max();

  //---

  // image size
  int iw = imageStyle.width ().getValue(1);
//int ih = imageStyle.height().getValue(1);

  int nd = imageData.size();
  int ny = nd/iw;
  int nx = nd/ny;

  //---

  // scale factor
  double idx = imageStyle.dx().getValue(1);
  double idy = imageStyle.dy().getValue(1);

  double ww = nx*idx;
  double hh = ny*idy;

  //---

  // center point
  double cx = 0, cy = 0;

  if (imageStyle.center().isValid()) {
    cx = imageStyle.center().getValue().x;
    cy = imageStyle.center().getValue().y;
  }
  else {
    cx = ww/2.0;
    cy = hh/2.0;
  }

  CPoint2D c(cx, cy);

  //---

  // origin point
  CPoint2D o;

  if (imageStyle.origin().isValid()) {
    o = imageStyle.origin().getValue();

    cx = o.x + ww/2;
    cy = o.y + hh/2;
  }
  else
    o = c;

  //---

//bool rotate = imageStyle.angle().isValid();

  double a  = -plot->imageAngle();
//double ra = CAngle::Deg2Rad(a);

  //if (imageStyle.isFlipY()) ra = -ra;

  //CPoint2D po(cx - ww/2, cy + (imageStyle.isFlipY() ? hh/2 : -hh/2));
  CPoint2D po(cx - ww/2, cy + hh/2);

  //---

  double gmax = 0;

  if (style_ == CGnuPlotTypes::PlotStyle::IMAGE) {
    int i = 0;

    for (int iy = 0; iy < ny; ++iy) {
      for (int ix = 0; ix < nx; ++ix, ++i) {
        CRGBA rgba = imageData[i];

        if (imageStyle.usingCols().numCols() > 0) {
          if (imageStyle.usingCols().numCols() > 1) {
            double r = decodeImageUsingColor(plot, 0, rgba);
            double g = decodeImageUsingColor(plot, 1, rgba);
            double b = decodeImageUsingColor(plot, 2, rgba);

            gmax = std::max(gmax, std::max(std::max(r, g), b));
          }
          else {
            double g = decodeImageUsingColor(plot, 0, rgba);

            gmax = std::max(gmax, g);
          }
        }
      }
    }
  }

  //---

  CBBox2D bbox(po.x, po.y - hh, po.x + ww, po.y);

  if (! renderer->isPseudo()) {
    plot->updateImageCacheSize(1);

    CGnuPlotImageObject *image = plot->imageObjects()[0];

    bool update = ! image->isUsed();

    if (! update)
      update = (image->size() != CISize2D(ww, hh));

    //---

    if (update) {
      //renderer->setAntiAlias(false);

      image->setBBox (bbox);
      image->setSize (CISize2D(ww, hh));
      image->setAngle(a);
      image->setFlipY(imageStyle.isFlipY());

      image->setTipText(imageStyle.filename());

      int i = 0;

      for (int iy = 0; iy < hh; ++iy) {
        //double y = po.y + (imageStyle.isFlipY() ? -1 : 1)*(iy*idy + 0.5);
        //double y = po.y - iy*idy - 0.5;
        //double y = (ny - 1 - iy)*idy + 0.5;

        int iy1 = (hh - 1 - iy)/idy;

        for (int ix = 0; ix < ww; ++ix, ++i) {
          //double x = po.x + ix*idx + 0.5;
          //double x = ix*idx + 0.5;

          int ix1 = ix/idx;

          int i = iy1*nx + ix1;

          CRGBA rgba = imageData[i];

          if (imageStyle.usingCols().numCols() > 0) {
            if (imageStyle.usingCols().numCols() > 1) {
              double r = decodeImageUsingColor(plot, 0, rgba);
              double g = decodeImageUsingColor(plot, 1, rgba);
              double b = decodeImageUsingColor(plot, 2, rgba);

              rgba = CRGBA(r, g, b);
            }
            else {
              double g = decodeImageUsingColor(plot, 0, rgba);

              if (style_ == CGnuPlotTypes::PlotStyle::IMAGE)
                rgba = group->palette()->getColor(g/gmax).rgba();
              else
                rgba = CRGBA(g, g, g);
            }
          }

          if (cbmin.isValid() && cbmax.isValid())
            rgba = applyCbRange(rgba, cbmin.getValue(), cbmax.getValue());

          CPoint2D p(ix, iy);

          image->drawPoint(p, rgba);

#if 0
          if (is3D) {
            //CPoint2D p1 = (rotate ? CMathGeom2D::RotatePoint(p, ra, o) : p);

            //if (renderer->clip().isSet() && ! renderer->clip().inside(p1))
            //  continue;

            for (int py = 0; py < npy; ++py) {
              for (int px = 0; px < npx; ++px) {
                CPoint2D p1(p.x + px*pw, p.y + py*ph);

                CPoint2D p2 = CMathGeom2D::RotatePoint(p1, ra, o);

                CPoint3D p3(p2.x, p2.y, 0.0); // zmin ?

                //renderer->drawPoint(p3, rgba);
                image->drawPoint(renderer->transform(p3), rgba);
                //renderer->drawPoint(p2, rgba);
                //image->drawPoint(p2, rgba);
              }
            }
          }
          else {
            //if (renderer->clip().isSet() && ! renderer->clip().inside(p))
            //  continue;

            CBBox2D bbox(p.x - idx/2.0, p.y - idy/2.0, p.x + idx/2.0, p.y + idy/2.0);

            //renderer->fillRect(bbox, rgba);
            if (rotate)
              image->fillRotatedRect(bbox, o, a, rgba);
            else
              image->fillRect(bbox, rgba);
          }
#endif
        }
      }

      //renderer->setAntiAlias(true);

      image->setUsed(true);
    }

    image->draw(renderer);
  }
  else {
    renderer->fillRect(bbox, CRGBA(0,0,0));
  }
}

void
CGnuPlotStyleImageBase::
drawImage2D(CGnuPlotPlot *plot, CGnuPlotRenderer *renderer)
{
  //double pw = 1, ph = 1;

  //if (! renderer->isPseudo()) {
  //  pw = renderer->pixelWidthToWindowWidth  (1);
  //  ph = renderer->pixelHeightToWindowHeight(1);
  //}

  //int npx = (pw < 1 && pw > 0 ? int(1.0/pw + 0.5) : 1);
  //int npy = (ph < 1 && ph > 0 ? int(1.0/ph + 0.5) : 1);

//const CGnuPlotPlot::ImageData &imageData  = plot->imageData();
  const CGnuPlotImageStyle      &imageStyle = plot->imageStyle();

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

  CPoint2D po = imageStyle.origin().getValue(CPoint2D(iw/2.0, ih/2.0));

  double xo = po.x;
  double yo = po.y;

//bool   rotate = imageStyle.angle().isValid();
//double a      = -plot->imageAngle();

//double ra = CAngle::Deg2Rad(a);

//if (imageStyle.isFlipY()) ra = -ra;

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
    int ny = plot->getPoints2D().size();

    if (! renderer->isPseudo()) {
      // count number of rects
      int n = 0;

      for (const auto &p : plot->getPoints2D()) {
        std::vector<double> reals;

        (void) p.getReals(reals);

        n += reals.size();
      }

      // udpate cache size
      plot->updateRectCacheSize(n);
    }

    //---

    int i = 0;
    int y = 0;

    for (const auto &p : plot->getPoints2D()) {
      double y1 = (imageStyle.isFlipY() ? ny - 1 - y : y);

      std::vector<double> reals;

      (void) p.getReals(reals);

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

  #if 0
        if (rotate) {
          CPoint2D p1 = CMathGeom2D::RotatePoint(p, ra, o);

          if (! renderer->isPseudo() && (renderer->clip().isSet() && ! renderer->clip().inside(p1)))
            continue;

          for (int py = 0; py < npy; ++py) {
            for (int px = 0; px < npx; ++px) {
              CPoint2D p1(p.x + px*pw, p.y + py*ph);

              CPoint2D p2 = CMathGeom2D::RotatePoint(p1, ra, o);

              renderer->drawPoint(p2, rgba);
            }
          }
        }
  #endif

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

      ++y;
    }
  }
  else {
    int i = 0;

    typedef std::vector<CPoint3D> Points;
    typedef std::vector<Points>   PointsArray;

    Points      points;
    PointsArray pointsArray;

    for (const auto &p : plot->getPoints2D()) {
      if (p.isDiscontinuity()) {
        if (! points.empty())
          pointsArray.push_back(points);

        points.clear();
      }

      CPoint3D p1;

      if (p.getPoint(p1))
        points.push_back(p1);
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

        CRGBA rgba;

        if (renderer->isPseudo())
          renderer->setCBValue(point.z);
        else
          rgba = cb->valueToColor(point.z).rgba();

        CPoint2D p(x1 + dx, y1 + dy);

        CBBox2D bbox(p.x - 0.5, p.y - 0.5, p.x + 0.5, p.y + 0.5);

        if (! renderer->isPseudo()) {
          CGnuPlotRectObject *rect = plot->rectObjects()[i];

          rect->setBBox(bbox);

          if (! rect->testAndSetUsed()) {
            rect->fill()->setType (CGnuPlotTypes::FillType::SOLID);
            rect->fill()->setColor(rgba);

            rect->stroke()->setEnabled(false);

            rect->setTipText(CStrUtil::strprintf("%g", point.z));
          }

          ++i;
        }
        else
          renderer->fillRect(bbox, rgba);
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
  //double pw = 1, ph = 1;

  //if (! renderer->isPseudo()) {
  //  pw = renderer->pixelWidthToWindowWidth  (1);
  //  ph = renderer->pixelHeightToWindowHeight(1);
  //}

  //int npx = (pw < 1 && pw > 0 ? int(1.0/pw + 0.5) : 1);
  //int npy = (ph < 1 && ph > 0 ? int(1.0/ph + 0.5) : 1);

//const CGnuPlotPlot::ImageData &imageData  = plot->imageData();
  const CGnuPlotImageStyle      &imageStyle = plot->imageStyle();

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

  CPoint2D po = imageStyle.origin().getValue(CPoint2D(iw/2.0, ih/2.0));

  double xo = po.x;
  double yo = po.y;

//bool   rotate = imageStyle.angle().isValid();
//double a      = -plot->imageAngle();

//double ra = CAngle::Deg2Rad(a);

//if (imageStyle.isFlipY()) ra = -ra;

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
  const CGnuPlotImageStyle &imageStyle = plot->imageStyle();

  int nc = imageStyle.usingCols().numCols();

  if (col < nc) {
    if (imageStyle.usingCols().getCol(col).isInt())
      col = imageStyle.usingCols().getCol(col).ival();
    else {
      bool lookup = true;

      std::string expr = imageStyle.usingCols().getCol(col).str();

      // replace $N variables
      // TODO: easier to define $1 variables
      auto pos = expr.find('$');

      while (pos != std::string::npos) {
        lookup = false;

        int pos1 = ++pos;

        while (isdigit(expr[pos1]))
          ++pos1;

        std::string numStr = expr.substr(pos, pos1 - pos);

        int icol = 0;

        (void) CStrUtil::toInteger(numStr, &icol);

        double r = indexImageColor(icol, rgba);

        expr = expr.substr(0, pos - 1) + CStrUtil::toString(r) + expr.substr(pos1);

        pos = expr.find('$');
      }

      if (expr.size() > 2 && expr[0] == '(' && expr[expr.size() - 1] == ')') {
        lookup = false;

        expr = expr.substr(1, expr.size() - 2);
      }

      CExprValuePtr value;

      if (! CExprInst->evaluateExpression(expr, value))
        value = CExprValuePtr();

      if (! lookup) {
        double r;

        if (value.isValid() && value->getRealValue(r))
          return r;
      }
      else {
        CExprValuePtr value;

        if (! CExprInst->evaluateExpression(expr, value))
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
