#ifndef CGnuPlotSVGDevice_H
#define CGnuPlotSVGDevice_H

#include <CGnuPlotDevice.h>
#include <fstream>

class CGnuPlotSVGRenderer;

class CGnuPlotSVGDevice : public CGnuPlotDevice {
 public:
  CGnuPlotSVGDevice();

 ~CGnuPlotSVGDevice() override;

  CGnuPlotSVGRenderer *svgRenderer() const { return renderer_; }

  void stateChanged(CGnuPlotWindow *window, CGnuPlotTypes::ChangeState state) override;

  void timeout() override;

  void drawInit(CGnuPlotWindow *window) override;
  void drawTerm() override;

  std::ostream &os() { return *os_; }

  CGnuPlotRenderer *renderer() override;

  bool parseArgs(CParseLine &line) override;

  void show(std::ostream &os) const override;

 private:
  CGnuPlotSVGRenderer *renderer_;
  std::ostream        *os_;
  std::ofstream        file_;
};

#endif
