#ifndef CQGnuPlot_H
#define CQGnuPlot_H

#include <CGnuPlot.h>

class CQGnuPlotWindow;
class CQGnuPlotPlot;

class CQGnuPlot : public CGnuPlot {
 public:
  CQGnuPlot();
 ~CQGnuPlot();

  CGnuPlotWindow *createWindow();
  CGnuPlotPlot   *createPlot(CGnuPlotWindow *window);

  void timeout();

  void load(const std::string &filename);
};

#endif
