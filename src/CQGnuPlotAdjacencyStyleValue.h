#ifndef CQGnuPlotAdjacencyStyleValue_H
#define CQGnuPlotAdjacencyStyleValue_H

#include <CGnuPlotAdjacencyStyleValue.h>
#include <CQGnuPlotPlot.h>
#include <QObject>

class CQGnuPlotPlot;

class CQGnuPlotAdjacencyStyleValue : public QObject, public CGnuPlotAdjacencyStyleValue {
  Q_OBJECT

  Q_PROPERTY(QString palette READ palette WRITE setPalette)

 public:
  CQGnuPlotAdjacencyStyleValue(CQGnuPlotPlot *qplot) :
   CGnuPlotAdjacencyStyleValue(qplot), qplot_(qplot) {
  }

  CQGnuPlotPlot *qplot() const { return qplot_; }

  QString palette() const {
    return CGnuPlotAdjacencyStyleValue::palette().c_str();
  }

  void setPalette(const QString &s) {
    if (s != palette()) {
      CGnuPlotAdjacencyStyleValue::setPalette(s.toStdString());
    }
  }

 private:
  CQGnuPlotPlot *qplot_;
};

#endif
