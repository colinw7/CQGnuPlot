#ifndef CQGnuPlotPNGDevice_H
#define CQGnuPlotPNGDevice_H

#include <CGnuPlotDevice.h>
#include <QSize>

class CQGnuPlotPNGRenderer;
class QPainter;
class QImage;

class CQGnuPlotPNGDevice : public CGnuPlotDevice {
 public:
  CQGnuPlotPNGDevice();

 ~CQGnuPlotPNGDevice() override;

  CQGnuPlotPNGRenderer *pngRenderer() const { return renderer_; }

  void setOutputSize(const QSize &size);

  void stateChanged(CGnuPlotWindow *window, CGnuPlotTypes::ChangeState state) override;

  void timeout() override;

  void drawInit(CGnuPlotWindow *window) override;
  void drawTerm() override;

  CGnuPlotRenderer *renderer() override;

  bool parseArgs(CParseLine &line) override;

  void show(std::ostream &os) const override;

 private:
  CQGnuPlotPNGRenderer *renderer_   { 0 };
  QSize                 outputSize_ { 800, 800 };
  QPainter             *painter_    { 0 };
  QImage               *image_      { 0 };
  int                   count_      { 1 };
};

#endif
