#include <CGnuPlotTextRenderer.h>
#include <CFontMgr.h>
#include <CMathGeom2D.h>
#include <CAngle.h>

namespace Util {
  template<typename T>
  inline T sign(const T &t) {
    if (t > T(0)) return  1;
    if (t < T(0)) return -1;
    return 0;
  }
}

//---

CGnuPlotTextRenderer::
CGnuPlotTextRenderer()
{
  font_ = CFontMgrInst->lookupFont("helvetica", CFONT_STYLE_NORMAL, 12);
}

void
CGnuPlotTextRenderer::
windowToPixel(const CPoint2D &w, CPoint2D &p)
{
  windowToPixel(w.x, w.y, &p.x, &p.y);
}

void
CGnuPlotTextRenderer::
pixelToWindow(const CPoint2D &p, CPoint2D &w)
{
  pixelToWindow(p.x, p.y, &w.x, &w.y);
}

double
CGnuPlotTextRenderer::
pixelWidthToWindowWidth(double w)
{
  double wx1, wy1, wx2, wy2;

  pixelToWindow(0, 0, &wx1, &wy1);
  pixelToWindow(w, w, &wx2, &wy2);

  return Util::sign(w)*fabs(wx2 - wx1);
}

double
CGnuPlotTextRenderer::
pixelHeightToWindowHeight(double h)
{
  double wx1, wy1, wx2, wy2;

  pixelToWindow(0, 0, &wx1, &wy1);
  pixelToWindow(h, h, &wx2, &wy2);

  return Util::sign(h)*fabs(wy1 - wy2);
}

double
CGnuPlotTextRenderer::
windowWidthToPixelWidth(double w)
{
  double wx1, wy1, wx2, wy2;

  windowToPixel(0, 0, &wx1, &wy1);
  windowToPixel(w, w, &wx2, &wy2);

  return Util::sign(w)*fabs(wx2 - wx1);
}

double
CGnuPlotTextRenderer::
windowHeightToPixelHeight(double h)
{
  double wx1, wy1, wx2, wy2;

  windowToPixel(0, 0, &wx1, &wy1);
  windowToPixel(h, h, &wx2, &wy2);

  return Util::sign(h)*fabs(wy2 - wy1);
}

CPoint2D
CGnuPlotTextRenderer::
rotatePoint(const CPoint2D &p, double a, const CPoint2D &o)
{
  double a1 = CAngle::Deg2Rad(a);

  CPoint2D po;

  windowToPixel(o, po);

  CPoint2D p1;

  windowToPixel(p, p1);

  CPoint2D pr1 = CMathGeom2D::RotatePoint(p1, -a1, po);

  CPoint2D pr;

  pixelToWindow(pr1, pr);

  return pr;
}

//------

CGnuPlotBBoxTextRenderer::
CGnuPlotBBoxTextRenderer(CGnuPlotTextRenderer *renderer) :
 renderer_(renderer), bbox_()
{
  font_ = renderer_->getFont();
}

void
CGnuPlotBBoxTextRenderer::
setFont(const CFontPtr &font)
{
  renderer_->setFont(font);

  CGnuPlotTextRenderer::setFont(font);
}

void
CGnuPlotBBoxTextRenderer::
pixelToWindow(double px, double py, double *wx, double *wy)
{
  renderer_->pixelToWindow(px, py, wx, wy);
}

void
CGnuPlotBBoxTextRenderer::
windowToPixel(double wx, double wy, double *px, double *py)
{
  renderer_->windowToPixel(wx, wy, px, py);
}

void
CGnuPlotBBoxTextRenderer::
drawText(const CPoint2D &p, const std::string &str, const CRGBA &)
{
  CFontPtr font = renderer_->getFont();

  double fa = renderer_->pixelHeightToWindowHeight(font->getCharAscent ());
  double fd = renderer_->pixelHeightToWindowHeight(font->getCharDescent());
  double fw = renderer_->pixelWidthToWindowWidth  (font->getStringWidth(str));

  bbox_.add(CPoint2D(p.x     , p.y - fd));
  bbox_.add(CPoint2D(p.x + fw, p.y + fa));
}

void
CGnuPlotBBoxTextRenderer::
drawRotatedText(const CPoint2D &p, const std::string &str, double /*a*/,
                CHAlignType, CVAlignType, const CRGBA &)
{
  CFontPtr font = renderer_->getFont();

  double fa = renderer_->pixelHeightToWindowHeight(font->getCharAscent ());
  double fd = renderer_->pixelHeightToWindowHeight(font->getCharDescent());
  double fw = renderer_->pixelWidthToWindowWidth  (font->getStringWidth(str));

  bbox_.add(CPoint2D(p.x     , p.y - fd));
  bbox_.add(CPoint2D(p.x + fw, p.y + fa));
}
