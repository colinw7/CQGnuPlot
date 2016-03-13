#ifndef CQGnuPlotChordDiagramStyleValue_H
#define CQGnuPlotChordDiagramStyleValue_H

#include <CGnuPlotChordDiagramStyleValue.h>
#include <CQGnuPlotPlot.h>
#include <QObject>

class CQGnuPlotPlot;

class CQGnuPlotChordDiagramStyleValue : public QObject, public CGnuPlotChordDiagramStyleValue {
  Q_OBJECT

  Q_PROPERTY(QString palette READ palette WRITE setPalette)

 public:
  CQGnuPlotChordDiagramStyleValue(CQGnuPlotPlot *qplot) :
   CGnuPlotChordDiagramStyleValue(qplot), qplot_(qplot) {
  }

  CQGnuPlotPlot *qplot() const { return qplot_; }

  QString palette() const {
    return CGnuPlotChordDiagramStyleValue::palette().c_str();
  }

  void setPalette(const QString &s) {
    if (s != palette()) {
      CGnuPlotChordDiagramStyleValue::setPalette(s.toStdString());
    }
  }

 private:
  CQGnuPlotPlot *qplot_;
};

#endif
