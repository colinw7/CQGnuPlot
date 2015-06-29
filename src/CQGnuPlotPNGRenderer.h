#ifndef CQGnuPlotPNGRenderer_H
#define CQGnuPlotPNGRenderer_H

#include <CQGnuPlotRenderer.h>

class CQGnuPlotPNGDevice;

class CQGnuPlotPNGRenderer : public CQGnuPlotRenderer {
 public:
  CQGnuPlotPNGRenderer(CQGnuPlotPNGDevice *device);
 ~CQGnuPlotPNGRenderer() override;

 private:
  CQGnuPlotPNGDevice *device_;
};

#endif
