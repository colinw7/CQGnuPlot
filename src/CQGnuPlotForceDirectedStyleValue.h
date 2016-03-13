#ifndef CQGnuPlotForceDirectedStyleValue_H
#define CQGnuPlotForceDirectedStyleValue_H

#include <CGnuPlotForceDirectedStyleValue.h>
#include <CQGnuPlotPlot.h>
#include <QObject>

class CQGnuPlotPlot;

class CQGnuPlotForceDirectedStyleValue : public QObject, public CGnuPlotForceDirectedStyleValue {
  Q_OBJECT

  Q_PROPERTY(QString palette    READ palette     WRITE setPalette   )
  Q_PROPERTY(double  circleSize READ circleSize  WRITE setCircleSize)
  Q_PROPERTY(bool    animating  READ isAnimating WRITE setAnimating )

 public:
  CQGnuPlotForceDirectedStyleValue(CQGnuPlotPlot *qplot) :
   CGnuPlotForceDirectedStyleValue(qplot), qplot_(qplot) {
  }

  CQGnuPlotPlot *qplot() const { return qplot_; }

  QString palette() const {
    return CGnuPlotForceDirectedStyleValue::palette().c_str();
  }

  void setPalette(const QString &s) {
    if (s != palette()) {
      CGnuPlotForceDirectedStyleValue::setPalette(s.toStdString());
    }
  }

  void setCircleSize(double r) {
    if (r != circleSize()) {
      CGnuPlotForceDirectedStyleValue::setCircleSize(r);
    }
  }

 private:
  CQGnuPlotPlot *qplot_;
};

#endif
