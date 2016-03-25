#ifndef CQGnuPlotCandlesticksStyleValue_H
#define CQGnuPlotCandlesticksStyleValue_H

#include <CGnuPlotCandlesticksStyleValue.h>
#include <CQGnuPlotPlot.h>
#include <QObject>

class CQGnuPlotPlot;

class CQGnuPlotCandlesticksStyleValue : public QObject, public CGnuPlotCandlesticksStyleValue {
  Q_OBJECT

  Q_PROPERTY(CQGnuPlotEnum::BoxWidthType type      READ type   WRITE setType)
  Q_PROPERTY(double                      width     READ width  WRITE setWidth)
  Q_PROPERTY(bool                        calc      READ isCalc WRITE setCalc)
  Q_PROPERTY(double                      spacing   READ getSpacing)
  Q_PROPERTY(bool                        autoWidth READ isAutoWidth)

 public:
  CQGnuPlotCandlesticksStyleValue(CQGnuPlotPlot *qplot) :
   CGnuPlotCandlesticksStyleValue(qplot), qplot_(qplot) {
  }

 ~CQGnuPlotCandlesticksStyleValue() { }

  CQGnuPlotPlot *qplot() const { return qplot_; }

  CQGnuPlotEnum::BoxWidthType type() const {
    return CQGnuPlotEnum::boxWidthTypeConv(CGnuPlotCandlesticksStyleValue::type());
  }

  void setType(const CQGnuPlotEnum::BoxWidthType &type) {
    if (type != this->type()) {
      CGnuPlotCandlesticksStyleValue::setType(CQGnuPlotEnum::boxWidthTypeConv(type));
    }
  }

 private:
  CQGnuPlotPlot *qplot_;
};

#endif
