#ifndef CGnuPlotColorBox_H
#define CGnuPlotColorBox_H

#include <CPoint2D.h>
#include <CSize2D.h>
#include <CBBox2D.h>
#include <memory>

class CGnuPlot;
class CGnuPlotGroup;

class CGnuPlotColorBox {
 public:
  CGnuPlotColorBox(CGnuPlotGroup *group=0);

  virtual ~CGnuPlotColorBox();

  CGnuPlot *app() const;

  CGnuPlotGroup *group() const { return group_; }

  void init(const CGnuPlotColorBox &box) {
    CGnuPlotGroup *group;

    std::swap(group, group_);

    *this = box;

    std::swap(group, group_);
  }

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

  const CPoint2D &origin() const { return origin_; }
  void setOrigin(const CPoint2D &p) { origin_ = p; }

  const CSize2D &size() const { return size_; }
  void setSize(const CSize2D &s) { size_ = s; }

  const CBBox2D &getBBox() const { return bbox_; }

  virtual void draw();

 private:
  CGnuPlotGroup*  group_       { 0 };
  bool            enabled_     { false };
  bool            vertical_    { true };
  bool            user_        { false };
  bool            front_       { true };
  bool            border_      { true };
  int             borderStyle_ { -1 };
  CPoint2D        origin_      { 0, 0 };
  CSize2D         size_        { 20, 100 };
  mutable CBBox2D bbox_;
};

typedef std::unique_ptr<CGnuPlotColorBox> CGnuPlotColorBoxP;

#endif
