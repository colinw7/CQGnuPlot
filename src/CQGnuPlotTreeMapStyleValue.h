#ifndef CQGnuPlotTreeMapStyleValue_H
#define CQGnuPlotTreeMapStyleValue_H

#include <CGnuPlotTreeMapStyleValue.h>
#include <CQGnuPlotPlot.h>
#include <QObject>

class CQGnuPlotPlot;

class CQGnuPlotTreeMapStyleValue : public QObject, public CGnuPlotTreeMapStyleValue {
  Q_OBJECT

  Q_PROPERTY(double  openBorderWidth   READ openBorderWidth   WRITE setOpenBorderWidth  )
  Q_PROPERTY(double  closedBorderWidth READ closedBorderWidth WRITE setClosedBorderWidth)
  Q_PROPERTY(QString palette           READ palette           WRITE setPalette          )

 public:
  CQGnuPlotTreeMapStyleValue(CQGnuPlotPlot *qplot) :
   CGnuPlotTreeMapStyleValue(qplot), qplot_(qplot) {
  }

  CQGnuPlotPlot *qplot() const { return qplot_; }

  QString palette() const {
    return CGnuPlotTreeMapStyleValue::palette().c_str();
  }

  void setPalette(const QString &s) {
    if (s != palette()) {
      CGnuPlotTreeMapStyleValue::setPalette(s.toStdString());

      qplot_->resetRectCache();
    }
  }


 private:
  CQGnuPlotPlot *qplot_;
};

#endif
