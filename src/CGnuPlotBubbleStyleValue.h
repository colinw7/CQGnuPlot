#ifndef CGnuPlotBubbleStyleValue_H
#define CGnuPlotBubbleStyleValue_H

#include <CGnuPlotStyleValue.h>
#include <CBubblePack.h>

class CGnuPlotBubbleStyleValue : public CGnuPlotStyleValue {
 public:
  CGnuPlotBubbleStyleValue(CGnuPlotPlot *plot) :
   CGnuPlotStyleValue(plot) {
    pack_ = new CBubblePack;
  }

 ~CGnuPlotBubbleStyleValue() {
    delete pack_;
  }

  CBubblePack *pack() const { return pack_; }

  bool isInited() const { return inited_; }
  void setInited(bool b) { inited_ = b; }

  void init() {
    delete pack_;

    pack_ = new CBubblePack;
  }

  const std::string &palette() const { return palette_; }
  void setPalette(const std::string &v) { palette_ = v; }

 private:
  CBubblePack *pack_    { nullptr };
  bool         inited_  { false };
  std::string  palette_ { "subtle" };
};

#endif
