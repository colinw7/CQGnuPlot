#ifndef CGnuPlotSunburstStyleValue_H
#define CGnuPlotSunburstStyleValue_H

#include <CGnuPlotStyleValue.h>
#include <CSunburst.h>
#include <CFontMgr.h>
#include <string>

class CGnuPlotSunburstStyleValue : public CGnuPlotStyleValue {
 public:
  CGnuPlotSunburstStyleValue(CGnuPlotPlot *plot=nullptr) :
   CGnuPlotStyleValue(plot) {
    font_ = CFontMgrInst->lookupFont("helvetica", CFONT_STYLE_NORMAL, 6);
  }

 ~CGnuPlotSunburstStyleValue() {
    delete sunburst_;
  }

  void init() {
    sunburst_ = new CSunburst;
  }

  CSunburst *sunburst() const { return sunburst_; }
  void setSunburst(CSunburst *s) { sunburst_ = s; }

  bool isInited() const { return inited_; }
  void setInited(bool b) { inited_ = b; }

  const CFontPtr &font() const { return font_; }
  void setFont(const CFontPtr &f) { font_ = f; }

  const CRGBA &borderColor() const { return borderColor_; }
  void setBorderColor(const CRGBA &v) { borderColor_ = v; }

  const std::string &palette() const { return palette_; }
  void setPalette(const std::string &v) { palette_ = v; }

  double innerRadius() const { return innerRadius_; }
  void setInnerRadius(double r) { innerRadius_ = r; }

  double startAngle() const { return startAngle_; }
  void setStartAngle(double r) { startAngle_ = r; }

 private:
  CSunburst*  sunburst_    { nullptr };
  bool        inited_      { false };
  CFontPtr    font_;
  CRGBA       borderColor_ { 0, 0, 0 };
  std::string palette_     { "sunburst" };
  double      innerRadius_ { 0.5 };
  double      startAngle_  { -90 };
};

#endif
