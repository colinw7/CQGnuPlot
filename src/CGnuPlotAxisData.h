#ifndef CGnuPlotAxisData_H
#define CGnuPlotAxisData_H

class CGnuPlotAxisData {
 public:
  CGnuPlotAxisData(int i=1) :
   ind_(i) {
  }

  int ind() const { return ind_; }

  bool isDisplayed() const { return displayed_; }
  void setDisplayed(bool b) { displayed_ = b; }

  bool hasGrid() const { return grid_; }
  void setGrid(bool b) { grid_ = b; }

  bool isMirror() const { return mirror_; }
  void setMirror(bool b) { mirror_ = b; }

  bool isReverse() const { return reverse_; }
  void setReverse(bool b) { reverse_ = b; }

  bool isWriteBack() const { return writeback_; }
  void setWriteBack(bool b) { writeback_ = b; }

  bool isExtend() const { return extend_; }
  void setExtend(bool b) { extend_ = b; }

  bool isTime() const { return isTime_; }
  void setIsTime(bool b) { isTime_ = b; }

  const COptReal &min() const { return min_; }
  void setMin(double r) { min_ = r; }
  void setMin(const COptReal &r) { min_ = r; }
  void updateMin(double r) { min_.updateMin(r); }
  void resetMin() { min_.setInvalid(); }

  const COptReal &max() const { return max_; }
  void setMax(double r) { max_ = r; }
  void setMax(const COptReal &r) { max_ = r; }
  void updateMax(double r) { max_.updateMax(r); }
  void resetMax() { max_.setInvalid(); }

  const std::string &text() const { return str_; }
  void setText(const std::string &s) { str_ = s; }

  bool hasTiclLabel(int i) const { return (ticlabel_.find(i) != ticlabel_.end()); }

  const std::string &ticlLabel(int i) const {
    auto p = ticlabel_.find(i);

    return (*p).second;
  }

  void setTicLabel(int i, const std::string &s) {
    ticlabel_[i] = s;
  }

  const std::string &format() const { return format_; }
  void setFormat(const std::string &s) { format_ = s; }

  const CFontPtr &font() const { return font_; }
  void setFont(const CFontPtr &f) { font_ = f; }

  bool showTics() const { return showTics_; }
  void setShowTics(bool b) { showTics_ = b; }

 private:
  typedef std::map<int,std::string> TicLabelMap;

  int         ind_;
  bool        displayed_ { true  };
  bool        grid_      { false };
  bool        mirror_    { true  };
  bool        reverse_   { false };
  bool        writeback_ { false };
  bool        extend_    { false };
  bool        isTime_    { false };
  COptReal    min_;
  COptReal    max_;
  std::string str_;
  TicLabelMap ticlabel_;
  double      offset_    { 0 };
  std::string format_;
  CFontPtr    font_;
  bool        showTics_  { true };
};

#endif
