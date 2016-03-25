#ifndef CQGnuPlotRadarStyleValue_H
#define CQGnuPlotRadarStyleValue_H

#include <CGnuPlotRadarStyleValue.h>
#include <CQGnuPlotPlot.h>
#include <CQGnuPlotUtil.h>
#include <QObject>

class CQGnuPlotPlot;

class CQGnuPlotRadarStyleValue : public QObject, public CGnuPlotRadarStyleValue {
  Q_OBJECT

  Q_PROPERTY(QString palette     READ palette     WRITE setPalette    )
  Q_PROPERTY(QColor  borderColor READ borderColor WRITE setBorderColor)
  Q_PROPERTY(double  borderWidth READ borderWidth WRITE setBorderWidth)
  Q_PROPERTY(double  angleStart  READ angleStart  WRITE setAngleStart )
  Q_PROPERTY(QColor  axisColor   READ axisColor   WRITE setAxisColor  )
  Q_PROPERTY(double  axisAlpha   READ axisAlpha   WRITE setAxisAlpha  )
  Q_PROPERTY(double  axisWidth   READ axisWidth   WRITE setAxisWidth  )
  Q_PROPERTY(double  strokeWidth READ strokeWidth WRITE setStrokeWidth)

 public:
  CQGnuPlotRadarStyleValue(CQGnuPlotPlot *qplot) :
   CGnuPlotRadarStyleValue(qplot), qplot_(qplot) {
  }

  CQGnuPlotPlot *qplot() const { return qplot_; }

  QString palette() const {
    return CGnuPlotRadarStyleValue::palette().c_str();
  }

  void setPalette(const QString &s) {
    if (s != palette()) {
      CGnuPlotRadarStyleValue::setPalette(s.toStdString());

      qplot_->resetPolygonCache();
    }
  }

  QColor borderColor() const {
    return CQGnuPlotUtil::toQColor(CGnuPlotRadarStyleValue::borderColor());
  }

  void setBorderColor(const QColor &c) {
    if (c != borderColor()) {
      CGnuPlotRadarStyleValue::setBorderColor(CQGnuPlotUtil::fromQColor(c));
    }
  }

  QColor axisColor() const {
    return CQGnuPlotUtil::toQColor(CGnuPlotRadarStyleValue::axisColor());
  }

  void setAxisColor(const QColor &c) {
    if (c != axisColor()) {
      CGnuPlotRadarStyleValue::setAxisColor(CQGnuPlotUtil::fromQColor(c));
    }
  }

 private:
  CQGnuPlotPlot *qplot_;
};

#endif
