#ifndef CGnuPlotRectStyle_H
#define CGnuPlotRectStyle_H

class CGnuPlotRectStyle {
 public:
  CGnuPlotRectStyle() {
    unset();
  }

  bool isFront() const { return front_; }
  void setFront(bool b) { front_ = b; }

  double lineWidth() const { return lineWidth_; }
  void setLineWidth(double r) { lineWidth_ = r; }

  const CGnuPlotColorSpec &fillColor() const { return fc_; }
  void setFillColor(const CGnuPlotColorSpec &v) { fc_ = v; }

  const CGnuPlotFillStyle &fillStyle() const { return fs_; }
  void setFillStyle(const CGnuPlotFillStyle &v) { fs_ = v; }

  void unset() {
    front_     = true;
    lineWidth_ = 1;
    fc_        = CGnuPlotColorSpec(); fc_.setBackground();
    fs_        = CGnuPlotFillStyle(); fs_.setStyle(CGnuPlotTypes::FillType::SOLID);
  }

  void show(std::ostream &os) const {
    os << "Rectangle style is ";
    os << (front_ ? "front" : "back");
    os << ", fill color " << fc_;
    os << ", lw " << lineWidth_;
    os << ", fillstyle " << fs_;
    os << std::endl;
  }

 private:
  bool              front_ { true };
  double            lineWidth_ { 1 };
  CGnuPlotColorSpec fc_;
  CGnuPlotFillStyle fs_;
};

#endif
