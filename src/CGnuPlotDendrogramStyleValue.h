#ifndef CGnuPlotDendrogramStyleValue_H
#define CGnuPlotDendrogramStyleValue_H

#include <CGnuPlotStyleValue.h>
#include <CDendrogram.h>

class CGnuPlotDendrogramStyleValue : public CGnuPlotStyleValue {
 public:
  CGnuPlotDendrogramStyleValue(CGnuPlotPlot *plot) :
   CGnuPlotStyleValue(plot) {
    dendrogram_ = new CDendrogram;
  }

 ~CGnuPlotDendrogramStyleValue() {
    delete dendrogram_;
  }

  CDendrogram *dendrogram() const { return dendrogram_; }

  bool isInited() const { return inited_; }
  void setInited(bool b) { inited_ = b; }

  void init() {
    delete dendrogram_;

    dendrogram_ = new CDendrogram;
  }

  double circleSize() const { return circleSize_; }
  void setCircleSize(double r) { circleSize_ = r; }

  double textMargin() const { return textMargin_; }
  void setTextMargin(double r) { textMargin_ = r; }

  double marginLeft() const { return marginLeft_; }
  void setMarginLeft(double r) { marginLeft_ = r; }

  double marginRight() const { return marginRight_; }
  void setMarginRight(double r) { marginRight_ = r; }

  double marginBottom() const { return marginBottom_; }
  void setMarginBottom(double r) { marginBottom_ = r; }

  double marginTop() const { return marginTop_; }
  void setMarginTop(double r) { marginTop_ = r; }

 private:
  CDendrogram *dendrogram_   { 0 };
  bool         inited_       { false };
  double       circleSize_   { 0.005 };
  double       textMargin_   { 0.001 };
  double       marginLeft_   { 0.05 };
  double       marginRight_  { 0.0 };
  double       marginBottom_ { 0.0 };
  double       marginTop_    { 0.0 };
};

#endif
