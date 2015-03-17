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

  const std::string &text() const { return text_; }
  void setText(const std::string &str) { text_ = str; }

  const CPoint2D &offset() const { return offset_; }
  void setOffset(const CPoint2D &o) { offset_ = o; }

  const CFontPtr &font() const { return font_; }
  void setFont(const CFontPtr &str) { font_ = str; }

  const CGnuPlotColorSpec &color() const { return color_; }
  void setColor(const CGnuPlotColorSpec &c) { color_ = c; }

  bool isEnhanced() const { return enhanced_; }
  void setEnhanced(bool b) { enhanced_ = b; }

  virtual void draw() const;

  void show(std::ostream &os) const {
    os << "set title \"" << text_ << "\"" << std::endl;

    if (font_.isValid())
      os << "set title font \"\" norotate" << std::endl;
    else
      os << "set title font \" norotate" << font_->getFamily() << "\"" << std::endl;
  }

  void print(std::ostream &os) const {
    os << "title is \"" << text_ << "\", " <<
          "offset at ((character units) 0, 0, 0)" << std::endl;
  }

 private:
  CGnuPlotGroup*    group_ { 0 };
  std::string       text_;
  CPoint2D          offset_ { 0, 0 };
  CFontPtr          font_;
  CGnuPlotColorSpec color_;
  bool              enhanced_ { false };
};

#endif
