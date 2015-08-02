#include <CGnuPlotRenderer.h>
#include <CGnuPlotWindow.h>
#include <CGnuPlotUtil.h>

#include <CFontMgr.h>
#include <CMathGeom2D.h>

CGnuPlotRenderer::
CGnuPlotRenderer()
{
  font_ = CFontMgrInst->lookupFont("helvetica", CFONT_STYLE_NORMAL, 12);
}

CGnuPlotRenderer::
~CGnuPlotRenderer()
{
}

void
CGnuPlotRenderer::
setFont(CFontPtr font)
{
  font_ = font;
}

double
CGnuPlotRenderer::
fontSize() const
{
  return font_->getSize();
}

void
CGnuPlotRenderer::
setFontSize(double s)
{
  font_ = font_->dup(font_->getFamily(), font_->getStyle(), s);
}

void
CGnuPlotRenderer::
fillPolygon(const std::vector<CPoint3D> &points, const CRGBA &c)
{
  std::vector<CPoint2D> points1;

  for (const auto &p : points) {
    CPoint2D p1 = transform(p);

    points1.push_back(p1);
  }

  fillPolygon(points1, c);
}

void
CGnuPlotRenderer::
drawPolygon(const std::vector<CPoint3D> &points, double lw, const CRGBA &c)
{
  std::vector<CPoint2D> points1;

  for (const auto &p : points) {
    CPoint2D p1 = transform(p);

    points1.push_back(p1);
  }

  drawPolygon(points1, lw, c);
}

void
CGnuPlotRenderer::
drawClippedPolygon(const std::vector<CPoint2D> &points, double w, const CRGBA &c)
{
  if (! isPseudo()) {
    std::vector<CPoint2D> ipoints;

    if (CMathGeom2D::IntersectPolygon(points, clip(), ipoints))
      drawPolygon(ipoints, w, c);
  }
  else
    drawPolygon(points, w, c);
}

void
CGnuPlotRenderer::
fillClippedPolygon(const std::vector<CPoint2D> &points, const CRGBA &c)
{
  if (! isPseudo()) {
    std::vector<CPoint2D> ipoints;

    if (CMathGeom2D::IntersectPolygon(points, clip(), ipoints))
      fillPolygon(ipoints, c);
  }
  else
    fillPolygon(points, c);
}

void
CGnuPlotRenderer::
patternClippedPolygon(const std::vector<CPoint2D> &points, CGnuPlotTypes::FillPattern pattern,
                     const CRGBA &fg, const CRGBA &bg)
{
  if (! isPseudo()) {
    std::vector<CPoint2D> ipoints;

    if (CMathGeom2D::IntersectPolygon(points, clip(), ipoints))
      patternPolygon(ipoints, pattern, fg, bg);
  }
  else
    patternPolygon(points, pattern, fg, bg);
}

void
CGnuPlotRenderer::
drawClippedRect(const CBBox2D &rect, const CRGBA &c, double w)
{
  if (clip_.isSet() && ! isPseudo()) {
    if      (clip_.inside(rect))
      drawRect(rect, c, w);
    else if (clip_.intersect(rect)) {
      drawClipLine(rect.getLL(), rect.getLR(), w, c);
      drawClipLine(rect.getLR(), rect.getUR(), w, c);
      drawClipLine(rect.getUR(), rect.getUL(), w, c);
      drawClipLine(rect.getUL(), rect.getLL(), w, c);
    }
  }
  else
    drawRect(rect, c, w);
}

void
CGnuPlotRenderer::
fillClippedRect(const CBBox2D &rect, const CRGBA &c)
{
  if (clip_.isSet() && ! isPseudo()) {
    CBBox2D crect;

    if      (clip_.inside(rect))
      fillRect(rect, c);
    else if (clip_.intersect(rect, crect))
      fillRect(crect, c);
  }
  else
    fillRect(rect, c);
}

