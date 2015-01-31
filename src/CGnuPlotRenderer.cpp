#include <CGnuPlotRenderer.h>
#include <CGnuPlotWindow.h>
#include <CGnuPlotUtil.h>

#include <CMathGeom2D.h>

CGnuPlotRenderer::
CGnuPlotRenderer()
{
}

CGnuPlotRenderer::
~CGnuPlotRenderer()
{
}

bool
CGnuPlotRenderer::
clipLine(CPoint2D &p1, CPoint2D &p2)
{
  double x1 = p1.x, y1 = p1.y;
  double x2 = p2.x, y2 = p2.y;

  if (! CMathGeom2D::clipLine(clip_.getXMin(), clip_.getYMin(), clip_.getXMax(), clip_.getYMax(),
                              &x1, &y1, &x2, &y2))
    return false;

  p1 = CPoint2D(x1, y1);
  p2 = CPoint2D(x2, y2);

  return true;
}

void
CGnuPlotRenderer::
drawHAlignedText(const CPoint2D &pos, CHAlignType halign, double x_offset,
                 CVAlignType valign, double y_offset, const std::string &str, const CRGBA &c)
{
  CFontPtr font = getFont();

  /* Calculate width and height of string (max width and sum of heights of all lines) */
  double font_size = font->getCharAscent() + font->getCharDescent();

  double width1  = 0;
  double height1 = 0;

  std::string str1 = str;

  auto pstr1 = str1.find('\n');

  while (pstr1 != std::string::npos) {
    std::string str2 = str1.substr(0, pstr1);

    double width2 = font->getStringWidth(str2);

    width1   = std::max(width1, width2);
    height1 += font_size;

    str1  = str1.substr(pstr1 + 1);
    pstr1 = str1.find('\n');
  }

  double width2 = font->getStringWidth(str1);

  width1   = std::max(width1, width2);
  height1 += font_size;

  /*-------*/

  /* Draw each line using X and Y alignment */

  str1 = str;

  pstr1 = str1.find('\n');

  double x1, y1;

  if      (valign == CVALIGN_TYPE_TOP)
    y1 = pos.y;
  else if (valign == CVALIGN_TYPE_CENTER)
    y1 = pos.y + pixelHeightToWindowHeight(height1/2);
  else if (valign == CVALIGN_TYPE_BOTTOM)
    y1 = pos.y + pixelHeightToWindowHeight(height1);

  y1 -= pixelHeightToWindowHeight(font->getCharAscent());

  while (pstr1 != std::string::npos) {
    std::string str2 = str1.substr(0, pstr1);

    double width2 = font->getStringWidth(str2);

    if      (halign == CHALIGN_TYPE_LEFT)
      x1 = pos.x;
    else if (halign == CHALIGN_TYPE_CENTER)
      x1 = pos.x - pixelWidthToWindowWidth(width2/2);
    else if (halign == CHALIGN_TYPE_RIGHT)
      x1 = pos.x - pixelWidthToWindowWidth(width2);

    CPoint2D w(x1 + pixelWidthToWindowWidth  (x_offset),
               y1 - pixelHeightToWindowHeight(y_offset));

    drawText(w, str2, c);

    y1 -= pixelHeightToWindowHeight(font_size);

    str1 = str1.substr(pstr1 + 1);
    pstr1 = str1.find('\n');
  }

  width2 = font->getStringWidth(str1);

  if      (halign == CHALIGN_TYPE_LEFT)
    x1 = pos.x;
  else if (halign == CHALIGN_TYPE_CENTER)
    x1 = pos.x - pixelWidthToWindowWidth(width2/2);
  else if (halign == CHALIGN_TYPE_RIGHT)
    x1 = pos.x - pixelWidthToWindowWidth(width2);

  CPoint2D w(x1 + pixelWidthToWindowWidth  (x_offset),
             y1 - pixelHeightToWindowHeight(y_offset));

  drawText(w, str1, c);
}

