#ifndef CGnuPlotColorBox_H
#define CGnuPlotColorBox_H

#include <CGnuPlotPosition.h>
#include <CGnuPlotAxisData.h>
#include <CSize2D.h>
#include <CBBox2D.h>
#include <memory>

class CGnuPlot;
class CGnuPlotGroup;

class CGnuPlotColorBoxData {
 public:
  CGnuPlotColorBoxData();

  bool isEnabled() const { return enabled_; }
  void setEnabled (bool b) { enabled_ = b; }

  bool isVertical() const { return vertical_; }
  void setVertical(bool b) { vertical_ = b; }

  bool isUser() const { return user_; }
  void setUser(bool b) { user_ = b; }

  bool isFront() const { return front_; }
  void setFront(bool b) { front_ = b; }

  bool hasBorder() const { return border_; }
  void setBorder(bool b) { border_ = b; }

  int borderStyle() const { return borderStyle_; }
  void setBorderStyle(int i) { borderStyle_ = i; }

  const CGnuPlotPosition &origin() const { return origin_; }
  void setOrigin(const CGnuPlotPosition &p) { origin_ = p; }

  const CSize2D &size() const { return size_; }
  void setSize(const CSize2D &s) { size_ = s; }

  const CGnuPlotAxisData &axis() const { return axis_; }
  CGnuPlotAxisData &axis() { return axis_; }

  const COptReal &min() const { return axis_.min(); }
  void setMin(double r) { axis_.setMin(r); }

  const COptReal &max() const { return axis_.max(); }
  void setMax(double r) { axis_.setMax(r); }

  void show(std::ostream &os) const;

  void save(std::ostream &os) const;

 private:
  bool             enabled_     { false };
  bool             vertical_    { true };
  bool             user_        { false };
  bool             front_       { true };
  bool             border_      { true };
  int              borderStyle_ { -1 };
  CGnuPlotPosition origin_;
  CSize2D          size_        { 0.05, 0.6 };
  CGnuPlotAxisData axis_;
};

//------

class CGnuPlotColorBox {
 public:
  CGnuPlotColorBox(CGnuPlotGroup *group=0);

  virtual ~CGnuPlotColorBox();

  CGnuPlot *app() const;

  CGnuPlotGroup *group() const { return group_; }

  void init(const CGnuPlotColorBoxData &data) {
    data_ = data;
  }

  bool isEnabled() const { return data_.isEnabled(); }
  void setEnabled (bool b) { data_.setEnabled(b); }

  bool isVertical() const { return data_.isVertical(); }
  void setVertical(bool b) { data_.setVertical(b); }

  bool isUser() const { return data_.isUser(); }
  void setUser(bool b) { data_.setUser(b); }

  bool isFront() const { return data_.isFront(); }
  void setFront(bool b) { data_.setFront(b); }

  bool hasBorder() const { return data_.hasBorder(); }
  void setBorder(bool b) { data_.setBorder(b); }

  int borderStyle() const { return data_.borderStyle(); }
  void setBorderStyle(int i) { data_.setBorderStyle(i); }

  const CGnuPlotPosition &origin() const { return data_.origin(); }
  void setOrigin(const CGnuPlotPosition &p) { data_.setOrigin(p); }

  const CSize2D &size() const { return data_.size(); }
  void setSize(const CSize2D &s) { data_.setSize(s); }

  const CGnuPlotAxisData &axis() const { return data_.axis(); }
  CGnuPlotAxisData &axis() { return data_.axis(); }

  const COptReal &min() const { return data_.min(); }
  void setMin(double r) { data_.setMin(r); }

  const COptReal &max() const { return data_.max(); }
  void setMax(double r) { data_.setMax(r); }

  const CBBox2D &getBBox() const { return bbox_; }

  CColor valueToColor(double x) const;

  virtual void draw(CGnuPlotRenderer *renderer);

 private:
  CGnuPlotGroup*       group_       { 0 };
  CGnuPlotColorBoxData data_;
  mutable CBBox2D      bbox_;
};

typedef std::unique_ptr<CGnuPlotColorBox> CGnuPlotColorBoxP;

#endif
