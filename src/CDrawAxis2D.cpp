#include <CDrawAxis2D.h>
#include <CFont.h>

CDrawAxis2D::
CDrawAxis2D() :
 pixel_size_(1)
{
}

void
CDrawAxis2D::
setPixelSize(double pixel_size)
{
  pixel_size_ = pixel_size;
}

void
CDrawAxis2D::
drawAxesAndGrid(double xmin, double ymin, double xmax, double ymax, double xc, double yc)
{
  CAxis2D x_axis(xmin, xmax);
  CAxis2D y_axis(ymin, ymax);

  drawAxis(x_axis, yc, CORIENTATION_HORIZONTAL);
  drawAxis(y_axis, xc, CORIENTATION_VERTICAL  );

  drawGrid(x_axis, y_axis);
}

void
CDrawAxis2D::
drawAxis(const CAxis2D &axis, double pos, COrientation direction)
{
  std::string str;

  if (! axis.getDisplayed())
    return;

  getFont(font_);

  if (direction == CORIENTATION_VERTICAL) {
    CFontPtr rfont = font_->rotated(90);

    setFont(rfont);
  }
  else
    setFont(font_);

  /*------*/

  /* Draw Axis Line */

  if (direction == CORIENTATION_HORIZONTAL)
    drawLine(CPoint2D(axis.getStart(), pos), CPoint2D(axis.getEnd(), pos));
  else
    drawLine(CPoint2D(pos, axis.getStart()), CPoint2D(pos, axis.getEnd()));

  /*------*/

  /* Draw Ticks Marks and Labels */

  double increment = axis.getMajorIncrement();

  double pos1 = axis.getStart();
  double pos2 = pos1;

  for (uint i = 0; i < axis.getNumMajorTicks(); i++) {
    /* Calculate Next Tick Point */

    pos2 = (i + 1)*increment + axis.getStart();

    /*------*/

    /* Draw Major Tick */

    if (pos1 >= axis.getStart() && pos1 <= axis.getEnd()) {
      /* Draw Tick Mark */

      if (direction == CORIENTATION_HORIZONTAL)
        drawAxisTick(CPoint2D(pos1, pos), CDIRECTION_TYPE_DOWN, true);
      else
        drawAxisTick(CPoint2D(pos, pos1), CDIRECTION_TYPE_LEFT, true);

      /*------*/

      /* Create Tick Label (Override with application supplied string if required) */

      str = axis.getValueStr(pos1);

      /*------*/

      /* Draw Tick Label */

      if (direction == CORIENTATION_HORIZONTAL)
        drawXString(CPoint2D(pos1, pos ), CHALIGN_TYPE_CENTER, 0, CVALIGN_TYPE_TOP   ,  8, str);
      else
        drawYString(CPoint2D(pos , pos1), CHALIGN_TYPE_RIGHT , 0, CVALIGN_TYPE_CENTER, -8, str);
    }

    /*------*/

    /* Draw Minor Ticks (use user defined distribution if defined) */

    if (axis.getTickSpaces() == NULL) {
      for (uint j = 1; j < axis.getNumMinorTicks(); j++) {
        double pos3 = j*(pos2 - pos1)/axis.getNumMinorTicks() + pos1;

        if (pos3 >= axis.getStart() && pos3 <= axis.getEnd()) {
          if (direction == CORIENTATION_HORIZONTAL)
            drawAxisTick(CPoint2D(pos3, pos), CDIRECTION_TYPE_DOWN, false);
          else
            drawAxisTick(CPoint2D(pos, pos3), CDIRECTION_TYPE_LEFT, false);
        }
      }
    }
    else {
      for (uint j = 1; j <= axis.getNumTickSpaces() - 2; j++) {
        double pos3 = axis.getTickSpace(j)*(pos2 - pos1) + pos1;

        if (pos3 >= axis.getStart() && pos3 <= axis.getEnd()) {
          if (direction == CORIENTATION_HORIZONTAL)
            drawAxisTick(CPoint2D(pos3, pos), CDIRECTION_TYPE_DOWN, false);
          else
            drawAxisTick(CPoint2D(pos, pos3), CDIRECTION_TYPE_LEFT, false);
        }
      }
    }

    /*------*/

    pos1 = pos2;
  }

  /*------*/

  /* Draw Last Major Tick */

  if (pos1 >= axis.getStart() && pos1 <= axis.getEnd()) {
    /* Draw Tick Mark */

    if (direction == CORIENTATION_HORIZONTAL)
      drawAxisTick(CPoint2D(pos1, pos), CDIRECTION_TYPE_DOWN, true);
    else
      drawAxisTick(CPoint2D(pos, pos1), CDIRECTION_TYPE_LEFT, true);

    /*------*/

    /* Create Tick Label (Override with application supplied string if required) */

    str = axis.getValueStr(pos1);

    /*------*/

    /* Draw Tick Label */

    if (direction == CORIENTATION_HORIZONTAL)
      drawXString(CPoint2D(pos1, pos ), CHALIGN_TYPE_CENTER, 0, CVALIGN_TYPE_TOP   ,  8, str);
    else
      drawYString(CPoint2D(pos , pos1), CHALIGN_TYPE_RIGHT , 0, CVALIGN_TYPE_CENTER, -8, str);
  }

  /*------*/

  /* Draw X Axis Label */

  double mid = (axis.getStart() + axis.getEnd())/2;

  const std::string &astr = axis.getLabel();

  if (direction == CORIENTATION_HORIZONTAL)
    drawXString(CPoint2D(mid, pos), CHALIGN_TYPE_CENTER, 0, CVALIGN_TYPE_TOP   ,  32, astr);
  else
    drawYString(CPoint2D(pos, mid), CHALIGN_TYPE_RIGHT , 0, CVALIGN_TYPE_CENTER, -32, astr);

  /*------*/

  if (direction == CORIENTATION_VERTICAL)
    setFont(font_);
}