void
CGnuPlotRenderer::
drawClipLine(const CPoint2D &p1, const CPoint2D &p2, double width, const CRGBA &c,
             const CLineDash &dash)
{
  if (! isPseudo()) {
    CPoint2D p11 = p1;
    CPoint2D p21 = p2;

    if (clipLine(p11, p21))
      drawLine(p11, p21, width, c, dash);
  }
  else
    drawLine(p1, p2, width, c, dash);
}

bool
CGnuPlotRenderer::
clipLine(CPoint2D &p1, CPoint2D &p2)
{
  if (! clip_.isSet() || isPseudo())
    return true;

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
drawEllipse(const CPoint3D &pos, double dx, double ry, double angle, const CRGBA &c,
            double w, const CLineDash &dash)
{
  CPoint2D pos1 = transform(pos);

  drawEllipse(pos1, dx, ry, angle, c, w, dash);
}

void
CGnuPlotRenderer::
drawHAlignedText(const CPoint3D &pos, CHAlignType halign, double x_offset,
                 CVAlignType valign, double y_offset, const std::string &str,
                 const CRGBA &c, double a)
{
  CPoint2D pos1 = transform(pos);

  drawHAlignedText(pos1, halign, x_offset, valign, y_offset, str, c, a);
}

void
CGnuPlotRenderer::
drawHAlignedText(const CPoint2D &pos, CHAlignType halign, double x_offset,
                 CVAlignType valign, double y_offset, const std::string &str,
                 const CRGBA &c, double a)
{
  if (fabs(a) > 1E-6)
    return drawRotatedText(pos, str, a, halign, valign, c);

  CFontPtr font = getFont();

  /* Calculate width and height of string (max width and sum of heights of all lines) */
  double font_size = font->getCharAscent() + font->getCharDescent();
//double font_size = font->getCharHeight();

  CBBox2D bbox = getHAlignedTextBBox(str);

//double width1  = bbox.getWidth();
  double height1 = bbox.getHeight();

  //------

  // Draw each line using X and Y alignment

  std::string str1 = str;

  auto pstr1 = str1.find('\n');

  double x1 = 0, y1 = 0;

  if      (valign == CVALIGN_TYPE_TOP)
    y1 = pos.y;
  else if (valign == CVALIGN_TYPE_CENTER)
    y1 = pos.y + height1/2;
  else if (valign == CVALIGN_TYPE_BOTTOM)
    y1 = pos.y + height1;

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

    CPoint2D p1(x1 + pixelWidthToWindowWidth  (x_offset),
                y1 - pixelHeightToWindowHeight(y_offset));

    drawText(p1, str2, c);

    y1 -= pixelHeightToWindowHeight(font_size);

    str1 = str1.substr(pstr1 + 1);
    pstr1 = str1.find('\n');
  }

  double width2 = font->getStringWidth(str1);

  if      (halign == CHALIGN_TYPE_LEFT)
    x1 = pos.x;
  else if (halign == CHALIGN_TYPE_CENTER)
    x1 = pos.x - pixelWidthToWindowWidth(width2/2);
  else if (halign == CHALIGN_TYPE_RIGHT)
    x1 = pos.x - pixelWidthToWindowWidth(width2);

  CPoint2D p1(x1 + pixelWidthToWindowWidth  (x_offset),
              y1 - pixelHeightToWindowHeight(y_offset));

  drawText(p1, str1, c);
}

