#ifndef CGnuPlotLabelsStyleValue_H
#define CGnuPlotLabelsStyleValue_H

#include <CGnuPlotStyleValue.h>
#include <CGnuPlotTextStyle.h>
#include <CGnuPlotTextBoxStyle.h>
#include <CGnuPlotLabelStyle.h>
#include <CGnuPlotPlot.h>

class CGnuPlotLabelsStyleValue : public CGnuPlotStyleValue {
 public:
  typedef CGnuPlotTypes::SymbolType   SymbolType;
  typedef COptValT<CGnuPlotColorSpec> OptColorSpec;

 public:
  CGnuPlotLabelsStyleValue(CGnuPlotPlot *plot) :
   CGnuPlotStyleValue(plot) {
  }

 ~CGnuPlotLabelsStyleValue() { }

  bool isInited() const { return inited_; }
  void setInited(bool b) { inited_ = b; }

  void init() {
    textStyle_    = plot_->textStyle();
    textBoxStyle_ = plot_->textBoxStyle();
    labelStyle_   = plot_->labelStyle();
  }

  bool isBoxed() const { return textStyle_.isBoxed(); }
  void setBoxed(bool b) { textStyle_.setBoxed(b); }

  const CPoint2D &offset() const { return textStyle_.offset(); }
  void setOffset(const CPoint2D &o) { textStyle_.setOffset(o); }

  const CHAlignType &halign() const { return textStyle_.halign(); }
  void setHAlign(const CHAlignType &v) { textStyle_.setHAlign(v); }

  bool isTransparent() const { return textBoxStyle_.isTransparent(); }
  void setTransparent(bool b) { textBoxStyle_.setTransparent(b); }

  OptColorSpec textColor() const { return labelStyle_.textColor(); }

  const CHAlignType &align() const { return labelStyle_.align(); }
  void setAlign(const CHAlignType &a) { labelStyle_.setAlign(a); }

  const CFontPtr &font() const { return labelStyle_.font(); }
  void setFont(const CFontPtr &f) { labelStyle_.setFont(f); }

  bool showPoint() const { return labelStyle_.showPoint(); }
  void setShowPoint(bool b) { labelStyle_.setShowPoint(b); }

  int pointType() const { return labelStyle_.pointType(); }
  void setPointType(int i) { labelStyle_.setPointType(i); }

  double pointSize() const { return labelStyle_.pointSize(); }
  void setPointSize(double r) { labelStyle_.setPointSize(r); }

  double lineWidth() const { return labelStyle_.lineWidth(); }
  void setLineWidth(double r) { labelStyle_.setLineWidth(r); }

  SymbolType symbolType() const { return labelStyle_.symbolType(); }

 private:
  bool                 inited_ { false };
  CGnuPlotTextStyle    textStyle_;
  CGnuPlotTextBoxStyle textBoxStyle_;
  CGnuPlotLabelStyle   labelStyle_;
};

#endif
