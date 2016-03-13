#ifndef CQGnuPlotDendrogramStyleValue_H
#define CQGnuPlotDendrogramStyleValue_H

#include <CGnuPlotDendrogramStyleValue.h>
#include <CQGnuPlotPlot.h>
#include <QObject>

class CQGnuPlotPlot;

class CQGnuPlotDendrogramStyleValue : public QObject, public CGnuPlotDendrogramStyleValue {
  Q_OBJECT

  Q_PROPERTY(double circleSize   READ circleSize   WRITE setCircleSize  )
  Q_PROPERTY(double marginLeft   READ marginLeft   WRITE setMarginLeft  )
  Q_PROPERTY(double marginRight  READ marginRight  WRITE setMarginRight )
  Q_PROPERTY(double marginBottom READ marginBottom WRITE setMarginBottom)
  Q_PROPERTY(double marginTop    READ marginTop    WRITE setMarginTop   )

 public:
  CQGnuPlotDendrogramStyleValue(CQGnuPlotPlot *qplot) :
   CGnuPlotDendrogramStyleValue(qplot), qplot_(qplot) {
  }

  CQGnuPlotPlot *qplot() const { return qplot_; }

 private:
  CQGnuPlotPlot *qplot_;
};

#endif
