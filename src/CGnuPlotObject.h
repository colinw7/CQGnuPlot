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
  CGnuPlotArrow() { }

  int getInd() const { return ind_; }
  void setInd(int t) { ind_ = t; }

  const CPoint2D &getFrom() const { return from_; }
  void setFrom(const CPoint2D &p) { from_ = p; }

  const CPoint2D &getTo() const { return to_; }
  void setTo(const CPoint2D &p) { to_ = p; }

  bool getRelative() const { return relative_; }
  void setRelative(bool b) { relative_ = b; }

  double getLength() const { return length_; }
  void setLength(double l) { length_ = l; }

  double getAngle() const { return angle_; }
  void setAngle(double a) { angle_ = a; }

  double getBackAngle() const { return backAngle_; }
  void setBackAngle(double a) { backAngle_ = a; }

  bool getFHead() const { return fhead_; }
  void setFHead(bool b) { fhead_ = b; }

  bool getTHead() const { return thead_; }
  void setTHead(bool b) { thead_ = b; }

  bool getFilled() const { return filled_; }
  void setFilled(bool b) { filled_ = b; }

  bool getEmpty() const { return empty_; }
  void setEmpty(bool b) { empty_ = b; }

  bool getFront() const { return front_; }
  void setFront(bool b) { front_ = b; }

  int getLineType() const { return lineType_; }
  void setLineType(int t) { lineType_ = t; }

  double getLineWidth() const { return lineWidth_; }
  void setLineWidth(double w) { lineWidth_ = w; }

 private:
  int      ind_       { -1 };
  CPoint2D from_      { 0, 0 };
  CPoint2D to_        { 1, 1 };
  bool     relative_  { false };
  double   length_    { -1 };
  double   angle_     { -1 };
  double   backAngle_ { -1 };
  bool     fhead_     { false };
  bool     thead_     { true };
  bool     filled_    { false };
  bool     empty_     { false };
  bool     front_     { false };
  int      lineType_  { -1 };
  double   lineWidth_ { -1 };
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

 private:
  CPoint2D from_ { 0, 0 };
  CPoint2D to_   { 1, 1 };
};

//---

typedef CRefPtr<CGnuPlotArrow>     CGnuPlotArrowP;
typedef CRefPtr<CGnuPlotLabel>     CGnuPlotLabelP;
typedef CRefPtr<CGnuPlotEllipse>   CGnuPlotEllipseP;
typedef CRefPtr<CGnuPlotPolygon>   CGnuPlotPolygonP;
typedef CRefPtr<CGnuPlotRectangle> CGnuPlotRectangleP;

#endif
