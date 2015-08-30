#include <CGnuPlotStyleImage.h>
#include <CGnuPlotPlot.h>
#include <CGnuPlotGroup.h>
#include <CGnuPlotRenderer.h>
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
    drawImage(plot, renderer);
}

void
CGnuPlotStyleImageBase::
draw3D(CGnuPlotPlot *plot, CGnuPlotRenderer *renderer)
{
  if (plot->isBinary())
    drawBinaryImage(plot, renderer);
  else
    drawImage(plot, renderer);
}

//------

void
CGnuPlotStyleImageBase::
drawBinaryImage(CGnuPlotPlot *plot, CGnuPlotRenderer *renderer)
{
  bool is3D = plot->is3D();

  double pw = 1, ph = 1;

  if (! renderer->isPseudo()) {
    pw = renderer->pixelWidthToWindowWidth  (1);
    ph = renderer->pixelHeightToWindowHeight(1);
  }

  int npx = (pw < 1 && pw > 0 ? int(1.0/pw + 0.5) : 1);
  int npy = (ph < 1 && ph > 0 ? int(1.0/ph + 0.5) : 1);

  const CGnuPlotPlot::ImageData &imageData  = plot->imageData();
  const CGnuPlotImageStyle      &imageStyle = plot->imageStyle();

  CGnuPlotGroup *group = plot->group();

  const CGnuPlotAxisData &cbaxis = group->colorBox()->axis();

  COptReal cbmin = cbaxis.min();
  COptReal cbmax = cbaxis.max();

  //---

  // image size
  int iw = imageStyle.w.getValue(1);
//int ih = imageStyle.h.getValue(1);

  int nd = imageData.size();
  int ny = nd/iw;
  int nx = nd/ny;

  //---

  // scale factor
  double idx = 1, idy = 1;

  if (imageStyle.dx.isValid())
    idx = imageStyle.dx.getValue();
  if (imageStyle.dy.isValid())
    idy = imageStyle.dy.getValue();

  double ww = nx*idx;
  double hh = ny*idy;

  //---

  // center point
  double cx = 0, cy = 0;

  if (imageStyle.c.isValid()) {
    cx = imageStyle.c.getValue().x;
    cy = imageStyle.c.getValue().y;
  }
  else {
    cx = ww/2.0;
    cy = hh/2.0;
  }

  CPoint2D c(cx, cy);

  //---

  // origin point
  CPoint2D o;

  if (imageStyle.o.isValid()) {
    o = imageStyle.o.getValue();

    cx = o.x + ww/2;
    cy = o.y + hh/2;
  }
  else
    o = c;

  //---

  bool   rotate = imageStyle.a.isValid();
  double a      = -plot->imageAngle();

  double ra = CAngle::Deg2Rad(a);

  if (imageStyle.flipy) ra = -ra;

  CPoint2D po(cx - ww/2, cy + (imageStyle.flipy ? hh/2 : -hh/2));

  renderer->setAntiAlias(false);

  //---


  double gmax = 0;

  if (style_ == CGnuPlotTypes::PlotStyle::IMAGE) {
    int i = 0;

    for (int iy = 0; iy < ny; ++iy) {
      for (int ix = 0; ix < nx; ++ix, ++i) {
        CRGBA rgba = imageData[i];

        if (imageStyle.usingCols.numCols() > 0) {
          if (imageStyle.usingCols.numCols() > 1) {
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

  int i = 0;

  for (int iy = 0; iy < ny; ++iy) {
    double y = po.y + (imageStyle.flipy ? -1 : 1)*(iy*idy + 0.5);

    for (int ix = 0; ix < nx; ++ix, ++i) {
      double x = po.x + ix*idx + 0.5;

      CRGBA rgba = imageData[i];

      if (imageStyle.usingCols.numCols() > 0) {
        if (imageStyle.usingCols.numCols() > 1) {
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

      CPoint2D p(x, y);

      if (rotate || is3D) {
        CPoint2D p1 = (rotate ? CMathGeom2D::RotatePoint(p, ra, o) : p);

        if (! renderer->isPseudo() && (renderer->clip().isSet() && ! renderer->clip().inside(p1)))
          continue;

        for (int py = 0; py < npy; ++py) {
          for (int px = 0; px < npx; ++px) {
            CPoint2D p1(p.x + px*pw, p.y + py*ph);

            CPoint2D p2 = CMathGeom2D::RotatePoint(p1, ra, o);

            if (is3D) {
              CPoint3D p3(p2.x, p2.y, 0.0); // zmin ?

              renderer->drawPoint(p3, rgba);
            }
            else
              renderer->drawPoint(p2, rgba);
          }
        }
      }
      else {
        if (! renderer->isPseudo() && (renderer->clip().isSet() && ! renderer->clip().inside(p)))
          continue;

        CBBox2D bbox(p.x - idx/2.0, p.y - idy/2.0, p.x + idx/2.0, p.y + idy/2.0);

        renderer->fillRect(bbox, rgba);
      }
    }
  }

  renderer->setAntiAlias(true);
}

void
CGnuPlotStyleImageBase::
drawImage(CGnuPlotPlot *plot, CGnuPlotRenderer *renderer)
{
  double pw = 1, ph = 1;

  if (! renderer->isPseudo()) {
    pw = renderer->pixelWidthToWindowWidth  (1);
    ph = renderer->pixelHeightToWindowHeight(1);
  }

  int npx = (pw < 1 && pw > 0 ? int(1.0/pw + 0.5) : 1);
  int npy = (ph < 1 && ph > 0 ? int(1.0/ph + 0.5) : 1);

//const CGnuPlotPlot::ImageData &imageData  = plot->imageData();
  const CGnuPlotImageStyle      &imageStyle = plot->imageStyle();

  //---

  // image size
  int iw = imageStyle.w.getValue(1);
  int ih = imageStyle.h.getValue(1);

  //---

  double dx = 0, dy = 0;

  if (imageStyle.c.isValid()) {
    dx = imageStyle.c.getValue().x - iw/2.0;
    dy = imageStyle.c.getValue().y - ih/2.0;
  }

  CPoint2D po = imageStyle.o.getValue(CPoint2D(iw/2.0, ih/2.0));

  double xo = po.x;
  double yo = po.y;

  bool   rotate = imageStyle.a.isValid();
  double a      = -plot->imageAngle();

  double ra = CAngle::Deg2Rad(a);

  if (imageStyle.flipy) ra = -ra;

  CPoint2D o(xo + dx, yo + dy);

  int ny = plot->getPoints2D().size();

  int y = 0;

  for (const auto &p : plot->getPoints2D()) {
    double y1 = (imageStyle.flipy ? ny - 1 - y : y);

    std::vector<double> reals;

    (void) p.getReals(reals);

    int x = 0;

    for (const auto &v : reals) {
      double x1 = x;

      CRGBA rgba = CGnuPlotStyleInst->indexColor(int(v));

      CPoint2D p(x1 + dx, y1 + dy);

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
      else {
        if (! renderer->isPseudo() && (renderer->clip().isSet() && ! renderer->clip().inside(p)))
          continue;

        CBBox2D bbox(p.x - 0.5, p.y - 0.5, p.x + 0.5, p.y + 0.5);

        renderer->fillRect(bbox, rgba);
      }

      ++x;
    }

    ++y;
  }
}

double
CGnuPlotStyleImageBase::
decodeImageUsingColor(CGnuPlotPlot *plot, int col, const CRGBA &rgba) const
{
  const CGnuPlotImageStyle &imageStyle = plot->imageStyle();

  int nc = imageStyle.usingCols.numCols();

  if (col < nc) {
    if (imageStyle.usingCols.getCol(col).isInt)
      col = imageStyle.usingCols.getCol(col).ival;
    else {
      bool lookup = true;

      std::string expr = imageStyle.usingCols.getCol(col).str;

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

CBBox2D
CGnuPlotStyleImageBase::
fit(CGnuPlotPlot *)
{
  return CBBox2D();
}
