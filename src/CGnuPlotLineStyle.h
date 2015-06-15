#ifndef CGnuPlotLineStyle_H
#define CGnuPlotLineStyle_H

#include <CGnuPlotTypes.h>
#include <CGnuPlotColorSpec.h>

#include <CRefPtr.h>
#include <CRGBA.h>
#include <COptVal.h>
#include <CStrUtil.h>

#include <sstream>

class CGnuPlot;

class CGnuPlotLineStyle {
 public:
  typedef CGnuPlotTypes::SymbolType   SymbolType;
  typedef COptValT<SymbolType>        OptSymbolType;
  typedef COptValT<CGnuPlotColorSpec> OptColorSpec;

 public:
  CGnuPlotLineStyle(CGnuPlot *plot);

  virtual ~CGnuPlotLineStyle() { }

  int ind() const { return ind_; }
  void setInd(int i) { ind_ = i; }

  const COptInt &lineType() const { return lineType_; }
  void setLineType(int type);
  void unsetLineType() { lineType_.setInvalid(); }

  const COptReal &lineWidth() const { return lineWidth_; }
  void setLineWidth(double width) { lineWidth_ = width; }
  void unsetLineWidth() { lineWidth_.setInvalid(); }

  const COptInt &lineDash() const { return lineDash_; }
  void setLineDash(int d) { lineDash_ = d; }
  void unsetLineDash() { lineDash_.setInvalid(); }

  const OptColorSpec &lineColor() const { return lineColor_; }
  void setLineColor(const CGnuPlotColorSpec &c) { lineColor_ = c; }
  void unsetLineColor() { lineColor_.setInvalid(); }

  const COptInt &pointType() const { return pointType_; }
  void setPointType(int type) { pointTypeStr_ = ""; pointType_ = type; }
  void setPointType(const std::string &str) {pointType_.setInvalid();  pointTypeStr_ = str; }
  void resetPointType() { pointType_.setInvalid(); pointTypeStr_ = ""; }

  const std::string &pointTypeStr() const { return pointTypeStr_; }

  const COptReal &pointSize() const { return pointSize_; }
  void setPointSize(double s) { pointSize_ = s; }
  void resetPointSize() { pointSize_.setInvalid(); }

  const COptInt &pointInterval() const { return pointInterval_; }
  void setPointInterval(int pi) { pointInterval_ = pi; }
  void resetPointInterval() { pointInterval_.setInvalid(); }

  bool palette() const { return palette_; }
  void setPalette(bool b) { palette_ = b; }

  //---

  double calcWidth(double w=1) const;

  int calcDashInd(int d=0) const;

  CLineDash calcDash(const CLineDash &d=CLineDash()) const;

  CRGBA calcColor(CGnuPlotPlot *plot, double x=0.0) const;

  CRGBA calcColor(CGnuPlotGroup *group, const CRGBA &c=CRGBA(0,0,0)) const;

  SymbolType calcPointType(const SymbolType &t=SymbolType::PLUS) const;

  double calcPointSize(double s=1) const;

  int calcPointInterval(int i=0) const;

  bool isCalcColor() const;

  //---

  void init(int ind);

  void unset();

  void print(std::ostream &os) const;

  void show(std::ostream &os) const;

 protected:
  CGnuPlot*    plot_ { 0 };
  int          ind_  { -1 };
  COptInt      lineType_;
  COptReal     lineWidth_;
  COptInt      lineDash_;
  OptColorSpec lineColor_;
  COptInt      pointType_;
  std::string  pointTypeStr_;
  COptReal     pointSize_;
  COptInt      pointInterval_;
  bool         palette_ { false };
};

typedef CRefPtr<CGnuPlotLineStyle> CGnuPlotLineStyleP;

#endif
