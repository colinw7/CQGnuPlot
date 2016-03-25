#ifndef CQGnuPlotCirclesStyleValue_H
#define CQGnuPlotCirclesStyleValue_H

#include <CGnuPlotCirclesStyleValue.h>
#include <CQGnuPlotPlot.h>
#include <QObject>

class CQGnuPlotCirclesStyleValue : public QObject, public CGnuPlotCirclesStyleValue {
  Q_OBJECT

  Q_PROPERTY(bool wedge READ wedge WRITE setWedge)
  Q_PROPERTY(bool clip  READ clip  WRITE setClip )

 public:
  CQGnuPlotCirclesStyleValue(CQGnuPlotPlot *qplot) :
   CGnuPlotCirclesStyleValue(qplot), qplot_(qplot) {
  }

 ~CQGnuPlotCirclesStyleValue() { }

  CQGnuPlotPlot *qplot() const { return qplot_; }

 private:
  CQGnuPlotPlot *qplot_;
};

#endif
