#ifndef CGnuPlotBoxObject_H
#define CGnuPlotBoxObject_H

#include <CGnuPlotObject.h>
#include <CGnuPlotTypes.h>
#include <CGnuPlotStroke.h>
#include <CGnuPlotFill.h>
#include <CGnuPlotMark.h>
#include <CBoxWhisker.h>

class CGnuPlotPlot;
class CGnuPlotRenderer;

class CGnuPlotBoxObject : public CGnuPlotPlotObject {
 public:
  typedef CBoxWhisker::Values           Values;
  typedef CBoxWhisker::Outliers         Outliers;
  typedef std::pair<CHAlignType,double> HAlignPos;
  typedef std::pair<CVAlignType,double> VAlignPos;

 public:
  CGnuPlotBoxObject(CGnuPlotPlot *plot);

  virtual ~CGnuPlotBoxObject() { }

  void setValues(const std::vector<double> &values) {
    whisker_.setValues(values);
  }

  double x() const { return x_; }
  void setX(double x) { x_ = x; }

  double y() const { return y_; }
  void setY(double y) { y_ = y; }

  double lineWidth() const { return lineWidth_; }
  void setLineWidth(double r) { lineWidth_ = r; }

  double boxWidth() const { return boxWidth_; }
  void setBoxWidth(double r) { boxWidth_ = r; }

  double minValue() const { return whisker_.min(); }
  double maxValue() const { return whisker_.max(); }

  double medianValue() const { return whisker_.median(); }
  double lowerValue () const { return whisker_.lower (); }
  double upperValue () const { return whisker_.upper (); }

  bool hasOutliers() const { return ! whisker_.outliers().empty(); }

  const Outliers &outlierValues() const { return whisker_.outliers(); }

  double value(int i) const { return whisker_.value(i); }

  const std::string &valueStr() const { return valueStr_; }
  void setValueStr(const std::string &v) { valueStr_ = v; }

  const CRGBA &textColor() const { return textColor_; }
  void setTextColor(const CRGBA &v) { textColor_ = v; }

  const CGnuPlotStrokeP &stroke() const { return stroke_; }
  void setStroke(const CGnuPlotStrokeP &s) { stroke_ = s; }

  const CGnuPlotFillP &fill() const { return fill_; }
  void setFill(const CGnuPlotFillP &f) { fill_ = f; }

  const CGnuPlotMarkP &outlierMark() const { return outlierMark_; }
  void setOutlierMark(const CGnuPlotMarkP &m) { outlierMark_ = m; }

  const CBBox2D &bbox() const override { return bbox_; }

  CGnuPlotTipData tip() const override;

  void draw(CGnuPlotRenderer *renderer) const override;

 protected:
  CBoxWhisker     whisker_;
  double          x_             { 0 };
  double          y_             { 0 };
  double          lineWidth_     { 1 };
  double          boxWidth_      { 1 };
  std::string     valueStr_;
  CRGBA           textColor_     { 0, 0, 0 };
  CGnuPlotStrokeP stroke_;
  CGnuPlotFillP   fill_;
  CGnuPlotMarkP   outlierMark_;
  mutable CBBox2D bbox_;
};

#endif