void
CGnuPlotRenderer::
drawHTextInBox(const CBBox2D &bbox, const std::string &str, CHAlignType halign, const CRGBA &c)
{
  CFontPtr font = getFont();

  double fa = pixelHeightToWindowHeight(font->getCharAscent());
  double fd = pixelHeightToWindowHeight(font->getCharDescent());

  //------

  // Draw each line using X and Y alignment

  std::string str1 = str;

  auto p1 = str1.find('\n');

  double y = bbox.getTop() - fa;

  while (p1 != std::string::npos) {
    std::string str2 = str1.substr(0, p1);

    double w = pixelWidthToWindowWidth(font->getStringWidth(str2));

    double x = bbox.getLeft();

    if      (halign == CHALIGN_TYPE_LEFT)
      ;
    else if (halign == CHALIGN_TYPE_CENTER)
      x = bbox.getCenter().x - w/2;
    else if (halign == CHALIGN_TYPE_RIGHT)
      x = bbox.getRight() - w;

    drawText(CPoint2D(x, y), str2, c);

    y -= fa + fd;

    str1 = str1.substr(p1 + 1);
    p1   = str1.find('\n');
  }

  double w = pixelWidthToWindowWidth(font->getStringWidth(str1));

  double x = bbox.getLeft();

  if      (halign == CHALIGN_TYPE_LEFT)
    ;
  else if (halign == CHALIGN_TYPE_CENTER)
    x = bbox.getCenter().x - w/2;
  else if (halign == CHALIGN_TYPE_RIGHT)
    x = bbox.getRight() - w;

  drawText(CPoint2D(x, y), str1, c);
}

void
CGnuPlotRenderer::
drawVAlignedText(const CPoint3D &pos, CHAlignType halign, double x_offset,
                 CVAlignType valign, double y_offset, const std::string &str,
                 const CRGBA &c, double a)
{
  CPoint2D pos1 = transform(pos);

  drawVAlignedText(pos1, halign, x_offset, valign, y_offset, str, c, a);
}

void
CGnuPlotRenderer::
drawVAlignedText(const CPoint2D &pos, CHAlignType halign, double x_offset,
                 CVAlignType valign, double y_offset, const std::string &str,
                 const CRGBA &c, double a)
{
  if (fabs(a) > 1E-6)
    return drawRotatedText(pos, str, a, halign, valign, c);

  CFontPtr font = getFont();

  /* Calculate width and height of string (max width and sum of heights of all lines) */
  double font_size = font->getCharHeight();

  double width1  = 0;
  double height1 = 0;

  //------

  // Draw each line using X and Y alignment

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

  double x1 = 0, y1 = 0;

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

  CFontPtr rfont = font->rotated(90);

  setFont(rfont);

  drawText(w, str1, c);

  setFont(font);
}

CBBox2D
CGnuPlotRenderer::
getHAlignedTextBBox(const std::string &str)
{
  CPoint2D pos(0, 0);
  CBBox2D  bbox(0, 0, 0, 0);

  CFontPtr font = getFont();

  double fa = pixelHeightToWindowHeight(font->getCharAscent());
  double fd = pixelHeightToWindowHeight(font->getCharDescent());

  std::string str1 = str;

  auto pstr1 = str1.find('\n');

  while (pstr1 != std::string::npos) {
    std::string str2 = str1.substr(0, pstr1);

    double w = pixelWidthToWindowWidth(font->getStringWidth(str2));

    bbox.add(CPoint2D(pos.x + w, pos.y - fd));
    bbox.add(CPoint2D(pos.x + w, pos.y + fa));

    pos.y -= fa + fd;

    str1  = str1.substr(pstr1 + 1);
    pstr1 = str1.find('\n');
  }

  double w = pixelWidthToWindowWidth(font->getStringWidth(str1));

  bbox.add(CPoint2D(pos.x + w, pos.y - fd));
  bbox.add(CPoint2D(pos.x + w, pos.y + fa));

  return bbox;
}

void
CGnuPlotRenderer::
drawPath(const std::vector<CPoint3D> &points, double width, const CRGBA &c,
         const CLineDash &dash)
{
  std::vector<CPoint2D> points1;

  for (const auto &p : points)
    points1.push_back(transform(p));

  drawPath(points1, width, c, dash);
}

