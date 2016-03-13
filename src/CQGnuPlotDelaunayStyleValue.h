#ifndef CQGnuPlotDelaunayStyleValue_H
#define CQGnuPlotDelaunayStyleValue_H

#include <CGnuPlotDelaunayStyleValue.h>
#include <CQGnuPlotPlot.h>
#include <CQGnuPlotUtil.h>
#include <QObject>

class CQGnuPlotPlot;

class CQGnuPlotDelaunayStyleValue : public QObject, public CGnuPlotDelaunayStyleValue {
  Q_OBJECT

  Q_PROPERTY(QColor lineColor READ lineColor WRITE setLineColor)
  Q_PROPERTY(double lineWidth READ lineWidth WRITE setLineWidth)

 public:
  CQGnuPlotDelaunayStyleValue(CQGnuPlotPlot *qplot) :
   CGnuPlotDelaunayStyleValue(qplot), qplot_(qplot) {
  }

  CQGnuPlotPlot *qplot() const { return qplot_; }

  QColor lineColor() const {
    return CQGnuPlotUtil::toQColor(CGnuPlotDelaunayStyleValue::lineColor());
  }

  void setLineColor(const QColor &c) {
    CGnuPlotDelaunayStyleValue::setLineColor(CQGnuPlotUtil::fromQColor(c));
  }

 private:
  CQGnuPlotPlot *qplot_;
};

#endif
