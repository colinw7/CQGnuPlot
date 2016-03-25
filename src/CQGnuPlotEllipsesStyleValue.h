#ifndef CQGnuPlotEllipsesStyleValue_H
#define CQGnuPlotEllipsesStyleValue_H

#include <CGnuPlotEllipsesStyleValue.h>
#include <CQGnuPlotPlot.h>
#include <QObject>

class CQGnuPlotEllipsesStyleValue : public QObject, public CGnuPlotEllipsesStyleValue {
  Q_OBJECT

  Q_PROPERTY(double angle READ angle WRITE setAngle)
  Q_PROPERTY(bool   clip  READ clip  WRITE setClip )

 public:
  CQGnuPlotEllipsesStyleValue(CQGnuPlotPlot *qplot) :
   CGnuPlotEllipsesStyleValue(qplot), qplot_(qplot) {
  }

 ~CQGnuPlotEllipsesStyleValue() { }

  CQGnuPlotPlot *qplot() const { return qplot_; }

 private:
  CQGnuPlotPlot *qplot_;
};

#endif