void
CGnuPlotRenderer::
drawVAlignedText(const CPoint2D &pos, CHAlignType halign, double x_offset,
                 CVAlignType valign, double y_offset, const std::string &str, const CRGBA &c)
{
  CFontPtr font = getFont();

  /* Calculate width and height of string (max width and sum of heights of all lines) */
  double font_size = font->getCharHeight();

  double width1  = 0;
  double height1 = 0;

  std::string str1 = str;

  auto pstr1 = str1.find('\n');

  while (pstr1 != std::string::npos) {
    std::string str2 = str1.substr(0, pstr1);

    double width2 = font->getStringWidth(str2);

    width1   = std::max(width1, width2);
    height1 += font_size + 1;

    str1  = str1.substr(pstr1 + 1);
    pstr1 = str1.find('\n');
  }

  double width2 = font->getStringWidth(str1);

  width1   = std::max(width1, width2);
  height1 += font_size;

  /*-------*/

  /* Draw each line using X and Y alignment */

  str1 = str;

  pstr1 = str1.find('\n');

  double x1, y1;

  if      (halign == CHALIGN_TYPE_LEFT)
    x1 = pos.x;
  else if (halign == CHALIGN_TYPE_CENTER)
    x1 = pos.x - pixelWidthToWindowWidth(height1/2);
  else if (halign == CHALIGN_TYPE_RIGHT)
    x1 = pos.x - pixelWidthToWindowWidth(height1);

  x1 += pixelWidthToWindowWidth(font->getCharAscent());

  while (pstr1 != std::string::npos) {
    std::string str2 = str1.substr(0, pstr1);

    double width2 = font->getStringWidth(str2);

    if      (valign == CVALIGN_TYPE_TOP)
      y1 = pos.y;
    else if (valign == CVALIGN_TYPE_CENTER)
      y1 = pos.y - pixelHeightToWindowHeight(width2/2);
    else if (valign == CVALIGN_TYPE_BOTTOM)
      y1 = pos.y - pixelHeightToWindowHeight(width2);

    CPoint2D w(x1 + pixelWidthToWindowWidth  (x_offset),
               y1 - pixelHeightToWindowHeight(y_offset));

    drawText(w, str2, c);

    x1 += pixelWidthToWindowWidth(font_size + 1);

    str1  = str1.substr(pstr1 + 1);
    pstr1 = str1.find('\n');
  }

  width2 = font->getStringWidth(str1);

  if      (valign == CVALIGN_TYPE_TOP)
    y1 = pos.y;
  else if (valign == CVALIGN_TYPE_CENTER)
    y1 = pos.y - pixelHeightToWindowHeight(width2/2);
  else if (valign == CVALIGN_TYPE_BOTTOM)
    y1 = pos.y - pixelHeightToWindowHeight(width2);

  CPoint2D w(x1 + pixelWidthToWindowWidth  (x_offset),
             y1 - pixelHeightToWindowHeight(y_offset));

  drawRotatedText(w, str1, c, 90);
}

void
CGnuPlotRenderer::
drawRotatedText(const CPoint2D &p, const std::string &text, const CRGBA &c, double a)
{
  CFontPtr font  = getFont();
  CFontPtr rfont = font->rotated(a);

  setFont(rfont);

  drawText(p, text, c);

  setFont(font);
}

double
CGnuPlotRenderer::
pixelWidthToWindowWidth(double w)
{
  double wx1, wy1, wx2, wy2;

  pixelToWindow(0, 0, &wx1, &wy1);
  pixelToWindow(w, w, &wx2, &wy2);

  return wx2 - wx1;
}

double
CGnuPlotRenderer::
pixelHeightToWindowHeight(double h)
{
  double wx1, wy1, wx2, wy2;

  pixelToWindow(0, 0, &wx1, &wy1);
  pixelToWindow(h, h, &wx2, &wy2);

  return wy1 - wy2;
}

double
CGnuPlotRenderer::
windowWidthToPixelWidth(double w)
{
  double wx1, wy1, wx2, wy2;

  windowToPixel(0, 0, &wx1, &wy1);
  windowToPixel(w, w, &wx2, &wy2);

  return fabs(wx2 - wx1);
}

double
CGnuPlotRenderer::
windowHeightToPixelHeight(double h)
{
  double wx1, wy1, wx2, wy2;

  windowToPixel(0, 0, &wx1, &wy1);
  windowToPixel(h, h, &wx2, &wy2);

  return fabs(wy2 - wy1);
}

