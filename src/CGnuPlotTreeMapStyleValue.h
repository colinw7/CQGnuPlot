#ifndef CGnuPlotTreeMapStyleValue_H
#define CGnuPlotTreeMapStyleValue_H

#include <CGnuPlotStyleValue.h>
#include <CTreeMap.h>
#include <CRGBA.h>

class CGnuPlotTreeMapStyleValue : public CGnuPlotStyleValue {
 public:
  CGnuPlotTreeMapStyleValue(CGnuPlotPlot *plot) :
   CGnuPlotStyleValue(plot) {
    tree_ = new CTreeMap;
  }

 ~CGnuPlotTreeMapStyleValue() {
    delete tree_;
  }

  CTreeMap *tree() const { return tree_; }

  bool isInited() const { return inited_; }
  void setInited(bool b) { inited_ = b; }

  double openBorderWidth() const { return openBorderWidth_; }
  void setOpenBorderWidth(double r) { openBorderWidth_ = r; }

  const CRGBA &openBorderColor() const { return openBorderColor_; }
  void setOpenBorderColor(const CRGBA &v) { openBorderColor_ = v; }

  const CRGBA &closedBorderColor() const { return closedBorderColor_; }
  void setClosedBorderColor(const CRGBA &v) { closedBorderColor_ = v; }

  double closedBorderWidth() const { return closedBorderWidth_; }
  void setClosedBorderWidth(double r) { closedBorderWidth_ = r; }

  const std::string &palette() const { return palette_; }
  void setPalette(const std::string &v) { palette_ = v; }

  void init() {
    delete tree_;

    tree_ = new CTreeMap;
  }

 private:
  CTreeMap*   tree_ { nullptr };
  bool        inited_ { false };
  CRGBA       openBorderColor_ { 0, 0, 0 };
  double      openBorderWidth_ { 2 };
  CRGBA       closedBorderColor_ { 0.5, 0.5, 0.5 };
  double      closedBorderWidth_ { 0 };
  std::string palette_ { "sunburst" };
};

class CGnuPlotHierTreeMapStyleValue : public CGnuPlotTreeMapStyleValue {
 public:
  CGnuPlotHierTreeMapStyleValue(CGnuPlotPlot *plot) :
   CGnuPlotTreeMapStyleValue(plot) {
  }
};

#endif
