#ifndef CGnuPlotPSDevice_H
#define CGnuPlotPSDevice_H

#include <CGnuPlotDevice.h>
#include <CPrint.h>
#include <fstream>

class CGnuPlotPSRenderer;

class CGnuPlotPSDevice : public CGnuPlotDevice {
 public:
  CGnuPlotPSDevice();

 ~CGnuPlotPSDevice() override;

  CGnuPlotPSRenderer *svgRenderer() const { return renderer_; }

  void stateChanged(CGnuPlotWindow *window, CGnuPlotTypes::ChangeState state) override;

  void timeout() override;

  void drawInit(CGnuPlotWindow *window) override;
  void drawTerm() override;

  CGnuPlotRenderer *renderer() override;

  CPrint &print() { return print_; }

  bool parseArgs(CParseLine &line) override;

  void show(std::ostream &os) const override;

 private:
  CGnuPlotPSRenderer *renderer_;
  CPrint              print_;
};

#endif
