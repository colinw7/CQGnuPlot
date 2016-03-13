#ifndef CQGnuPlotSunburstStyleValue_H
#define CQGnuPlotSunburstStyleValue_H

#include <CGnuPlotSunburstStyleValue.h>
#include <CQGnuPlotPlot.h>
#include <CQGnuPlotUtil.h>
#include <QObject>

class CQGnuPlotPlot;

class CQGnuPlotSunburstStyleValue : public QObject, public CGnuPlotSunburstStyleValue {
  Q_OBJECT

  Q_PROPERTY(QColor  borderColor READ borderColor WRITE setBorderColor)
  Q_PROPERTY(QString palette     READ palette     WRITE setPalette    )
  Q_PROPERTY(double  innerRadius READ innerRadius WRITE setInnerRadius)
  Q_PROPERTY(double  startAngle  READ startAngle  WRITE setStartAngle )

 public:
  CQGnuPlotSunburstStyleValue(CQGnuPlotPlot *qplot) :
   CGnuPlotSunburstStyleValue(qplot), qplot_(qplot) {
  }

  CQGnuPlotPlot *qplot() const { return qplot_; }

  QColor borderColor() const {
    return CQGnuPlotUtil::toQColor(CGnuPlotSunburstStyleValue::borderColor());
  }

  void setBorderColor(const QColor &c) {
    if (c != borderColor()) {
      CGnuPlotSunburstStyleValue::setBorderColor(CQGnuPlotUtil::fromQColor(c));

      qplot_->clearPieCache();
    }
  }

  QString palette() const {
    return CGnuPlotSunburstStyleValue::palette().c_str();
  }

  void setPalette(const QString &s) {
    if (s != palette()) {
      CGnuPlotSunburstStyleValue::setPalette(s.toStdString());

      qplot_->clearPieCache();
    }
  }

  void setInnerRadius(double r) {
    if (r != innerRadius()) {
      CGnuPlotSunburstStyleValue::setInnerRadius(r);

      setInited(false);

      qplot_->clearPieCache();
    }
  }

  void setStartAngle(double r) {
    if (r != startAngle()) {
      CGnuPlotSunburstStyleValue::setStartAngle(r);

      setInited(false);

      qplot_->clearPieCache();
    }
  }

 private:
  CQGnuPlotPlot *qplot_;
};

#endif
