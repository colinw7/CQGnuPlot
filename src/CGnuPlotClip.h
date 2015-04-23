#ifndef CGnuPlotClip_H
#define CGnuPlotClip_H

class CGnuPlotClip {
 public:
  CGnuPlotClip() { }

  bool isPoints() const { return points_; }
  void setPoints(bool b) { points_ = b; }

  bool isOne() const { return one_; }
  void setOne(bool b) { one_ = b; }

  bool isTwo() const { return two_; }
  void setTwo(bool b) { two_ = b; }

  bool isOn() const { return (points_ || one_ || two_); }

  const CBBox2D &rect() const { return rect_; }
  void setRect(const CBBox2D &r) { rect_ = r; }

  void reset() {
    points_ = true;
    one_    = false;
    two_    = false;
  }

  void unset() {
    points_ = false;
    one_    = false;
    two_    = false;
  }

  void show(std::ostream &os) {
    os << "point clip is " << (isOn() ? "ON" : "OFF") << std::endl;

    if (points_)
      os << "drawing and clipping lines between inrange and outrange points" << std::endl;
    else
      os << "not drawing lines between inrange and outrange points" << std::endl;

    if (one_ || two_)
      os << "drawing and clipping lines between two outrange points" << std::endl;
    else
      os << "not drawing lines between two outrange points" << std::endl;
  }

 private:
  bool    points_ { false };
  bool    one_    { true  };
  bool    two_    { false };
  CBBox2D rect_;
};

#endif
