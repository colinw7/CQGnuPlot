#ifndef CGnuPlotLineType_H
#define CGnuPlotLineType_H

#include <CGnuPlotTypes.h>
#include <CGnuPlotColorSpec.h>
#include <CGnuPlotDash.h>
#include <CRefPtr.h>

class CGnuPlot;

class CGnuPlotLineType {
 public:
  typedef CGnuPlotTypes::SymbolType SymbolType;

 public:
  CGnuPlotLineType();

  virtual ~CGnuPlotLineType() { }

  int ind() const { return ind_; }
  void setInd(int i) { ind_ = i; }

  double lineWidth() const { return lineWidth_; }
  void setLineWidth(double width) { lineWidth_ = width; }

  const CGnuPlotDash &lineDash() const;
  void setLineDash(const CGnuPlotDash &dash) { lineDash_ = dash; }

  const CGnuPlotColorSpec &lineColor() const;
  void setLineColor(const CGnuPlotColorSpec &c) { lineColor_ = c; }

  int pointType() const { return pointType_; }
  void setPointType(int type) { pointTypeStr_ = ""; pointType_ = type; }
  void setPointType(const std::string &type) { pointType_ = -1; pointTypeStr_ = type; }

  double pointSize() const { return pointSize_; }
  void setPointSize(double s) { pointSize_ = s; }

  int pointInterval() const { return pointInterval_; }
  void setPointInterval(int pi) { pointInterval_ = pi; }

  bool palette() const { return palette_; }
  void setPalette(bool b) { palette_ = b; }

  SymbolType symbolType() const;

  CRGBA calcColor(const CGnuPlotGroup *group=0, const CRGBA &c=CRGBA(0,0,0)) const;

  CLineDash calcDash(CGnuPlot *plot, const CLineDash &d=CLineDash()) const;

  void init(int ind);

  void unset();

  void show(std::ostream &os) const;

 protected:
  // point type:
  //  -1 black
  //  -2 nodraw
  //  -3 bgnd

  int               ind_       { -1 };
  double            lineWidth_ { 1 };
  CGnuPlotDash      lineDash_;
  CGnuPlotColorSpec lineColor_;
  int               pointType_ { -1 };
  std::string       pointTypeStr_;
  double            pointSize_ { -1 };
  int               pointInterval_ { 0 };
  bool              palette_ { false };
};

typedef CRefPtr<CGnuPlotLineType> CGnuPlotLineTypeP;

#endif
