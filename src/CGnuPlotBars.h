#ifndef CGnuPlotBars_H
#define CGnuPlotBars_H

class CGnuPlotBars {
 public:
  CGnuPlotBars(double s=1, bool f=true) :
   size_(s), front_(f) {
  }

  double size() const { return size_; }
  void setSize(double r) { size_ = r; }

  bool isFront() const { return front_; }
  void setFront(bool b) { front_ = b; }

  void show(std::ostream &os) const {
    if (size_ <= 0.0)
      os << "errors are plotted without bars" << std::endl;
    else
      os << "errorbars are plotted in " << (front_ ? "front" : "back") <<
            " with bars of size " << size_ << std::endl;
  }

  void save(std::ostream &os) const {
    os << "set bar " << size_ << " " << (front_ ? "front" : "back") << std::endl;
  }

 private:
  double size_ { 1 };
  bool   front_ { true };
};

#endif
