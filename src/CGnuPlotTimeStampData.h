#ifndef CGnuPlotTimeStampData_H
#define CGnuPlotTimeStampData_H

class CGnuPlotTimeStampData {
 public:
  CGnuPlotTimeStampData() { }

  const std::string &format() const { return format_; }
  void setFormat(const std::string &v) { format_ = v; }

  bool isTop() const { return top_; }
  void setTop(bool b) { top_ = b; }

  bool isRotated() const { return rotated_; }
  void setRotated(bool b) { rotated_ = b; }

  const CPoint2D &offset() const { return offset_; }
  void setOffset(const CPoint2D &v) { offset_ = v; }

  const CFontPtr &font() const { return font_; }
  void setFont(const CFontPtr &v) { font_ = v; }

  const CGnuPlotColorSpec &textColor() const { return textColor_; }
  void setTextColor(const CGnuPlotColorSpec &v) { textColor_ = v; }

  void unset() {
    format_    = "";
    top_       = false;
    rotated_   = false;
    offset_    = CPoint2D(0, 0);
    font_      = CFontPtr();
    textColor_ = CGnuPlotColorSpec();
  }

  void show(std::ostream &os) {
    os << "time is \"" << format_ << "\", offset at (" << offset_ << ")";

    if (font_.isValid())
      os << ", using font " << font_;

    os << std::endl;

    os << "written in " << (top_ ? "top" : "bottom") << " corner" << std::endl;
    os << (rotated_ ? "rotated" : "not rotated") << std::endl;
  }

 private:
  std::string       format_;
  bool              top_     { false };
  bool              rotated_ { false };
  CPoint2D          offset_  { 0, 0 };
  CFontPtr          font_;
  CGnuPlotColorSpec textColor_;
};

#endif