void
CGnuPlotRenderer::
windowToPixel(const CPoint2D &w, CPoint2D &p)
{
  windowToPixel(w.x, w.y, &p.x, &p.y);
}

void
CGnuPlotRenderer::
pixelToWindow(const CPoint2D &p, CPoint2D &w)
{
  pixelToWindow(p.x, p.y, &w.x, &w.y);
}

void
CGnuPlotRenderer::
windowToPixel(double wx, double wy, double *px, double *py)
{
  const CISize2D &s = window()->size();

  if (! mapping_) {
    *px = wx;
    *py = wy;
    return;
  }

  // place on screen
  double pxmin = region_.getLeft  ()*s.width;
  double pymin = region_.getBottom()*s.height;
  double pxmax = region_.getRight ()*s.width;
  double pymax = region_.getTop   ()*s.height;

  double pw = pxmax - pxmin;
  double ph = pymax - pymin;

  // add margin
  double ps = std::min(pw, ph);

  double lmargin = ps*margin_.left  ()/100.0;
  double rmargin = ps*margin_.right ()/100.0;
  double tmargin = ps*margin_.top   ()/100.0;
  double bmargin = ps*margin_.bottom()/100.0;

  pxmin += lmargin; pxmax -= rmargin;
  pymin += tmargin; pymax -= bmargin;

  // adjust for ratio
  if (ratio_.isValid() && ratio_.getValue() > 0.0) {
    double ratio = ratio_.getValue();

    pw = pxmax - pxmin;
    ph = pymax - pymin;

    double pr = (1.0*pw)/ph;

    double pw1 = pw*ratio/pr;
    double ph1 = ph;

    if (pw1 > pw) {
      ph1 *= pw/pw1;
      pw1  = pw;
    }

    double pxc = (pxmin + pxmax)/2.0;
    double pyc = (pymin + pymax)/2.0;

    pxmin = pxc - pw1/2.0; pxmax = pxc + pw1/2.0;
    pymin = pyc - ph1/2.0; pymax = pyc + ph1/2.0;
  }

  // map
  *px = CGnuPlotUtil::map(wx, range_.getXMin(), range_.getXMax(), pxmin, pxmax);
  *py = CGnuPlotUtil::map(wy, range_.getYMin(), range_.getYMax(), pymax, pymin);
}

void
CGnuPlotRenderer::
pixelToWindow(double px, double py, double *wx, double *wy)
{
  const CISize2D &s = window()->size();

  if (! mapping_) {
    *wx = px;
    *wy = py;
    return;
  }

  // place on screen
  double pxmin = region_.getLeft  ()*s.width;
  double pymin = region_.getBottom()*s.height;
  double pxmax = region_.getRight ()*s.width;
  double pymax = region_.getTop   ()*s.height;

  double pw = pxmax - pxmin;
  double ph = pymax - pymin;

  // add margin
  double ps = std::min(pw, ph);

  double lmargin = ps*margin_.left  ()/100.0;
  double rmargin = ps*margin_.right ()/100.0;
  double tmargin = ps*margin_.top   ()/100.0;
  double bmargin = ps*margin_.bottom()/100.0;

  pxmin += lmargin; pxmax -= rmargin;
  pymin += tmargin; pymax -= bmargin;

  // adjust for ratio
  if (ratio_.isValid() && ratio_.getValue() > 0.0) {
    double ratio = ratio_.getValue();

    pw = pxmax - pxmin;
    ph = pymax - pymin;

    double pr = (1.0*pw)/ph;

    double pw1 = pw*ratio/pr;
    double ph1 = ph;

    if (pw1 > pw) {
      ph1 *= pw/pw1;
      pw1  = pw;
    }

    double pxc = (pxmin + pxmax)/2.0;
    double pyc = (pymin + pymax)/2.0;

    pxmin = pxc - pw1/2.0; pxmax = pxc + pw1/2.0;
    pymin = pyc - ph1/2.0; pymax = pyc + ph1/2.0;
  }

  // map
  *wx = CGnuPlotUtil::map(px, pxmin, pxmax, range_.getXMin(), range_.getXMax());
  *wy = CGnuPlotUtil::map(py, pymax, pymin, range_.getYMin(), range_.getYMax());
}
