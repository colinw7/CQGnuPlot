#ifndef CQGnuPlotBivariateStyleValue_H
#define CQGnuPlotBivariateStyleValue_H

#include <CGnuPlotBivariateStyleValue.h>
#include <CQGnuPlotPlot.h>
#include <QObject>

class CQGnuPlotPlot;

class CQGnuPlotBivariateStyleValue : public QObject, public CGnuPlotBivariateStyleValue {
  Q_OBJECT

 public:
  CQGnuPlotBivariateStyleValue(CQGnuPlotPlot *qplot) :
   CGnuPlotBivariateStyleValue(qplot), qplot_(qplot) {
  }

  CQGnuPlotPlot *qplot() const { return qplot_; }

 private:
  CQGnuPlotPlot *qplot_;
};

#endif
