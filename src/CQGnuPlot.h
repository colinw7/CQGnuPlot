#ifndef CQGnuPlot_H
#define CQGnuPlot_H

#include <CGnuPlot.h>

class CQGnuPlotWindow;
class CQGnuPlotPlot;

class CQGnuPlot : public CGnuPlot {
 public:
  CQGnuPlot();
 ~CQGnuPlot() override;

  CGnuPlotWindow *createWindow() override;
  CGnuPlotPlot   *createPlot(CGnuPlotWindow *window) override;

  void timeout() override;

  void load(const std::string &filename);
};

#endif
