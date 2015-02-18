#ifndef CGnuPlotTitle_H
#define CGnuPlotTitle_H

#include <CGnuPlotColorSpec.h>
#include <CPoint2D.h>
#include <CFont.h>

class CGnuPlotGroup;

class CGnuPlotTitle {
 public:
  CGnuPlotTitle(CGnuPlotGroup *group=0);

  virtual ~CGnuPlotTitle() { }

  CGnuPlotGroup *group() const { return group_; }
  void setGroup(CGnuPlotGroup *group) { group_ = group; }

  void setText(const std::string &str) { text_ = str; }
  const std::string &text() const { return text_; }

  const CPoint2D &offset() const { return offset_; }
  void setOffset(const CPoint2D &o) { offset_ = o; }

  void setFont(const CFontPtr &str) { font_ = str; }
  const CFontPtr &font() const { return font_; }

  const CGnuPlotColorSpec &color() const { return color_; }
  void setColor(const CGnuPlotColorSpec &c) { color_ = c; }

  void setEnhanced(bool b) { enhanced_ = b; }
  bool enhanced() const { return enhanced_; }

  virtual void draw() const;

 private:
  CGnuPlotGroup*    group_ { 0 };
  std::string       text_;
  CPoint2D          offset_ { 0, 0 };
  CFontPtr          font_;
  CGnuPlotColorSpec color_;
  bool              enhanced_ { false };
};

#endif