void
CDrawAxis2D::
drawAxisTick(const CPoint2D &pos, CDirectionType type, bool large)
{
  int psize = 6;

  if (! large)
    psize = 3;

  if (type == CDIRECTION_TYPE_LEFT || type == CDIRECTION_TYPE_RIGHT) {
    int dx = (type == CDIRECTION_TYPE_LEFT ? -1 : 1);

    double x1 = pos.x + dx*pixelWidthToWindowWidth(psize);

    drawLine(pos, CPoint2D(x1, pos.y));
  }
  else {
    int dy = (type == CDIRECTION_TYPE_DOWN  ? -1 : 1);

    double y1 = pos.y + dy*pixelHeightToWindowHeight(psize);

    drawLine(pos, CPoint2D(pos.x, y1));
  }
}

void
CDrawAxis2D::
drawXString(const CPoint2D &pos, CHAlignType halign, int x_offset,
            CVAlignType valign, int y_offset, const std::string &str)
{
  /* Calculate width and height of string (max width and sum of heights of all lines) */

  CFontPtr font;

  getFont(font);

  if (! font.isValid())
    return;

  int font_size = font->getISize();

  int width1  = 0;
  int height1 = 0;

  std::string str1 = str;

  std::string::size_type pstr1 = str1.find('\n');

  while (pstr1 != std::string::npos) {
    std::string str2 = str1.substr(0, pstr1);

    int width2 = font->getIStringWidth(str2);

    width1   = std::max(width1, width2);
    height1 += font_size;

    str1  = str1.substr(pstr1 + 1);
    pstr1 = str1.find('\n');
  }

  int width2 = font->getIStringWidth(str1);

  width1   = std::max(width1, width2);
  height1 += font_size;

  /*-------*/

  /* Draw each line using X and Y alignment */

  str1 = str;

  pstr1 = str1.find('\n');

  double x1, y1;

  if      (valign == CVALIGN_TYPE_TOP)
    y1 = pos.y - pixelHeightToWindowHeight(height1);
  else if (valign == CVALIGN_TYPE_CENTER)
    y1 = pos.y - pixelHeightToWindowHeight(height1/2);
  else if (valign == CVALIGN_TYPE_BOTTOM)
    y1 = pos.y;

  while (pstr1 != std::string::npos) {
    std::string str2 = str1.substr(0, pstr1);

    int width2 = font->getIStringWidth(str2);

    if      (halign == CHALIGN_TYPE_LEFT)
      x1 = pos.x;
    else if (halign == CHALIGN_TYPE_CENTER)
      x1 = pos.x - pixelWidthToWindowWidth(width2/2);
    else if (halign == CHALIGN_TYPE_RIGHT)
      x1 = pos.x - pixelWidthToWindowWidth(width2);

    CPoint2D w(x1 + pixelWidthToWindowWidth(x_offset), y1 - pixelHeightToWindowHeight(y_offset));

    fillText(w, str2);

    y1 += font_size;

    str1 = str1.substr(pstr1 + 1);
    pstr1 = str1.find('\n');
  }

  width2 = font->getIStringWidth(str1);

  if      (halign == CHALIGN_TYPE_LEFT)
    x1 = pos.x;
  else if (halign == CHALIGN_TYPE_CENTER)
    x1 = pos.x - pixelWidthToWindowWidth(width2/2);
  else if (halign == CHALIGN_TYPE_RIGHT)
    x1 = pos.x - pixelWidthToWindowWidth(width2);

  CPoint2D w(x1 + pixelWidthToWindowWidth(x_offset), y1 - pixelHeightToWindowHeight(y_offset));

  fillText(w, str1);
}

