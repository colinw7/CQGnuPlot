#ifndef CQGnuPlotBubbleStyleValue_H
#define CQGnuPlotBubbleStyleValue_H

#include <CGnuPlotBubbleStyleValue.h>
#include <CQGnuPlotPlot.h>
#include <QObject>

class CQGnuPlotPlot;

class CQGnuPlotBubbleStyleValue : public QObject, public CGnuPlotBubbleStyleValue {
  Q_OBJECT

  Q_PROPERTY(QString palette READ palette WRITE setPalette)

 public:
  CQGnuPlotBubbleStyleValue(CQGnuPlotPlot *qplot) :
   CGnuPlotBubbleStyleValue(qplot), qplot_(qplot) {
  }

  CQGnuPlotPlot *qplot() const { return qplot_; }

  QString palette() const {
    return CGnuPlotBubbleStyleValue::palette().c_str();
  }

  void setPalette(const QString &s) {
    if (s != palette()) {
      CGnuPlotBubbleStyleValue::setPalette(s.toStdString());

      qplot_->resetBubbleCache();
    }
  }

 private:
  CQGnuPlotPlot *qplot_;
};

#endif
