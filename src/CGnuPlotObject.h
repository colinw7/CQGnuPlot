#ifndef CGnuPlotObject_H
#define CGnuPlotObject_H

#include <CRefPtr.h>
#include <CRGBA.h>
#include <CPoint2D.h>
#include <CAlignType.h>

class CGnuPlotObject {
 public:
  CGnuPlotObject() { }

  const CRGBA &getStrokeColor() const { return strokeColor_; }
  void setStrokeColor(const CRGBA &c) { strokeColor_ = c; }

  const CRGBA &getFillColor() const { return fillColor_; }
  void setFillColor(const CRGBA &c) { fillColor_ = c; }

 private:
  CRGBA strokeColor_ { 0, 0, 0 };
  CRGBA fillColor_   { 0, 0, 0, 0 };
};

//---

class CGnuPlotArrow : public CGnuPlotObject {
 public:
  struct ArrowStyle {
    bool     fhead     { false };
    bool     thead     { true };
    double   length    { -1 };
    double   angle     { -1 };
    double   backAngle { -1 };
    bool     filled    { false };
    bool     empty     { false };
    bool     front     { false };
    int      lineType  { -1 };
    double   lineWidth { -1 };
    int      lineStyle { -1 };
  };

 public:
  CGnuPlotArrow() { }

  int getInd() const { return ind_; }
  void setInd(int t) { ind_ = t; }

  const CPoint2D &getFrom() const { return from_; }
  void setFrom(const CPoint2D &p) { from_ = p; }

  const CPoint2D &getTo() const { return to_; }
  void setTo(const CPoint2D &p) { to_ = p; }

  bool getRelative() const { return relative_; }
  void setRelative(bool b) { relative_ = b; }

  bool getFHead() const { return style_.fhead; }
  void setFHead(bool b) { style_.fhead = b; }

  bool getTHead() const { return style_.thead; }
  void setTHead(bool b) { style_.thead = b; }

  double getLength() const { return style_.length; }
  void setLength(double l) { style_.length = l; }

  double getAngle() const { return style_.angle; }
  void setAngle(double a) { style_.angle = a; }

  double getBackAngle() const { return style_.backAngle; }
  void setBackAngle(double a) { style_.backAngle = a; }

  bool getFilled() const { return style_.filled; }
  void setFilled(bool b) { style_.filled = b; }

  bool getEmpty() const { return style_.empty; }
  void setEmpty(bool b) { style_.empty = b; }

  bool getFront() const { return style_.front; }
  void setFront(bool b) { style_.front = b; }

  int getLineType() const { return style_.lineType; }
  void setLineType(int t) { style_.lineType = t; }

  double getLineWidth() const { return style_.lineWidth; }
  void setLineWidth(double w) { style_.lineWidth = w; }

  int getLineStyle() const { return style_.lineStyle; }
  void setLineStyle(int t) { style_.lineStyle = t; }

 private:
  int        ind_      { -1 };
  CPoint2D   from_     { 0, 0 };
  CPoint2D   to_       { 1, 1 };
  bool       relative_ { false };
  ArrowStyle style_;
};

//---

class CGnuPlotLabel : public CGnuPlotObject {
 public:
  CGnuPlotLabel() { }

  int getInd() const { return ind_; }
  void setInd(int t) { ind_ = t; }

  const std::string &getText() const { return text_; }
  void setText(const std::string &t) { text_ = t; }

  const CHAlignType &getAlign() const { return align_; }
  void setAlign(const CHAlignType &a) { align_ = a; }

  const CPoint2D &getPos() const { return pos_; }
  void setPos(const CPoint2D &p) { pos_ = p; }

  const std::string &getFont() const { return font_; }
  void setFont(const std::string &f) { font_ = f; }

  double getAngle() const { return angle_; }
  void setAngle(double a) { angle_ = a; }

  bool getFront() const { return front_; }
  void setFront(bool b) { front_ = b; }

  double getOffset() const { return offset_; }
  void setOffset(double o) { offset_ = o; }

 private:
  int         ind_    { -1 };
  std::string text_;
  CHAlignType align_  { CHALIGN_TYPE_LEFT };
  CPoint2D    pos_    { 0, 0 };
  std::string font_;
  double      angle_  { -1 };
  bool        front_  { false };
  double      offset_ { 0.0 };
};

//---

class CGnuPlotEllipse : public CGnuPlotObject {
 public:
  CGnuPlotEllipse() { }

  const CPoint2D &getCenter() const { return p_; }
  void setCenter(const CPoint2D &p) { p_ = p; }

  double getRX() const { return rx_; }
  void setRX(double x) { rx_ = x; }

  double getRY() const { return ry_; }
  void setRY(double y) { ry_ = y; }

 private:
  CPoint2D p_  { 0, 0 };
  double   rx_ { 1 };
  double   ry_ { 1 };
};

//---

class CGnuPlotPolygon : public CGnuPlotObject {
 public:
  typedef std::vector<CPoint2D> Points;

 public:
  CGnuPlotPolygon() { }

  void addPoint(const CPoint2D &p) {
    points_.push_back(p);
  }

  const Points &getPoints() const { return points_; }

 private:
  Points points_;
};

//---

class CGnuPlotRectangle : public CGnuPlotObject {
 public:
  CGnuPlotRectangle() { }

  const CPoint2D &getFrom() const { return from_; }
  void setFrom(const CPoint2D &p) { from_ = p; }

  const CPoint2D &getTo() const { return to_; }
  void setTo(const CPoint2D &p) { to_ = p; }

  int getFillStyle() const { return fs_; }
  void setFillStyle(int fs) { fs_ = fs; }

  double getLineWidth() const { return lw_; }
  void setLineWidth(double w) { lw_ = w; }

 private:
  CPoint2D from_ { 0, 0 };
  CPoint2D to_   { 1, 1 };
  int      fs_   { -1 };
  double   lw_   { 1 };
};

//---

typedef CRefPtr<CGnuPlotArrow>     CGnuPlotArrowP;
typedef CRefPtr<CGnuPlotLabel>     CGnuPlotLabelP;
typedef CRefPtr<CGnuPlotEllipse>   CGnuPlotEllipseP;
typedef CRefPtr<CGnuPlotPolygon>   CGnuPlotPolygonP;
typedef CRefPtr<CGnuPlotRectangle> CGnuPlotRectangleP;

#endif
