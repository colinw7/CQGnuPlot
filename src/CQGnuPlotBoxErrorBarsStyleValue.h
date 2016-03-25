#ifndef CQGnuPlotBoxErrorBarsStyleValue_H
#define CQGnuPlotBoxErrorBarsStyleValue_H

#include <CGnuPlotBoxErrorBarsStyleValue.h>
#include <CQGnuPlotPlot.h>
#include <QObject>

class CQGnuPlotPlot;

class CQGnuPlotBoxErrorBarsStyleValue : public QObject, public CGnuPlotBoxErrorBarsStyleValue {
  Q_OBJECT

  Q_PROPERTY(CQGnuPlotEnum::BoxWidthType type      READ type   WRITE setType)
  Q_PROPERTY(double                      width     READ width  WRITE setWidth)
  Q_PROPERTY(bool                        calc      READ isCalc WRITE setCalc)
  Q_PROPERTY(double                      spacing   READ getSpacing)
  Q_PROPERTY(bool                        autoWidth READ isAutoWidth)

 public:
  CQGnuPlotBoxErrorBarsStyleValue(CQGnuPlotPlot *qplot) :
   CGnuPlotBoxErrorBarsStyleValue(qplot), qplot_(qplot) {
  }

 ~CQGnuPlotBoxErrorBarsStyleValue() { }

  CQGnuPlotPlot *qplot() const { return qplot_; }

  CQGnuPlotEnum::BoxWidthType type() const {
    return CQGnuPlotEnum::boxWidthTypeConv(CGnuPlotBoxErrorBarsStyleValue::type());
  }

  void setType(const CQGnuPlotEnum::BoxWidthType &type) {
    if (type != this->type()) {
      CGnuPlotBoxErrorBarsStyleValue::setType(CQGnuPlotEnum::boxWidthTypeConv(type));
    }
  }

 private:
  CQGnuPlotPlot *qplot_;
};

#endif
