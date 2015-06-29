#ifndef CQGnuPlotPNGDevice_H
#define CQGnuPlotPNGDevice_H

#include <CGnuPlotDevice.h>

class CQGnuPlotPNGRenderer;
class QPainter;
class QImage;

class CQGnuPlotPNGDevice : public CGnuPlotDevice {
 public:
  CQGnuPlotPNGDevice();

 ~CQGnuPlotPNGDevice() override;

  CQGnuPlotPNGRenderer *pngRenderer() const { return renderer_; }

  void stateChanged(CGnuPlotWindow *window, CGnuPlotTypes::ChangeState state) override;

  void timeout() override;

  void drawInit(CGnuPlotWindow *window) override;
  void drawTerm() override;

  CGnuPlotRenderer *renderer() override;

  bool parseArgs(CParseLine &line) override;

  void show(std::ostream &os) const override;

 private:
  CQGnuPlotPNGRenderer *renderer_;
  QPainter             *painter_;
  QImage               *image_;
  int                   count_;
};

#endif
