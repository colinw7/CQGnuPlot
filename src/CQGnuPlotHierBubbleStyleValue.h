#ifndef CQGnuPlotHierBubbleStyleValue_H
#define CQGnuPlotHierBubbleStyleValue_H

#include <CGnuPlotHierBubbleStyleValue.h>
#include <CQGnuPlotPlot.h>
#include <QObject>

class CQGnuPlotPlot;

class CQGnuPlotHierBubbleStyleValue : public QObject, public CGnuPlotHierBubbleStyleValue {
  Q_OBJECT

  Q_PROPERTY(QString palette READ palette WRITE setPalette)

 public:
  CQGnuPlotHierBubbleStyleValue(CQGnuPlotPlot *qplot) :
   CGnuPlotHierBubbleStyleValue(qplot), qplot_(qplot) {
  }

  CQGnuPlotPlot *qplot() const { return qplot_; }

  QString palette() const {
    return CGnuPlotHierBubbleStyleValue::palette().c_str();
  }

  void setPalette(const QString &s) {
    if (s != palette()) {
      CGnuPlotHierBubbleStyleValue::setPalette(s.toStdString());
    }
  }

 private:
  CQGnuPlotPlot *qplot_;
};

#endif
