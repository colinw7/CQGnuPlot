#ifndef CGnuPlotProbeEvent_H
#define CGnuPlotProbeEvent_H

class CGnuPlotProbeEvent {
 public:
  class Line {
   public:
    Line(double x, double y1, double y2, double y3, const std::string &tip) :
     x_(x), y1_(y1), y2_(y2), y3_(y3), tip_(tip) {
    }

    double x() const { return x_; }
    void setX(double r) { x_ = r; }

    double y1() const { return y1_; }
    void setY1(double r) { y1_ = r; }

    double y2() const { return y2_; }
    void setY2(double r) { y2_ = r; }

    double y3() const { return y3_; }
    void setY3(double r) { y3_ = r; }

    const std::string &tip() const { return tip_; }
    void setTip(const std::string &v) { tip_ = v; }

   private:
    double      x_;
    double      y1_;
    double      y2_;
    double      y3_;
    std::string tip_;
  };

  typedef std::vector<Line> Lines;

 public:
  CGnuPlotProbeEvent() { }

  double x() const { return x_; }
  void setX(double r) { x_ = r; }

  bool isNearest() const { return nearest_; }
  void setNearest(bool b) { nearest_ = b; }

  void addLine(const Line &line) { lines_.push_back(line); }

  const Lines &lines() const { return lines_; }
  void setLines(const Lines &v) { lines_ = v; }

 private:
  double      x_       { 0 };
  bool        nearest_ { true };
  Lines       lines_;
};

#endif