void
CDrawAxis2D::
drawYString(const CPoint2D &pos, CHAlignType halign, int x_offset,
            CVAlignType valign, int y_offset, const std::string &str)
{
  /* Calculate width and height of string (max width and sum of heights of all lines) */

  CFontPtr font;

  getFont(font);

  if (! font.isValid())
    return;

  int font_size = font->getISize();

  int width1  = 0;
  int height1 = 0;

  std::string str1 = str;

  std::string::size_type pstr1 = str1.find('\n');

  while (pstr1 != std::string::npos) {
    std::string str2 = str1.substr(0, pstr1);

    int width2 = font->getIStringWidth(str2);

    width1   = std::max(width1, width2);
    height1 += font_size;

    str1  = str1.substr(pstr1 + 1);
    pstr1 = str1.find('\n');
  }

  int width2 = font->getIStringWidth(str1);

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
    x1 = pos.x - pixelHeightToWindowHeight(height1/2);
  else if (halign == CHALIGN_TYPE_RIGHT)
    x1 = pos.x - pixelHeightToWindowHeight(height1);

  while (pstr1 != std::string::npos) {
    std::string str2 = str1.substr(0, pstr1);

    int width2 = font->getIStringWidth(str2);

    if      (valign == CVALIGN_TYPE_TOP)
      y1 = pos.y;
    else if (valign == CVALIGN_TYPE_CENTER)
      y1 = pos.y - pixelWidthToWindowWidth(width2/2);
    else if (valign == CVALIGN_TYPE_BOTTOM)
      y1 = pos.y - pixelWidthToWindowWidth(width2);

    CPoint2D w(x1 + pixelWidthToWindowWidth(x_offset), y1 - pixelHeightToWindowHeight(y_offset));

    fillText(w, str2);

    x1 += font_size;

    str1  = str1.substr(pstr1 + 1);
    pstr1 = str1.find('\n');
  }

  width2 = font->getIStringWidth(str1);

  if      (valign == CVALIGN_TYPE_TOP)
    y1 = pos.y;
  else if (valign == CVALIGN_TYPE_CENTER)
    y1 = pos.y - pixelWidthToWindowWidth(width2/2);
  else if (valign == CVALIGN_TYPE_BOTTOM)
    y1 = pos.y - pixelWidthToWindowWidth(width2);

  CPoint2D w(x1 + pixelWidthToWindowWidth(x_offset), y1 - pixelHeightToWindowHeight(y_offset));

  fillText(w, str1);
}

void
CDrawAxis2D::
drawGrid(const CAxis2D &x_axis, const CAxis2D &y_axis)
{
  static double grid_dashes[]   = {1, 3};
  static uint   num_grid_dashes = 2;

  /*------*/

  setLineDash(CLineDash(grid_dashes, num_grid_dashes));

  /*------*/

  double xincrement = x_axis.getMajorIncrement();

  for (uint i = 0; i <= x_axis.getNumMajorTicks(); i++) {
    double x = i*xincrement + x_axis.getStart();

    if (x >= x_axis.getStart() && x <= x_axis.getEnd())
      drawLine(CPoint2D(x, y_axis.getStart()), CPoint2D(x, y_axis.getEnd()));
  }

  /*------*/

  double yincrement = y_axis.getMajorIncrement();

  for (uint i = 0; i <= y_axis.getNumMajorTicks(); i++) {
    double y = i*yincrement + y_axis.getStart();

    if (y >= y_axis.getStart() && y <= y_axis.getEnd())
      drawLine(CPoint2D(x_axis.getStart(), y), CPoint2D(x_axis.getEnd(), y));
  }

  /*------*/

  setLineDash(CLineDash());
}
