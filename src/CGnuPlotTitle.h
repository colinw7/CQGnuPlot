#ifndef CGnuPlotTitle_H
#define CGnuPlotTitle_H

#include <CGnuPlotColorSpec.h>
#include <CGnuPlotPosition.h>
#include <CFont.h>

class CGnuPlotGroup;
class CGnuPlotRenderer;

class CGnuPlotTitleData {
 public:
  CGnuPlotTitleData() { }

  const std::string &text() const { return text_; }
  void setText(const std::string &str) { text_ = str; }

  const CGnuPlotPosition &offset() const { return offset_; }
  void setOffset(const CGnuPlotPosition &o) { offset_ = o; }

  const CFontPtr &font() const { return font_; }
  void setFont(const CFontPtr &str) { font_ = str; }

  const CGnuPlotColorSpec &color() const { return color_; }
  void setColor(const CGnuPlotColorSpec &c) { color_ = c; }

  bool isEnhanced() const { return enhanced_; }
  void setEnhanced(bool b) { enhanced_ = b; }

  void unset() {
    setText("");
  }

  void save(std::ostream &os) const {
    os << "set title \"" << text() << "\"" << std::endl;

    if (font().isValid())
      os << "set title font \"\" norotate" << std::endl;
    else
      os << "set title font \" norotate" << font()->getFamily() << "\"" << std::endl;
  }

  void show(std::ostream &os) const {
    os << "title is \"" << text() << "\"";
    os << ", " << "offset at (" << offset() << ")";

    if (font().isValid())
      os << ", using font \"" << font() << "\"";

    if (color().isValid())
      os << " textcolor " << color();

    os << std::endl;
  }

 private:
  std::string       text_;
  CGnuPlotPosition  offset_;
  CFontPtr          font_;
  CGnuPlotColorSpec color_;
  bool              enhanced_ { true };
};

class CGnuPlotTitle {
 public:
  typedef std::pair<CHAlignType,double> HAlignPos;
  typedef std::pair<CVAlignType,double> VAlignPos;

 public:
  CGnuPlotTitle(CGnuPlotGroup *group=0);

  virtual ~CGnuPlotTitle() { }

  CGnuPlotGroup *group() const { return group_; }
  void setGroup(CGnuPlotGroup *group) { group_ = group; }

  const CGnuPlotTitleData &data() const { return data_; }
  void setData(const CGnuPlotTitleData &d) { data_ = d; }

  const std::string &text() const { return data_.text(); }
  void setText(const std::string &str) { data_.setText(str); }

  const CGnuPlotPosition &offset() const { return data_.offset(); }
  void setOffset(const CGnuPlotPosition &o) { data_.setOffset(o); }

  const CFontPtr &font() const { return data_.font(); }
  void setFont(const CFontPtr &f) { data_.setFont(f); }

  const CGnuPlotColorSpec &color() const { return data_.color(); }
  void setColor(const CGnuPlotColorSpec &c) { data_.setColor(c); }

  bool isEnhanced() const { return data_.isEnhanced(); }
  void setEnhanced(bool b) { data_.setEnhanced(b); }

  bool isDisplayed() const { return displayed_; }
  void setDisplayed(bool b) { displayed_ = b; }

  const CBBox2D &bbox() const { return bbox_; }
  void setBBox(const CBBox2D &v) { bbox_ = v; }

  const CPoint2D &lastOffset() const { return lastOffset_; }
  void setLastOffset(const CPoint2D &v) { lastOffset_ = v; }

  virtual void draw(CGnuPlotRenderer *renderer) const;

 private:
  CGnuPlotGroup*    group_ { 0 };
  CGnuPlotTitleData data_;
  bool              displayed_ { true };
  mutable CBBox2D   bbox_;
  mutable CPoint2D  lastOffset_;
};

typedef std::shared_ptr<CGnuPlotTitle> CGnuPlotTitleP;

#endif
