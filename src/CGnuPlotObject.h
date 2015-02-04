#ifndef CGnuPlotObject_H
#define CGnuPlotObject_H

#include <CGnuPlotArrowStyle.h>
#include <CGnuPlotColorSpec.h>
#include <CGnuPlotPosition.h>

#include <CRGBA.h>
#include <CBBox2D.h>
#include <CSize2D.h>
#include <CPoint2D.h>
#include <CAlignType.h>
#include <vector>
#include <memory>

class CGnuPlotRenderer;

//---

enum class CGnuPlotLayer {
  FRONT,
  BACK,
  BEHIND
};

//---

class CGnuPlotObject {
 public:
  CGnuPlotObject(CGnuPlot *plot) :
   plot_(plot) {
  }

  virtual ~CGnuPlotObject() { }

 protected:
  CGnuPlot *plot_;
};

//---

class CGnuPlotAnnotation : public CGnuPlotObject {
 public:
  CGnuPlotAnnotation(CGnuPlot *plot) :
   CGnuPlotObject(plot) {
  }

  virtual ~CGnuPlotAnnotation() { }

  const CRGBA &getStrokeColor() const { return strokeColor_; }
  void setStrokeColor(const CRGBA &c) { strokeColor_ = c; }

  const CGnuPlotColorSpec &getFillColor() const { return fillColor_; }
  void setFillColor(const CGnuPlotColorSpec &c) { fillColor_ = c; }

  const CGnuPlotLayer &getLayer() const { return layer_; }
  void setLayer(const CGnuPlotLayer &l) { layer_ = l; }

  virtual void draw(CGnuPlotRenderer *renderer) const = 0;

 protected:
  CRGBA             strokeColor_ { 0, 0, 0 };
  CGnuPlotColorSpec fillColor_;
  CGnuPlotLayer     layer_       { CGnuPlotLayer::FRONT };
};

//---

class CGnuPlotArrow : public CGnuPlotAnnotation {
 public:
  CGnuPlotArrow(CGnuPlot *plot) :
   CGnuPlotAnnotation(plot) {
  }

  virtual ~CGnuPlotArrow() { }

  int getInd() const { return ind_; }
  void setInd(int t) { ind_ = t; }

  const CPoint2D &getFrom() const { return from_; }
  void setFrom(const CPoint2D &p) { from_ = p; }

  const CPoint2D &getTo() const { return to_; }
  void setTo(const CPoint2D &p) { to_ = p; }

  bool getRelative() const { return relative_; }
  void setRelative(bool b) { relative_ = b; }

  const CGnuPlotArrowStyle &style() const { return style_; }
  void setStyle(const CGnuPlotArrowStyle &s) { style_ = s; }

  bool getFHead() const { return style_.fhead(); }
  void setFHead(bool b) { style_.setFHead(b); }

  bool getTHead() const { return style_.thead(); }
  void setTHead(bool b) { style_.setTHead(b); }

  const CGnuPlotCoordValue &getLength() const { return style_.length(); }
  void setLength(const CGnuPlotCoordValue &l) { style_.setLength(l); }

  double getLengthValue() const { return style_.lengthValue(); }
  void setLengthValue(double l) { style_.setLengthValue(l); }

  double getAngle() const { return style_.angle(); }
  void setAngle(double a) { style_.setAngle(a); }

  double getBackAngle() const { return style_.backAngle(); }
  void setBackAngle(double a) { style_.setBackAngle(a); }

  bool getFilled() const { return style_.filled(); }
  void setFilled(bool b) { style_.setFilled(b); }

  bool getEmpty() const { return style_.empty(); }
  void setEmpty(bool b) { style_.setEmpty(b); }

  bool getFront() const { return style_.front(); }
  void setFront(bool b) { style_.setFront(b); }

  double getLineWidth() const { return style_.lineWidth(plot_); }
  void setLineWidth(double w) { style_.setLineWidth(w); }

  int getLineStyle() const { return style_.lineStyle(); }
  void setLineStyle(int t) { style_.setLineStyle(t); }

  bool getVariable() const { return style_.variable(); }
  void setVariable(bool b) { style_.setVariable(b); }

  const COptReal &getVarValue() const { return style_.varValue(); }
  void setVarValue(double r) { style_.setVarValue(r); }

  CRGBA getLineColor() const;

  void draw(CGnuPlotRenderer *renderer) const override;

