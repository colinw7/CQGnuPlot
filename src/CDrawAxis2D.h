#ifndef CDRAW_AXIS_2D_H
#define CDRAW_AXIS_2D_H

#include <COrientation.h>
#include <CAlignType.h>
#include <CDirectionType.h>
#include <CLineDash.h>
#include <CPoint2D.h>
#include <CAxis2D.h>
#include <CFont.h>

class CDrawAxis2D {
 public:
  CDrawAxis2D();

  virtual ~CDrawAxis2D() { }

  void setPixelSize(double size);

  void drawAxesAndGrid(double xmin, double ymin, double xmax, double ymax, double xc, double yc);

  void drawAxis(const CAxis2D &axis, double pos, COrientation direction);

  void drawGrid(const CAxis2D &x_axis, const CAxis2D &y_axis);

 protected:
  virtual void getFont(CFontPtr &font) = 0;
  virtual void setFont(CFontPtr  font) = 0;

  virtual double pixelWidthToWindowWidth  (double p) = 0;
  virtual double pixelHeightToWindowHeight(double p) = 0;

  virtual void setLineDash(const CLineDash &dash) = 0;

  virtual void drawLine(const CPoint2D &p1, const CPoint2D &p2) = 0;

  virtual void fillText(const CPoint2D &p, const std::string &str) = 0;

 protected:
  void drawAxisTick(const CPoint2D &p, CDirectionType type, bool large);

  void drawXString(const CPoint2D &p, CHAlignType halign, int x_offset,
                   CVAlignType valign, int y_offset, const std::string &str);
  void drawYString(const CPoint2D &p, CHAlignType halign, int x_offset,
                   CVAlignType valign, int y_offset, const std::string &str);

 private:
  CFontPtr font_;
  double   pixel_size_;
};

#endif
