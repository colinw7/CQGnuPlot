#ifndef CGnuPlotLogDevice_H
#define CGnuPlotLogDevice_H

#include <CGnuPlotDevice.h>
#include <fstream>

class CGnuPlotLogRenderer;

class CGnuPlotLogDevice : public CGnuPlotDevice {
 public:
  CGnuPlotLogDevice();

 ~CGnuPlotLogDevice() override;

  CGnuPlotLogRenderer *svgRenderer() const { return renderer_; }

  void stateChanged(CGnuPlotWindow *window, CGnuPlotTypes::ChangeState state) override;

  void timeout() override;

  void drawInit(CGnuPlotWindow *window) override;
  void drawTerm() override;

  std::ostream &os() { return *os_; }

  CGnuPlotRenderer *renderer() override;

  bool parseArgs(CParseLine &line) override;

  void show(std::ostream &os) const override;

 private:
  CGnuPlotLogRenderer *renderer_;
  std::ostream        *os_;
  std::ofstream        file_;
};

#endif
