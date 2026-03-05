#ifndef CGnuPlotLineStyle_H
#define CGnuPlotLineStyle_H

#include <CGnuPlotTypes.h>
#include <CGnuPlotColorSpec.h>
#include <CGnuPlotDash.h>

#include <CRGBA.h>
#include <CStrUtil.h>

#include <sstream>
#include <optional>

class CGnuPlot;

class CGnuPlotLineStyle {
 public:
  typedef CGnuPlotTypes::SymbolType        SymbolType;
  typedef std::optional<SymbolType>        OptSymbolType;
  typedef std::optional<CGnuPlotColorSpec> OptColorSpec;

 public:
  CGnuPlotLineStyle(CGnuPlot *plot);

  virtual ~CGnuPlotLineStyle() { }

  CGnuPlotLineStyle *dup() const {
    return new CGnuPlotLineStyle(*this);
  }

  const std::optional<int> &ind() const { return ind_; }
  void setInd(int i) { ind_ = i; }
  void resetInd() { ind_.reset(); }

  const std::optional<int> &lineType() const { return lineType_; }
  void setLineType(int type);
  void unsetLineType() { lineType_.reset(); }

  const std::optional<double> &lineWidth() const { return lineWidth_; }
  void setLineWidth(double width) { lineWidth_ = width; }
  void unsetLineWidth() { lineWidth_.reset(); }

  const CGnuPlotDash &lineDash() const { return lineDash_; }
  void setLineDash(const CGnuPlotDash &dash) { lineDash_ = dash; }
  void unsetLineDash() { lineDash_.reset(); }

  const OptColorSpec &lineColor() const { return lineColor_; }
  void setLineColor(const CGnuPlotColorSpec &c) { lineColor_ = c; }
  void unsetLineColor() { lineColor_.reset(); }

  const std::optional<int> &pointType() const { return pointType_; }
  void setPointType(int type) { pointTypeStr_ = ""; pointType_ = type; }
  void resetPointType() { pointType_.reset(); pointTypeStr_ = ""; }

  void setPointTypeStr(const std::string &str) {pointType_.reset(); pointTypeStr_ = str; }
  const std::string &pointTypeStr() const { return pointTypeStr_; }

  const std::optional<double> &pointSize() const { return pointSize_; }
  void setPointSize(double s) { pointSize_ = s; }
  void resetPointSize() { pointSize_.reset(); }

  const std::optional<int> &pointInterval() const { return pointInterval_; }
  void setPointInterval(int pi) { pointInterval_ = pi; }
  void resetPointInterval() { pointInterval_.reset(); }

  bool palette() const { return palette_; }
  void setPalette(bool b) { palette_ = b; }

  bool isTipPoints() const { return tippoints_; }
  void setTipPoints(bool b) { tippoints_ = b; }

  //---

  double calcWidth(double w=1) const;

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
  CGnuPlot*             plot_ { nullptr };
  std::optional<int>    ind_;
  std::optional<int>    lineType_;
  std::optional<double> lineWidth_;
  CGnuPlotDash          lineDash_;
  OptColorSpec          lineColor_;
  std::optional<int>    pointType_;
  std::string           pointTypeStr_;
  std::optional<double> pointSize_;
  std::optional<int>    pointInterval_;
  bool                  palette_ { false };
  bool                  tippoints_ { true };
};

typedef std::shared_ptr<CGnuPlotLineStyle> CGnuPlotLineStyleP;

#endif
