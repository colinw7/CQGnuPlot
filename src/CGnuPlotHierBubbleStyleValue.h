#ifndef CGnuPlotHierBubbleStyleValue_H
#define CGnuPlotHierBubbleStyleValue_H

#include <CGnuPlotStyleValue.h>
#include <CHierBubblePack.h>

class CGnuPlotHierBubbleStyleValue : public CGnuPlotStyleValue {
 public:
  CGnuPlotHierBubbleStyleValue(CGnuPlotPlot *plot) :
   CGnuPlotStyleValue(plot) {
    pack_ = new CHierBubblePack;
  }

 ~CGnuPlotHierBubbleStyleValue() {
    delete pack_;
  }

  CHierBubblePack *pack() const { return pack_; }

  bool isInited() const { return inited_; }
  void setInited(bool b) { inited_ = b; }

  void init() {
    delete pack_;

    pack_ = new CHierBubblePack;
  }

  const std::string &palette() const { return palette_; }
  void setPalette(const std::string &v) { palette_ = v; }

 private:
  CHierBubblePack *pack_ { 0 };
  bool             inited_ { false };
  std::string      palette_ { "subtle" };
};

#endif
