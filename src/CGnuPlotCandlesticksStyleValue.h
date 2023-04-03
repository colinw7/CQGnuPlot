#ifndef CGnuPlotCandlesticksStyleValue_H
#define CGnuPlotCandlesticksStyleValue_H

#include <CGnuPlotStyleValue.h>
#include <CGnuPlotBoxWidth.h>

class CGnuPlotPlot;

class CGnuPlotCandlesticksStyleValue : public CGnuPlotStyleValue {
 public:
  typedef CGnuPlotTypes::BoxWidthType BoxWidthType;

 public:
  CGnuPlotCandlesticksStyleValue(CGnuPlotPlot *plot=nullptr) :
   CGnuPlotStyleValue(plot) {
  }

 ~CGnuPlotCandlesticksStyleValue() { }

  bool isInited() const { return inited_; }
  void setInited(bool b) { inited_ = b; }

  void init();

  const CGnuPlotBoxWidth &boxWidth() const { return boxWidth_; }
  void setBoxWidth(const CGnuPlotBoxWidth &v) { boxWidth_ = v; }

  CGnuPlotTypes::BoxWidthType getBoxWidthType() const { return boxWidth_.type(); }
  void setBoxWidthType(const CGnuPlotTypes::BoxWidthType &t) { boxWidth_.setType(t); }

  void unset() { boxWidth_.unset(); }

  double width() const { return boxWidth_.width(); }
  void setWidth(double w) { boxWidth_.setWidth(w); }

  BoxWidthType type() const { return boxWidth_.type(); }
  void setType(BoxWidthType type) { boxWidth_.setType(type); }

  bool isCalc() const { return boxWidth_.isCalc(); }
  void setCalc(bool b) { boxWidth_.setCalc(b); }

  double getSpacing() const;

  bool isAutoWidth() const { return (width() == -2); }

 private:
  bool             inited_ { false };
  CGnuPlotBoxWidth boxWidth_;
};

#endif
