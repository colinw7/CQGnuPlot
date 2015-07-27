#ifndef CGnuPlotGridData_H
#define CGnuPlotGridData_H

class CGnuPlotGridData {
 public:
  typedef CGnuPlotTypes::DrawLayer DrawLayer;

  CGnuPlotGridData() { }

  bool isEnabled() const { return majorTics_ || minorTics_; }

  bool hasMajorTics() const { return majorTics_; }
  void setMajorTics(bool b) { majorTics_ = b; }

  bool hasMinorTics() const { return minorTics_; }
  void setMinorTics(bool b) { minorTics_ = b; }

  const DrawLayer &layer() const { return layer_; }
  void setLayer(const DrawLayer &v) { layer_ = v; }

  double polarAngle() const { return polarAngle_; }
  void setPolarAngle(double r) { polarAngle_ = r; }

  int majorLineStyle() const { return majorLineStyle_; }
  void setMajorLineStyle(int i) { majorLineStyle_ = i; }

  int minorLineStyle() const { return minorLineStyle_; }
  void setMinorLineStyle(int i) { minorLineStyle_ = i; }

  int majorLineType() const { return majorLineType_; }
  void setMajorLineType(int i) { majorLineType_ = i; }

  int minorLineType() const { return minorLineType_; }
  void setMinorLineType(int i) { minorLineType_ = i; }

  double majorLineWidth() const { return majorLineWidth_; }
  void setMajorLineWidth(double r) { majorLineWidth_ = r; }

  double minorLineWidth() const { return minorLineWidth_; }
  void setMinorLineWidth(double r) { minorLineWidth_ = r; }

 private:
  bool      majorTics_      { false };
  bool      minorTics_      { false };
  DrawLayer layer_          { DrawLayer::DEFAULT };
  double    polarAngle_     { 30 };  // degrees
  int       majorLineStyle_ { -1 };
  int       minorLineStyle_ { -1 };
  int       majorLineType_  { -1 };
  int       minorLineType_  { -1 };
  double    majorLineWidth_ { 1.0 };
  double    minorLineWidth_ { 1.0 };
};

#endif
