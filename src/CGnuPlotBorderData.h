#ifndef CGnuPlotBorderData_H
#define CGnuPlotBorderData_H

#include <CGnuPlotTypes.h>

class CGnuPlotBorderData {
 public:
  typedef CGnuPlotTypes::DrawLayer DrawLayer;

 public:
  CGnuPlotBorderData() { }

  const std::optional<int> &sides() const { return sides_; }
  void setSides(int i) { sides_ = i; }
  void resetSides() { sides_.reset(); }

  const DrawLayer &layer() const { return layer_; }
  void setLayer(const DrawLayer &v) { layer_ = v; }

  double lineWidth() const { return lineWidth_; }
  void setLineWidth(double r) { lineWidth_ = r; }

  const std::optional<int> &lineStyle() const { return lineStyle_; }
  void setLineStyle(int s) { lineStyle_ = s; }

  const std::optional<int> &lineType() const { return lineType_; }
  void setLineType(int t) { lineType_ = t; }

  void unset() {
    resetSides();
  }

  void show(std::ostream &os) const;

  void save(std::ostream &os) const;

 private:
  std::optional<int> sides_;
  DrawLayer          layer_     { DrawLayer::FRONT };
  double             lineWidth_ { 1.0 };
  std::optional<int> lineStyle_;
  std::optional<int> lineType_;
};

#endif
