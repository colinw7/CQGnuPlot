#ifndef CQGnuPlotPieChartStyleValue_H
#define CQGnuPlotPieChartStyleValue_H

#include <CGnuPlotPieChartStyleValue.h>
#include <CQGnuPlotPlot.h>
#include <CQGnuPlotUtil.h>
#include <QObject>

class CQGnuPlotPlot;

class CQGnuPlotPieChartStyleValue : public QObject, public CGnuPlotPieChartStyleValue {
  Q_OBJECT

  Q_PROPERTY(QString palette     READ palette     WRITE setPalette    )
  Q_PROPERTY(double  alpha       READ alpha       WRITE setAlpha      )
  Q_PROPERTY(double  startAngle  READ startAngle  WRITE setStartAngle )
  Q_PROPERTY(double  innerRadius READ innerRadius WRITE setInnerRadius)
  Q_PROPERTY(double  labelRadius READ labelRadius WRITE setLabelRadius)

 public:
  CQGnuPlotPieChartStyleValue(CQGnuPlotPlot *qplot) :
   CGnuPlotPieChartStyleValue(qplot), qplot_(qplot) {
  }

  CQGnuPlotPlot *qplot() const { return qplot_; }

  QString palette() const {
    return CGnuPlotPieChartStyleValue::palette().c_str();
  }

  void setPalette(const QString &s) {
    if (s != CGnuPlotPieChartStyleValue::palette().c_str()) {
      CGnuPlotPieChartStyleValue::setPalette(s.toStdString());

      qplot_->clearPieCache();
    }
  }

  void setAlpha(double r) {
    if (r != CGnuPlotPieChartStyleValue::alpha()) {
      CGnuPlotPieChartStyleValue::setAlpha(r);

      qplot_->clearPieCache();
    }
  }

  void setStartAngle(double r) {
    if (r != CGnuPlotPieChartStyleValue::startAngle()) {
      CGnuPlotPieChartStyleValue::setStartAngle(r);

      qplot_->clearPieCache();
    }
  }

  void setInnerRadius(double r) {
    if (r != CGnuPlotPieChartStyleValue::innerRadius()) {
      CGnuPlotPieChartStyleValue::setInnerRadius(r);

      qplot_->clearPieCache();
    }
  }

  void setLabelRadius(double r) {
    if (r != CGnuPlotPieChartStyleValue::labelRadius()) {
      CGnuPlotPieChartStyleValue::setLabelRadius(r);

      qplot_->clearPieCache();
    }
  }

 private:
  CQGnuPlotPlot *qplot_;
};

#endif