void
CGnuPlotRenderer::
drawClippedPath(const std::vector<CPoint2D> &points, double width, const CRGBA &c,
                const CLineDash &dash)
{
  int np = points.size();
  if (np < 2) return;

  std::vector<CPoint2D> points1;

  for (int i = 1; i < np; ++i) {
    CPoint2D p1 = points[i - 1];
    CPoint2D p2 = points[i    ];

    if (clipLine(p1, p2)) {
      if (points1.empty() || points1.back() != p1)
        points1.push_back(p1);

      points1.push_back(p2);
    }
    else {
      if (! points1.empty())
        drawPath(points1, width, c, dash);

      points1.clear();
    }
  }

  if (! points1.empty())
    drawPath(points1, width, c, dash);
}

void
CGnuPlotRenderer::
drawClippedPath(const std::vector<CPoint3D> &points, double width, const CRGBA &c,
                const CLineDash &dash)
{
  if (clip_.isSet() && ! isPseudo()) {
    std::vector<CPoint2D> points1;

    for (const auto &p : points) {
      CPoint2D p1 = transform(p);

      if (clip_.inside(p1))
        points1.push_back(p1);
    }

    drawPath(points1, width, c, dash);
  }
  else
    drawPath(points, width, c, dash);
}

void
CGnuPlotRenderer::
drawSymbol(const CPoint3D &p, SymbolType type, double size, const CRGBA &c, double lw)
{
  drawSymbol(transform(p), type, size, c, lw);
}

void
CGnuPlotRenderer::
drawPoint(const CPoint3D &p, const CRGBA &c)
{
  drawPoint(transform(p), c);
}

void
CGnuPlotRenderer::
drawLine(const CPoint3D &p1, const CPoint3D &p2, double width, const CRGBA &c,
         const CLineDash &dash)
{
  drawLine(transform(p1), transform(p2), width, c, dash);
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
  if (! mapping_) {
    *px = wx;
    *py = wy;
    return;
  }

  // place on screen
  CPoint2D p1, p2;

  regionToPixel(region_.getLL(), p1);
  regionToPixel(region_.getUR(), p2);

  double pxmin = p1.x, pymin = p2.y;
  double pxmax = p2.x, pymax = p1.y;

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
  if (reverseX_)
    *px = CGnuPlotUtil::map(wx, range_.getXMin(), range_.getXMax(), pxmax, pxmin);
  else
    *px = CGnuPlotUtil::map(wx, range_.getXMin(), range_.getXMax(), pxmin, pxmax);

  if (reverseY_)
    *py = CGnuPlotUtil::map(wy, range_.getYMin(), range_.getYMax(), pymin, pymax);
  else
    *py = CGnuPlotUtil::map(wy, range_.getYMin(), range_.getYMax(), pymax, pymin);
}

void
CGnuPlotRenderer::
pixelToWindow(double px, double py, double *wx, double *wy)
{
  if (! mapping_) {
    *wx = px;
    *wy = py;
    return;
  }

  // place on screen
  CPoint2D p1, p2;

  regionToPixel(region_.getLL(), p1);
  regionToPixel(region_.getUR(), p2);

  double pxmin = p1.x, pymin = p2.y;
  double pxmax = p2.x, pymax = p1.y;

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
  if (reverseX_)
    *wx = CGnuPlotUtil::map(px, pxmax, pxmin, range_.getXMin(), range_.getXMax());
  else
    *wx = CGnuPlotUtil::map(px, pxmin, pxmax, range_.getXMin(), range_.getXMax());

  if (reverseY_)
    *wy = CGnuPlotUtil::map(py, pymin, pymax, range_.getYMin(), range_.getYMax());
  else
    *wy = CGnuPlotUtil::map(py, pymax, pymin, range_.getYMin(), range_.getYMax());
}

void
CGnuPlotRenderer::
regionToPixel(const CPoint2D &r, CPoint2D &p)
{
  p = CPoint2D(r.x*(width() - 1), (1 - r.y)*(height() - 1));
}

CPoint2D
CGnuPlotRenderer::
transform(const CPoint3D &p) const
{
  if (camera_.isValid()) {
    CPoint3D p1 = camera_->transform(p);

    return CPoint2D(p1.x, p1.y);
  }
  else
    return CPoint2D(p.x, p.y);
}
