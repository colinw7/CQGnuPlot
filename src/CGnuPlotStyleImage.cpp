#include <CGnuPlotStyleImage.h>
#include <CGnuPlotPlot.h>
#include <CGnuPlotRenderer.h>
#include <CMathGeom2D.h>

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

//------

void
CGnuPlotStyleImageBase::
drawBinaryImage(CGnuPlotPlot *plot, CGnuPlotRenderer *renderer)
{
  double pw = renderer->pixelWidthToWindowWidth  (1);
  double ph = renderer->pixelHeightToWindowHeight(1);

  int npx = (pw < 1 && pw > 0 ? int(1.0/pw + 0.5) : 1);
  int npy = (ph < 1 && ph > 0 ? int(1.0/ph + 0.5) : 1);

  const CGnuPlotPlot::ImageData &imageData  = plot->imageData();
  const CGnuPlotImageStyle      &imageStyle = plot->imageStyle();

  int iw = imageStyle.w.getValue(1);
  int ih = imageStyle.h.getValue(1);

  int nd = imageData.size();
  int ny = nd/iw;
  int nx = nd/ny;

  double dx = 0, dy = 0;

  if (imageStyle.c.isValid()) {
    dx = imageStyle.c.getValue().x - iw/2.0;
    dy = imageStyle.c.getValue().y - ih/2.0;
  }

  CPoint2D po = imageStyle.o.getValue(CPoint2D(iw/2.0, ih/2.0));

  double xo = po.x;
  double yo = po.y;

  double idx = 1, idy = 1;

  if (imageStyle.dx.isValid())
    idx = imageStyle.dx.getValue();
  if (imageStyle.dy.isValid())
    idy = imageStyle.dy.getValue();

  bool   rotate = imageStyle.a.isValid();
  double a      = -plot->imageAngle();

  double ra = CAngle::Deg2Rad(a);

  if (imageStyle.flipy) ra = -ra;

  CPoint2D o(xo + dx, yo + dy);

//double ww = nx*idx;
  double hh = ny*idy;

  int i = 0;

  for (int iy = 0; iy < ny; ++iy) {
    double y = o.y + (imageStyle.flipy ? hh - iy*idy - idy : iy*idy) + dy;

    for (int ix = 0; ix < nx; ++ix, ++i) {
      double x = o.x + ix*idx + dx;

      CRGBA c = imageData[i];

      if (! imageStyle.usingCols.empty()) {
        if (imageStyle.usingCols.size() > 1) {
          double r = decodeImageUsingColor(plot, 0, c);
          double g = decodeImageUsingColor(plot, 1, c);
          double b = decodeImageUsingColor(plot, 2, c);

          c = CRGBA(r, g, b);
        }
        else {
          double g = decodeImageUsingColor(plot, 0, c);

          c = CRGBA(g, g, g);
        }
      }

      CPoint2D p(x, y);

      if (rotate) {
        CPoint2D p1 = CMathGeom2D::RotatePoint(p, ra, o);

        if (! renderer->clip().inside(p1))
          continue;

        for (int py = 0; py < npy; ++py) {
          for (int px = 0; px < npx; ++px) {
            CPoint2D p1(p.x + px*pw, p.y + py*ph);

            CPoint2D p2 = CMathGeom2D::RotatePoint(p1, ra, o);

            renderer->drawPoint(p2, c);
          }
        }
      }
      else {
        if (! renderer->clip().inside(p))
          continue;

        CBBox2D bbox(p.x - idx/2.0, p.y - idy/2.0, p.x + idx/2.0, p.y + idy/2.0);

        renderer->fillRect(bbox, c);
      }
    }
  }
}

void
CGnuPlotStyleImageBase::
drawImage(CGnuPlotPlot *plot, CGnuPlotRenderer *renderer)
{
  double pw = renderer->pixelWidthToWindowWidth  (1);
  double ph = renderer->pixelHeightToWindowHeight(1);

  int npx = (pw < 1 && pw > 0 ? int(1.0/pw + 0.5) : 1);
  int npy = (ph < 1 && ph > 0 ? int(1.0/ph + 0.5) : 1);

//const CGnuPlotPlot::ImageData &imageData  = plot->imageData();
  const CGnuPlotImageStyle      &imageStyle = plot->imageStyle();

  int iw = imageStyle.w.getValue(1);
  int ih = imageStyle.h.getValue(1);

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

      CRGBA c = CGnuPlotStyleInst->indexColor(int(v));

      CPoint2D p(x1 + dx, y1 + dy);

      if (rotate) {
        CPoint2D p1 = CMathGeom2D::RotatePoint(p, ra, o);

        if (! renderer->clip().inside(p1))
          continue;

        for (int py = 0; py < npy; ++py) {
          for (int px = 0; px < npx; ++px) {
            CPoint2D p1(p.x + px*pw, p.y + py*ph);

            CPoint2D p2 = CMathGeom2D::RotatePoint(p1, ra, o);

            renderer->drawPoint(p2, c);
          }
        }
      }
      else {
        if (! renderer->clip().inside(p))
          continue;

        CBBox2D bbox(p.x - 0.5, p.y - 0.5, p.x + 0.5, p.y + 0.5);

        renderer->fillRect(bbox, c);
      }

      ++x;
    }

    ++y;
  }
}

double
CGnuPlotStyleImageBase::
decodeImageUsingColor(CGnuPlotPlot *plot, int col, const CRGBA &c) const
{
  const CGnuPlotImageStyle &imageStyle = plot->imageStyle();

  int nc = imageStyle.usingCols.size();

  if (col < nc) {
    if (imageStyle.usingCols[col].isInt)
      col = imageStyle.usingCols[col].ival;
    else {
      bool lookup = true;

      std::string expr = imageStyle.usingCols[col].str;

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

        double r = indexImageColor(icol, c);

        expr = expr.substr(0, pos - 1) + CStrUtil::toString(r) + expr.substr(pos1);

        pos = expr.find('$');
      }

      if (expr.size() > 2 && expr[0] == '(' && expr[expr.size() - 1] == ')') {
        lookup = false;

        expr = expr.substr(1, expr.size() - 2);
      }

      CExprValueP value;

      if (! CExprInst->evaluateExpression(expr, value))
        value = CExprValueP();

      if (! lookup) {
        double r;

        if (value.isValid() && value->getRealValue(r))
          return r;
      }
      else {
        CExprValueP value;

        if (! CExprInst->evaluateExpression(expr, value))
          value = CExprValueP();

        long l;

        if (value.isValid() && value->getIntegerValue(l))
          return indexImageColor(l, c);
      }

      return 1;
    }
  }

  return indexImageColor(col, c);
}

double
CGnuPlotStyleImageBase::
indexImageColor(int i, const CRGBA &c) const
{
  switch (i) {
    case 1 : return c.getRed  ();
    case 2 : return c.getGreen();
    case 3 : return c.getBlue ();
    default: return 1;
  }
}

CBBox2D
CGnuPlotStyleImageBase::
fit(CGnuPlotPlot *)
{
  return CBBox2D();
}