  void print(std::ostream &os) const {
    style_.print(plot_, os);

    os << " from " << from_ << (relative_ ? " rto " : " to ") << to_;
  }

 protected:
  int                ind_      { -1 };
  CPoint2D           from_     { 0, 0 };
  CPoint2D           to_       { 1, 1 };
  bool               relative_ { false };
  CGnuPlotArrowStyle style_;
};

//---

class CGnuPlotLabel : public CGnuPlotAnnotation {
 public:
  CGnuPlotLabel(CGnuPlot *plot) :
   CGnuPlotAnnotation(plot) {
  }

  virtual ~CGnuPlotLabel() { }

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

  void draw(CGnuPlotRenderer *renderer) const override;

  void print(std::ostream &os) const {
    os << " \"" << text_ << "\"" << " at " << pos_;
  }

 protected:
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

class CGnuPlotEllipse : public CGnuPlotAnnotation {
 public:
  CGnuPlotEllipse(CGnuPlot *plot) :
   CGnuPlotAnnotation(plot) {
  }

  virtual ~CGnuPlotEllipse() { }

  const CPoint2D &getCenter() const { return p_; }
  void setCenter(const CPoint2D &p) { p_ = p; }

  double getRX() const { return rx_; }
  void setRX(double x) { rx_ = x; }

  double getRY() const { return ry_; }
  void setRY(double y) { ry_ = y; }

  void draw(CGnuPlotRenderer *renderer) const override;

 protected:
  CPoint2D p_  { 0, 0 };
  double   rx_ { 1 };
  double   ry_ { 1 };
};

//---

class CGnuPlotPolygon : public CGnuPlotAnnotation {
 public:
  typedef std::vector<CPoint2D> Points;

 public:
  CGnuPlotPolygon(CGnuPlot *plot) :
   CGnuPlotAnnotation(plot) {
  }

  virtual ~CGnuPlotPolygon() { }

  void addPoint(const CPoint2D &p) {
    points_.push_back(p);
  }

  const Points &getPoints() const { return points_; }

  void draw(CGnuPlotRenderer *renderer) const override;

 protected:
  Points points_;
};

//---

class CGnuPlotRectangle : public CGnuPlotAnnotation {
 public:
  CGnuPlotRectangle(CGnuPlot *plot) :
   CGnuPlotAnnotation(plot) {
  }

  virtual ~CGnuPlotRectangle() { }

  const CGnuPlotPosition &getFrom() const { return from_.getValue(); }
  void setFrom(const CGnuPlotPosition &p) { from_ = p; }

  const CGnuPlotPosition &getTo() const { return to_.getValue(); }
  void setTo(const CGnuPlotPosition &p) { to_ = p; }

  const CGnuPlotPosition &getRTo() const { return rto_.getValue(); }
  void setRTo(const CGnuPlotPosition &p) { rto_ = p; }

  const CGnuPlotPosition &getCenter() const { return center_.getValue(); }
  void setCenter(const CGnuPlotPosition &p) { center_ = p; }

  const CSize2D &getSize() const { return size_.getValue(); }
  void setSize(const CSize2D &p) { size_ = p; }

  int getFillStyle() const { return fs_.getValue(-1); }
  void setFillStyle(int fs) { fs_ = fs; }

  double getLineWidth() const { return lw_.getValue(0); }
  void setLineWidth(double w) { lw_ = w; }

  void draw(CGnuPlotRenderer *renderer) const override;

 protected:
  COptValT<CGnuPlotPosition> from_;
  COptValT<CGnuPlotPosition> to_;
  COptValT<CGnuPlotPosition> rto_;
  COptValT<CGnuPlotPosition> center_;
  COptValT<CSize2D>          size_;
  COptValT<int>              fs_;
  COptValT<double >          lw_;
  mutable CBBox2D            bbox_;
};

//---

typedef std::shared_ptr<CGnuPlotArrow>     CGnuPlotArrowP;
typedef std::shared_ptr<CGnuPlotLabel>     CGnuPlotLabelP;
typedef std::shared_ptr<CGnuPlotEllipse>   CGnuPlotEllipseP;
typedef std::shared_ptr<CGnuPlotPolygon>   CGnuPlotPolygonP;
typedef std::shared_ptr<CGnuPlotRectangle> CGnuPlotRectangleP;

#endif
