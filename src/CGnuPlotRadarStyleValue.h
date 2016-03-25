#ifndef CGnuPlotRadarStyleValue_H
#define CGnuPlotRadarStyleValue_H

#include <CGnuPlotStyleValue.h>
#include <CLineDash.h>
#include <CRGBA.h>
#include <string>

class CGnuPlotRadarStyleValue : public CGnuPlotStyleValue {
 public:
  CGnuPlotRadarStyleValue(CGnuPlotPlot *plot) :
   CGnuPlotStyleValue(plot) {
  }

 ~CGnuPlotRadarStyleValue() { }

  bool isInited() const { return inited_; }
  void setInited(bool b) { inited_ = b; }

  const std::string &palette() const { return palette_; }
  void setPalette(const std::string &v) { palette_ = v; }

  const CRGBA &borderColor() const { return borderColor_; }
  void setBorderColor(const CRGBA &v) { borderColor_ = v; }

  double borderWidth() const { return borderWidth_; }
  void setBorderWidth(double r) { borderWidth_ = r; }

  const CLineDash &borderDash() const { return borderDash_; }
  void setBorderDash(const CLineDash &v) { borderDash_ = v; }

  double angleStart() const { return angleStart_; }
  void setAngleStart(double r) { angleStart_ = r; }

  const CRGBA &axisColor() const { return axisColor_; }
  void setAxisColor(const CRGBA &v) { axisColor_ = v; }

  double axisAlpha() const { return axisAlpha_; }
  void setAxisAlpha(double r) { axisAlpha_ = r; }

  double axisWidth() const { return axisWidth_; }
  void setAxisWidth(double r) { axisWidth_ = r; }

  const CLineDash &axisDash() const { return axisDash_; }
  void setAxisDash(const CLineDash &v) { axisDash_ = v; }

  double strokeWidth() const { return strokeWidth_; }
  void setStrokeWidth(double r) { strokeWidth_ = r; }

  const CLineDash &strokeDash() const { return strokeDash_; }
  void setStrokeDash(const CLineDash &v) { strokeDash_ = v; }

 private:
  bool        inited_      { false };
  std::string palette_     { "subtle" };
  CRGBA       borderColor_ { 0, 0, 0 };
  double      borderWidth_ { 0 };
  CLineDash   borderDash_;
  double      angleStart_  { 90 };
  CRGBA       axisColor_   { 0.5, 0.5, 0.5 };
  double      axisAlpha_   { 0.5 };
  double      axisWidth_   { 0 };
  CLineDash   axisDash_;
  double      strokeWidth_ { 2 };
  CLineDash   strokeDash_;
};

#endif
