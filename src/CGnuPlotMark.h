#ifndef CGnuPlotMark_H
#define CGnuPlotMark_H

#include <CRGBA.h>
#include <CRefPtr.h>
#include <CGnuPlotTypes.h>

class CGnuPlotPlot;

class CGnuPlotMark {
 public:
  typedef CGnuPlotTypes::SymbolType SymbolType;

 public:
  CGnuPlotMark(CGnuPlotPlot *plot=0);

  virtual ~CGnuPlotMark() { }

  virtual CGnuPlotMark *dup() const;

  void init(CGnuPlotPlot *plot);

  const SymbolType &type() const { return type_; }
  void setType(const SymbolType &v) { type_ = v; }

  double size() const { return size_; }
  void setSize(double size) { size_ = size; }

  const CRGBA &color() const { return color_; }
  void setColor(const CRGBA &color) { color_ = color; }

  double width() const { return width_; }
  void setWidth(double w) { width_ = w; }

  bool isPixelSize() const { return pixelSize_; }
  void setPixelSize(bool b) { pixelSize_ = b; }

 protected:
  CGnuPlotPlot* plot_      { 0 };
  SymbolType    type_      { SymbolType::CROSS };
  double        size_      { 1 };
  CRGBA         color_     { 0, 0, 0 };
  double        width_     { 1 };
  bool          pixelSize_ { true };
};

typedef CRefPtr<CGnuPlotMark> CGnuPlotMarkP;

#endif

