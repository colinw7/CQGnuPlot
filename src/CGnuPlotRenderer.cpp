#include <CGnuPlotRenderer.h>

void
CGnuPlotRenderer::
drawHAlignedText(const CPoint2D &pos, CHAlignType halign, int x_offset,
                 CVAlignType valign, int y_offset, const std::string &str, const CRGBA &c)
{
  CFontPtr font = getFont();

  /* Calculate width and height of string (max width and sum of heights of all lines) */
  double font_size = font->getCharAscent() + font->getCharDescent();

  double width1  = 0;
  double height1 = 0;

  std::string str1 = str;

  std::string::size_type pstr1 = str1.find('\n');

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
drawVAlignedText(const CPoint2D &pos, CHAlignType halign, int x_offset,
                 CVAlignType valign, int y_offset, const std::string &str, const CRGBA &c)
{
  CFontPtr font = getFont();

  /* Calculate width and height of string (max width and sum of heights of all lines) */
  double font_size = font->getCharHeight();

  double width1  = 0;
  double height1 = 0;

  std::string str1 = str;

  std::string::size_type pstr1 = str1.find('\n');

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
